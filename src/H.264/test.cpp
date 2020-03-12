#include "h264_parser.h"

int main()
{
    H264Parser parser("NewRules.264");
    parser.h264_parser();
    system("pause");
    return 0;
}