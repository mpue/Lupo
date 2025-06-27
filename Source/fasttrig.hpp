#pragma once
/**
 * fast_trig.hpp
 * --------------
 * Header-only, blitzschnelle Sinus- und Cosinus-Approximation auf Basis einer
 * Zweierpotenz-Lookup-Tabelle mit linearer Interpolation.
 *
 *   Tabellengrî·e: 4096 EintrÑge (~0,0015ˇradˇ~ˇ0,086¯ Auflîsung, 32ˇKiB fÅr
 *    Sinus & Cosinus zusammen).
 *   Konstruktion: Eine einzige statische Instanz wird beim ersten Aufruf
 *    erzeugt - thread-safe seit C++11.
 *   ôffentliche API: float sin_fast(float rad), float cos_fast(float rad)
 *
 *  Kompilieren mit z.ˇB.:
 *      g++ -std=c++20 -O3 -ffast-math   .
 *
 *  Beispiel:
 *      #include "fast_trig.hpp"
 *      int main()
 *      {
 *          float a = 1.2345f;
 *          float s = fast_trig::sin_fast(a);
 *          float c = fast_trig::cos_fast(a);
 *      }
 */

#include <array>
#include <cmath>
#include <cstddef>

namespace fast_trig {

/*-----------------------------------------------------
  Grundlegende Konstanten
  ----------------------------------------------------*/
constexpr std::size_t LUT_BITS = 10;           // 2^12 = 4096
constexpr std::size_t LUT_SIZE = 1u << LUT_BITS;
constexpr std::size_t LUT_MASK = LUT_SIZE - 1u;

constexpr float TWO_PI     = 6.283185307179586476925286766559f;
constexpr float INV_TWO_PI = 1.0f / TWO_PI;

/*-----------------------------------------------------
  Hilfsfunktion: Winkel in [0, 2p) falten (branch-frei)
  ----------------------------------------------------*/
inline float wrap(float x) noexcept
{
    x -= TWO_PI * std::floor(x * INV_TWO_PI);
    return x;
}

/*-----------------------------------------------------
  Lookup-Tabelle, einmalig initialisiert
  ----------------------------------------------------*/
class TrigLUT
{
public:
    TrigLUT()
    {
        for (std::size_t i = 0; i < LUT_SIZE; ++i)
        {
            const float angle = static_cast<float>(i) * TWO_PI / static_cast<float>(LUT_SIZE);
            sin_[i] = std::sin(angle);
            cos_[i] = std::cos(angle);
        }
    }

    static const TrigLUT& instance()
    {
        static TrigLUT lut; // thread-safe ab C++11
        return lut;
    }

    inline float sin(std::size_t idx) const noexcept { return sin_[idx]; }
    inline float cos(std::size_t idx) const noexcept { return cos_[idx]; }

private:
    float sin_[LUT_SIZE];
    float cos_[LUT_SIZE];
};

/*-----------------------------------------------------
  ôffentliche API: schnelle Sinus- und Cosinusfunktionen
  ----------------------------------------------------*/
inline float sin_fast(float radians) noexcept
{
    const auto& lut = TrigLUT::instance();

    const float wrapped = wrap(radians);
    const float fIdx    = wrapped * (static_cast<float>(LUT_SIZE) * INV_TWO_PI);

    const std::size_t i0 = static_cast<std::size_t>(fIdx);
    const std::size_t i1 = (i0 + 1u) & LUT_MASK;            // nÑchster Index (gewrappt)
    const float       t  = fIdx - static_cast<float>(i0);   // Interpolationsfaktor

    return lut.sin(i0) + (lut.sin(i1) - lut.sin(i0)) * t;   // linear
}

inline float cos_fast(float radians) noexcept
{
    const auto& lut = TrigLUT::instance();

    const float wrapped = wrap(radians);
    const float fIdx    = wrapped * (static_cast<float>(LUT_SIZE) * INV_TWO_PI);

    const std::size_t i0 = static_cast<std::size_t>(fIdx);
    const std::size_t i1 = (i0 + 1u) & LUT_MASK;
    const float       t  = fIdx - static_cast<float>(i0);

    return lut.cos(i0) + (lut.cos(i1) - lut.cos(i0)) * t;
}

/*-----------------------------------------------------
  Bequeme double-Overloads (Delegation auf float-Pfad)
  ----------------------------------------------------*/
inline double sin_fast(double radians) noexcept
{
    return static_cast<double>(sin_fast(static_cast<float>(radians)));
}

inline double cos_fast(double radians) noexcept
{
    return static_cast<double>(cos_fast(static_cast<float>(radians)));
}

} // namespace fast_trig
