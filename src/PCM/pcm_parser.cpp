#include "pcm_parser.h"

bool PcmParser::pcm16be_to_pcm16le(const std::string url)
{
    // It also can be Little Endian to Big Endan
    size_t dot_pos = url.rfind('.');
    std::string output_url(url.begin(), url.begin() + dot_pos);
    output_url += "(LE).pcm";
    FILE* input = fopen(url.c_str(), "rb+");
    FILE* output = fopen(output_url.c_str(), "wb+");

    unsigned char* voice = new unsigned char[4];

    while (!feof(input)) {
        fread(voice, 1, 4, input);
        fwrite(voice + 1, 1, 1, output);
        fwrite(voice, 1, 1, output);
        fwrite(voice + 3, 1, 1, output);
        fwrite(voice + 2, 1, 1, output);
    }

    fclose(input);
    fclose(output);
    delete[] voice;
    return true;
}

bool PcmParser::pcm16_split(const std::string url)
{
    FILE *input = fopen(url.c_str(), "rb+");
    FILE *output_left = fopen("output_left.pcm", "wb+");
    FILE *output_right = fopen("output_right.pcm", "wb+");

    unsigned char *voice = new unsigned char[4];

    while (!feof(input)) {
        fread(voice, 1, 4, input);
        fwrite(voice, 1, 2, output_left);
        fwrite(voice + 2, 1, 2, output_right);
    }

    fclose(input);
    fclose(output_left);
    fclose(output_right);
    delete[] voice;
    return true;
}

bool PcmParser::pcm16be_half_volume_left(const std::string url)
{
    FILE* input = fopen(url.c_str(), "rb+");
    FILE* output_half_volume_left = fopen("output_half_volume_left.pcm", "wb+");

    unsigned char* voice = new unsigned char[4];

    // Little Endian machine need ateention:You cannot directly
    // convert Big Endian pcm data to short on Little Endian machine
    short* tmp_data = new short;
    unsigned char* low_byte = reinterpret_cast<unsigned char*>(tmp_data);
    unsigned char* high_byte = reinterpret_cast<unsigned char*>(tmp_data) + 1;


    while (!feof(input)) {
        fread(voice, 1, 4, input);
        *high_byte = voice[0];
        *low_byte = voice[1];
        *tmp_data /= 2;
        fwrite(high_byte, 1, 1, output_half_volume_left);
        fwrite(low_byte, 1, 1, output_half_volume_left);
        fwrite(voice + 2, 1, 2, output_half_volume_left);
    }

    fclose(input);
    fclose(output_half_volume_left);
    delete[] voice;
    return true;
}

bool PcmParser::pcm16be_double_speed(const std::string url)
{
    FILE* input = fopen(url.c_str(), "rb+");
    FILE* output_double_speed = fopen("output_double_speed.pcm", "wb+");

    int count = 0;

    unsigned char* voice = new unsigned char[4];

    while (!feof(input)) {
        fread(voice, 1, 4, input);
        if (0 == count % 2) {
            fwrite(voice, 1, 2, output_double_speed);
            fwrite(voice + 2, 1, 2, output_double_speed);
        }
        count++;
    }

    fclose(input);
    fclose(output_double_speed);
    delete[] voice;
    return true;
}

bool PcmParser::pcm16be_to_pcm8(const std::string url)
{
    FILE* input = fopen(url.c_str(), "rb+");
    FILE* output_pcm8 = fopen("output_pcm8.pcm", "wb+");

    unsigned char* voice = new unsigned char[4];

    while (!feof(input)) {
        fread(voice, 1, 4, input);
        voice[0] += 128;
        voice[2] += 128;
        fwrite(voice, 1, 1, output_pcm8);
        fwrite(voice + 2, 1, 1, output_pcm8);
    }

    fclose(input);
    fclose(output_pcm8);
    delete[] voice;
    return true;
}

bool PcmParser::pcm16be_cut(const std::string url, size_t start, size_t end)
{
    FILE* input = fopen(url.c_str(), "rb+");
    FILE* output_cut = fopen("output_cut.pcm", "wb+");

    unsigned char* voice = new unsigned char[4];
    size_t cur_pos = 0;

    while (!feof(input) && cur_pos <= end) {
        fread(voice, 1, 4, input);
        if (cur_pos >= start) {
            fwrite(voice, 1, 4, output_cut);
        }
        cur_pos++;
    }

    fclose(input);
    fclose(output_cut);
    delete[] voice;
    return true;
}

bool PcmParser::pcm16be_to_wave(const std::string url, int channels, int sample_rate)
{
    size_t dot_pos = url.rfind('.');
    std::string output_url(url.begin(), url.begin() + dot_pos);
    output_url += ".wav";
    FILE* input = fopen(url.c_str(), "rb+");
    FILE* output_wave = fopen(output_url.c_str(), "wb+");

    unsigned char* voice = new unsigned char[44];
    unsigned int pcm_data_size = 0;

    // Simplest WAV file = WAV header + PCM data
    Wave::WAVE_HEADER wave_header;
    Wave::WAVE_FMT wave_fmt;
    Wave::WAVE_DATA wave_data;
    long header_size = sizeof(wave_header) + sizeof(wave_fmt) + sizeof(wave_data);

    // Write PCM data first
    fseek(output_wave, header_size, SEEK_CUR);
    while (!feof(input)) {
        fread(voice, 1, 2, input);
        // Save data in Little Endian
        fwrite(voice + 1, 1, 1, output_wave);
        fwrite(voice, 1, 1, output_wave);
        pcm_data_size += 2;
    }

    // Orgnize WAV header
    memcpy(wave_header.ChunkID, "RIFF", strlen("RIFF"));
    wave_header.ChunkSize = header_size + pcm_data_size;
    memcpy(wave_header.FccType, "WAVE", strlen("WAVE"));

    memcpy(wave_fmt.ChunkID, "fmt ", strlen("fmt "));
    wave_fmt.ChunkID[4] = ' ';
    wave_fmt.ChunkSize = sizeof(Wave::WAVE_FMT) - 8;
    wave_fmt.FormatTag = 1;
    if (channels == 0 || sample_rate == 0) {
        wave_fmt.Channels = 2;
        wave_fmt.SamplePerSec = 44100;
    } else {
        wave_fmt.Channels = channels;
        wave_fmt.SamplePerSec = sample_rate;
    }
    wave_fmt.BytesPerSec = wave_fmt.SamplePerSec * 16;
    wave_fmt.BlockAlign = 4;
    wave_fmt.BitsPerSample = 16;

    memcpy(wave_data.ChunkID, "data", strlen("data"));
    wave_data.ChunkSize = pcm_data_size;

    rewind(output_wave);
    fwrite(&wave_header, sizeof(Wave::WAVE_HEADER), 1, output_wave);
    fwrite(&wave_fmt, sizeof(Wave::WAVE_FMT), 1, output_wave);
    fwrite(&wave_data, sizeof(Wave::WAVE_DATA), 1, output_wave);

    fclose(input);
    fclose(output_wave);
    delete[] voice;
    return true;
}
