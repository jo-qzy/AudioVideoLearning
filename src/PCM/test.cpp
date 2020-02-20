#include "pcm_parser.h"

void test_pcm_parser()
{
    PcmParser::pcm_split("MoonAndMyth.pcm");
}

int main()
{
    test_pcm_parser();
    printf("All audio generate finished!\n");
    printf("All files will be named output* and stored in the current folder!\n");
    system("pause");
    return 0;
}