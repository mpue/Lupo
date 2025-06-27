// ===================== MultithreadVoiceProcessor.h =====================
#pragma once

#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

class Voice; // forward declaration – your existing Voice class

/**
 *  Lock‑free* Voice mixer for LupoSynth (C++14).
 *  Splits all voices über mehrere Worker‑Threads, sammelt sie in Stereo‑Buffers
 *  und gibt das Ergebnis an den Audio‑Thread zurück.
 *
 *  *Alle kritischen Abschnitte vermeiden Mutex‑Locking im Audio‑Thread;
 *  einzig two condition variables werden verwendet, sind aber ausserhalb
 *  der Echtzeit‑Zone gesperrt.
 */
class MultithreadVoiceProcessor
{
public:
    /**
     * @param voices        reference auf das Voice‑Vektor deiner Engine
     * @param maxBlockSize  größte Buffer‑Länge (samplesPerBlock)
     * @param numThreads    >0 ⇒ feste Anzahl ‑ sonst automatic (cores‑1)
     */
    MultithreadVoiceProcessor(std::vector<std::unique_ptr<Voice>>& voices,
        int maxBlockSize,
        int numThreads = 0);
    ~MultithreadVoiceProcessor();

    /** mischt alle Voices in outL/outR (müssen numSamples floats haben) */
    void process(float* outL, float* outR,
        int numSamples,
        float masterGain);

private:
    void workerLoop(int id);

    std::vector<std::unique_ptr<Voice>>& voices;
    const int maxBlock;
    int threads;

    std::vector<std::thread> pool;
    std::atomic<bool> quit{ false };

    std::vector<std::vector<float>> accL, accR; // per‑Thread Akkumulatoren

    /** Zweistufige Barriere ➜ vermeidet Dead‑Locks beim Schließen */
    std::mutex startMtx;              std::condition_variable cvStart;
    std::mutex doneMtx;               std::condition_variable cvDone;

    std::atomic<int>  runningWorkers{ 0 };  // zählt aktive Worker
    std::atomic<bool> workReady{ false };

    // aktuelle Callback‑Parameter
    int   curSamples{ 0 };
    float curGain{ 1.f };
};