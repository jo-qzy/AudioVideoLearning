#include "rgb_parser.h"

void test_rgb_parser()
{
    RgbParser::rgb24_split("cie1931_500x500.rgb", 500, 500, 1);
    RgbParser::rgb24_to_bmp("lena512_rgb24.rgb", 512, 512);
    RgbParser::rgb24_to_yuv420p("lena512_rgb24.rgb", 512, 512);
    RgbParser::rgb24_colorbar(1024, 512);
}

int main()
{
    test_rgb_parser();
    printf("All pic generate finished!\n");
    printf("All files will be named output* and stored in the current folder!\n");
    system("pause");
    return 0;
}