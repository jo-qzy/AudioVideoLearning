#ifndef __RGB_PARSER_H__
#define __RGB_PARSER_H__

#include <string>

class RgbParser
{
public:
    // Split R, G, B planes in RGB24 file.
    // @param [in] Location of input yuv file.
    // @param [in] Width of input RGB file.
    // @param [in] Height of input RGB file.
    // @param [in] Number of frames to process.
    // @return True if success, else false.
    static bool rgb24_split(const std::string input_url, int width, int height, int frame_num);

    // Convert RGB24 file to BMP file.
    // @param [in] Location of input yuv file.
    // @param [in] Width of input RGB file.
    // @param [in] Height of input RGB file.
    // @return True if success, else false.
    static bool rgb24_to_bmp(const std::string input_url, int width, int height);

    // Convert RGB24 file to YUV420P file.
    // @param [in] Location of input yuv file.
    // @param [in] Width of input RGB file.
    // @param [in] Height of input RGB file.
    // @return True if success, else false.
    static bool rgb24_to_yuv420p(const std::string input_url, int width, int height);

    // Genetate RGB24 colorbar.
    // @param [in] Width of input RGB file.
    // @param [in] Height of input RGB file.
    // @return True if success, else false.
    static bool rgb24_colorbar(int width, int height);
};

#pragma pack(1)

class BMP
{
public:
    struct BIT_MAP_FILE_HEADER
    {
        unsigned short int  bfType;       //位图文件的类型，Windows下必须为BM 
        unsigned long       bfSize;       //文件大小，以字节为单位
        unsigned short int  bfReserverd1; //位图文件保留字，必须为0 
        unsigned short int  bfReserverd2; //位图文件保留字，必须为0 
        unsigned long       bfbfOffBits;  //位图文件头到数据的偏移量，以字节为单位
    };

    struct BIT_MAP_INFO_HEADER
    {
        long        biSize;          //该结构大小，字节为单位
        long        biWidth;         //图形宽度以象素为单位
        long        biHeight;        //图形高度以象素为单位
        short int   biPlanes;        //目标设备的级别，必须为1 
        short int   biBitcount;      //颜色深度，每个象素所需要的位数
        long        biCompression;   //位图的压缩类型
        long        biSizeImage;     //位图的大小，以字节为单位
        long        biXPelsPermeter; //位图水平分辨率，每米像素数
        long        biYPelsPermeter; //位图垂直分辨率，每米像素数
        long        biClrUsed;       //位图实际使用的颜色表中的颜色数
        long        biClrImportant;  //位图显示过程中重要的颜色数
    };
};

#endif
