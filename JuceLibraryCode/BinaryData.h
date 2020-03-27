/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   DSEG14ClassicRegular_ttf;
    const int            DSEG14ClassicRegular_ttfSize = 28416;

    extern const char*   Knob_32_png;
    const int            Knob_32_pngSize = 207362;

    extern const char*   Knob_48_png;
    const int            Knob_48_pngSize = 446229;

    extern const char*   Knob_64_png;
    const int            Knob_64_pngSize = 762558;

    extern const char*   Knob_128_png;
    const int            Knob_128_pngSize = 2692011;

    extern const char*   logo_png;
    const int            logo_pngSize = 19035;

    extern const char*   oscillator_noise_48_png;
    const int            oscillator_noise_48_pngSize = 17465;

    extern const char*   oscillator_saw_48_png;
    const int            oscillator_saw_48_pngSize = 15735;

    extern const char*   oscillator_sine_48_png;
    const int            oscillator_sine_48_pngSize = 18394;

    extern const char*   oscillator_square_48_png;
    const int            oscillator_square_48_pngSize = 17545;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 10;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
