#pragma once

#undef clamp

#include<cmath>
namespace util {
    template<typename T>
    constexpr const T& clamp(const T& value, const T& min, const T& max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    static float mapEnvelopeTime(float normalizedValue) {
        // normalizedValue is in [0.0, 1.0]
        const float minTime = 0.001f; // 1ms
        const float maxTime = 1.0f;  // 10s
        return minTime * std::pow(maxTime / minTime, normalizedValue);
    }
}