#ifndef __PCM_PARSER_H__
#define __PCM_PARSER_H__

#include <string>
#include <cstring>

class Wave;

class PcmParser
{
public:
    static bool pcm16be_to_pcm16le(const std::string url);

    static bool pcm16_split(const std::string url);

    static bool pcm16be_half_volume_left(const std::string url);

    static bool pcm16be_double_speed(const std::string url);

    static bool pcm16be_to_pcm8(const std::string url);

    static bool pcm16be_cut(const std::string url, size_t start, size_t end);

    static bool pcm16be_to_wave(const std::string url, int channels = 0, int sample_rate = 0);
};

#pragma pack(1)

class Wave
{
public:
    struct WAVE_HEADER
    {
        char         ChunkID[4];
        unsigned int ChunkSize;
        char         FccType[4];
    };

    struct WAVE_FMT
    {
        char           ChunkID[4];
        unsigned int   ChunkSize;
        unsigned short FormatTag;
        unsigned short Channels;
        unsigned int   SamplePerSec;
        unsigned int   BytesPerSec;
        unsigned short BlockAlign;
        unsigned short BitsPerSample;
    };

    struct WAVE_DATA
    {
        char         ChunkID[4];
        unsigned int ChunkSize;
    };
};

#endif
