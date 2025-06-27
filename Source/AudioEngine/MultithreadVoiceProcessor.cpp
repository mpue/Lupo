// ===================== MultithreadVoiceProcessor.cpp =====================
#include "MultithreadVoiceProcessor.h"
#include "Voice.h"   // passe den Pfad ggf. an
#include <algorithm>
#include <cmath>

MultithreadVoiceProcessor::MultithreadVoiceProcessor(
    std::vector<std::unique_ptr<Voice>>& v,
    int maxBlockSize,
    int numThreads)
    : voices(v), maxBlock(maxBlockSize)
{
    threads = (numThreads > 0)
        ? numThreads
        : std::max(1u, std::thread::hardware_concurrency() - 1);

    accL.resize(threads);
    accR.resize(threads);
    for (int t = 0; t < threads; ++t)
    {
        accL[t].resize(maxBlock);
        accR[t].resize(maxBlock);
        pool.emplace_back([this, t] { workerLoop(t); });
    }
}

MultithreadVoiceProcessor::~MultithreadVoiceProcessor()
{
    quit = true;

    // Worker wecken (Start‑Phase)
    {
        std::lock_guard<std::mutex> lk(startMtx);
        workReady = true;
    }
    cvStart.notify_all();

    // Audio‑Thread aus warten lösen (Done‑Phase)
    {
        std::lock_guard<std::mutex> lk(doneMtx);
        runningWorkers = 0;
    }
    cvDone.notify_one();

    for (auto& th : pool)
        if (th.joinable()) th.join();
}

void MultithreadVoiceProcessor::process(float* outL, float* outR,
    int numSamples, float masterGain)
{
    curSamples = numSamples;
    curGain = masterGain;

    for (int t = 0; t < threads; ++t)
    {
        std::fill(accL[t].begin(), accL[t].begin() + numSamples, 0.0f);
        std::fill(accR[t].begin(), accR[t].begin() + numSamples, 0.0f);
    }
    std::fill(outL, outL + numSamples, 0.0f);
    std::fill(outR, outR + numSamples, 0.0f);

    // -------- Start‑Barriere --------
    {
        std::lock_guard<std::mutex> lk(startMtx);
        runningWorkers = threads;
        workReady = true;
    }
    cvStart.notify_all();

    // -------- Done‑Barriere --------
    {
        std::unique_lock<std::mutex> lk(doneMtx);
        cvDone.wait(lk, [this] { return runningWorkers == 0; });
    }

    // Mixdown
    for (int t = 0; t < threads; ++t)
    {
        const float* l = accL[t].data();
        const float* r = accR[t].data();
        for (int s = 0; s < numSamples; ++s)
        {
            outL[s] += l[s];
            outR[s] += r[s];
        }
    }
}

void MultithreadVoiceProcessor::workerLoop(int id)
{
    while (true)
    {
        // Warten auf Arbeit
        {
            std::unique_lock<std::mutex> lk(startMtx);
            cvStart.wait(lk, [this] { return workReady || quit; });
            if (quit) return;
        }

        const int voiceCount = static_cast<int>(voices.size());
        float* sumL = accL[id].data();
        float* sumR = accR[id].data();

        for (int s = 0; s < curSamples; ++s)
        {
            float l = 0.f, r = 0.f;
            for (int v = id; v < voiceCount; v += threads)
            {
                Voice* voice = voices[v].get();
                if (voice->getAmpEnvelope()->getState() != SynthLab::ADSR::env_idle)
                {
                    l += voice->process(0) * curGain;
                    r += voice->process(1) * curGain;
                }
            }
            sumL[s] = l;
            sumR[s] = r;
        }

        // Fertig → Done‑Barriere
        {
            std::lock_guard<std::mutex> lk(doneMtx);
            if (--runningWorkers == 0)
            {
                workReady = false;
                cvDone.notify_one();
            }
        }
    }

}
