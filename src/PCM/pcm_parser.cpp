#include "pcm_parser.h"

bool PcmParser::pcm_split(const std::string url)
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