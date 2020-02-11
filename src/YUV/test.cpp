#include "yuv_parser.h"

void test_yuv_parser()
{
    /*
    YuvParser::yuv420_split("lena_512_yuv420p.yuv", 512, 512, 1);
    YuvParser::yuv444_split("lena_512_yuv444p.yuv", 512, 512, 1);
    YuvParser::yuv420_gray("lena_512_yuv420p.yuv", 512, 512, 1);
    YuvParser::yuv420_half_bright("lena_512_yuv420p.yuv", 512, 512, 1);
    YuvParser::yuv420_border("lena_512_yuv420p.yuv", 512, 512, 20, 1);
    */
    //YuvParser::yuv420_graybar(1024, 512, 0, 255, 10);

    YuvParser::yuv420_psnr("lena_256_yuv420p.yuv", "lena_distort_256_yuv420p.yuv", 256, 256, 1);
}

int main()
{
    test_yuv_parser();
    printf("All yuv pic create finished!\n");
    system("pause");
    return 0;
}