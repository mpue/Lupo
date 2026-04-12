#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
#include <array>
#include <atomic>

// Manages MIDI CC learn assignments for plugin parameters.
// Thread-safe: processMidiCC() may be called from the audio thread.
class MidiLearnManager
{
public:
    MidiLearnManager() = default;

    // Start learning mode for the given parameter (call from message thread).
    void startLearning(const String& paramID)
    {
        const ScopedLock sl(lock);
        learningParamID = paramID;
        learning = true;
    }

    void cancelLearning()
    {
        const ScopedLock sl(lock);
        learning = false;
        learningParamID = {};
    }

    bool isCurrentlyLearning() const
    {
        const ScopedLock sl(lock);
        return learning;
    }

    String getCurrentLearningParam() const
    {
        const ScopedLock sl(lock);
        return learningParamID;
    }

    // Called from the audio thread. Handles MIDI CC learn assignment or pending value buffering.
    // Returns true if the CC was consumed.
    // Call applyPendingUpdates() from the message thread to actually push values to parameters.
    bool processMidiCC(int ccNumber, int value)
    {
        if (ccNumber < 0 || ccNumber >= 128)
            return false;

        const ScopedTryLock stl(lock);
        if (!stl.isLocked())
            return false;

        if (learning && !learningParamID.isEmpty())
        {
            // Remove any existing mapping for this CC number
            auto itCC = ccToParam.find(ccNumber);
            if (itCC != ccToParam.end())
            {
                paramToCC.erase(itCC->second);
                ccToParam.erase(itCC);
            }
            // Remove any existing CC for this param
            auto itParam = paramToCC.find(learningParamID);
            if (itParam != paramToCC.end())
            {
                ccToParam.erase(itParam->second);
                paramToCC.erase(itParam);
            }

            ccToParam[ccNumber] = learningParamID;
            paramToCC[learningParamID] = ccNumber;
            learning = false;
            learningParamID = {};
            dirty = true;
            return true;
        }

        if (ccToParam.count(ccNumber) > 0)
        {
            // Buffer the value; message thread will apply it via applyPendingUpdates()
            pendingValues[ccNumber].store(static_cast<float>(value) / 127.0f,
                                          std::memory_order_relaxed);
            return true;
        }

        return false;
    }

    // Call from the message thread (e.g. timer callback) to push buffered CC values to parameters.
    void applyPendingUpdates(AudioProcessorValueTreeState& apvts)
    {
        for (int cc = 0; cc < 128; ++cc)
        {
            float v = pendingValues[cc].exchange(-1.0f, std::memory_order_relaxed);
            if (v >= 0.0f)
            {
                const ScopedLock sl(lock);
                auto it = ccToParam.find(cc);
                if (it != ccToParam.end())
                {
                    if (auto* param = apvts.getParameter(it->second))
                        param->setValueNotifyingHost(v);
                }
            }
        }
    }

    // Returns the CC number assigned to paramID, or -1 if none.
    int getCCForParam(const String& paramID) const
    {
        const ScopedLock sl(lock);
        auto it = paramToCC.find(paramID);
        return (it != paramToCC.end()) ? it->second : -1;
    }

    void clearParam(const String& paramID)
    {
        const ScopedLock sl(lock);
        auto it = paramToCC.find(paramID);
        if (it != paramToCC.end())
        {
            ccToParam.erase(it->second);
            paramToCC.erase(it);
            dirty = true;
        }
    }

    // True if mappings have changed since the last save.
    bool isDirty() const
    {
        const ScopedLock sl(lock);
        return dirty;
    }

    void saveToFile(const File& f)
    {
        const ScopedLock sl(lock);
        String s;
        for (auto& kv : paramToCC)
            s += kv.first + "=" + String(kv.second) + "\n";
        f.replaceWithText(s);
        dirty = false;
    }

    void loadFromFile(const File& f)
    {
        const ScopedLock sl(lock);
        ccToParam.clear();
        paramToCC.clear();
        if (!f.exists())
            return;

        StringArray lines;
        lines.addLines(f.loadFileAsString());
        for (auto& line : lines)
        {
            int eq = line.indexOf("=");
            if (eq > 0)
            {
                String pid = line.substring(0, eq).trim();
                int cc = line.substring(eq + 1).trim().getIntValue();
                if (!pid.isEmpty() && cc >= 0 && cc < 128)
                {
                    paramToCC[pid] = cc;
                    ccToParam[cc] = pid;
                }
            }
        }
    }

private:
    mutable CriticalSection lock;
    bool dirty = false;
    bool learning = false;
    String learningParamID;
    std::map<int, String> ccToParam;
    std::map<String, int> paramToCC;
    // Pending CC values written by audio thread, read by message thread. -1 = no pending.
    std::array<std::atomic<float>, 128> pendingValues { {} };

    struct PendingInit {
        PendingInit(std::array<std::atomic<float>, 128>& arr) {
            for (auto& v : arr) v.store(-1.0f, std::memory_order_relaxed);
        }
    } pendingInit { pendingValues };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiLearnManager)
};
