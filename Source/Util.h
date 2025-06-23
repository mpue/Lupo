#pragma once

#undef clamp

namespace util {
    template<typename T>
    constexpr const T& clamp(const T& value, const T& min, const T& max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
}