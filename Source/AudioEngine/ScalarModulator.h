#pragma once
#include "Modulator.h"
#include <atomic>

/**
 * A Modulator whose output is a single scalar value set from outside
 * (e.g. MIDI CC, velocity). Thread-safe via atomic.
 */
class ScalarModulator : public Modulator
{
public:
    void setValue(float v) noexcept { value.store(v); }

    float process()   override { return value.load(); }
    float getOutput() override { return value.load(); }

private:
    std::atomic<float> value { 0.0f };
};
