#include "pcm_parser.h"

void test_pcm_parser()
{
    PcmParser::pcm16be_to_pcm16le("MoonAndMyth.pcm");
    PcmParser::pcm16_split("MoonAndMyth.pcm");
    PcmParser::pcm16be_half_volume_left("MoonAndMyth.pcm");
    PcmParser::pcm16be_double_speed("MoonAndMyth.pcm");
    PcmParser::pcm16be_to_pcm8("MoonAndMyth.pcm");
    PcmParser::pcm16be_cut("MoonAndMyth.pcm", 0, 240000);
    PcmParser::pcm16be_to_wave("MoonAndMyth.pcm");
}

int main()
{
    test_pcm_parser();
    printf("All audio generate finished!\n");
    printf("All files will be named output* and stored in the current folder!\n");
    system("pause");
    return 0;
}