#pragma once
// Minimal mmreg types: WAVEFORMATEX used by xrSound
#include <stdint.h>

#define WAVE_FORMAT_PCM 1

typedef struct tWAVEFORMATEX
{
    uint16_t wFormatTag;
    uint16_t nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t nBlockAlign;
    uint16_t wBitsPerSample;
    uint16_t cbSize;
} WAVEFORMATEX, *LPWAVEFORMATEX;
