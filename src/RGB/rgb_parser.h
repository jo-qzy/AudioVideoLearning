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
        unsigned short int  bfType;       //λͼ�ļ������ͣ�Windows�±���ΪBM 
        unsigned long       bfSize;       //�ļ���С�����ֽ�Ϊ��λ
        unsigned short int  bfReserverd1; //λͼ�ļ������֣�����Ϊ0 
        unsigned short int  bfReserverd2; //λͼ�ļ������֣�����Ϊ0 
        unsigned long       bfbfOffBits;  //λͼ�ļ�ͷ�����ݵ�ƫ���������ֽ�Ϊ��λ
    };

    struct BIT_MAP_INFO_HEADER
    {
        long        biSize;          //�ýṹ��С���ֽ�Ϊ��λ
        long        biWidth;         //ͼ�ο��������Ϊ��λ
        long        biHeight;        //ͼ�θ߶�������Ϊ��λ
        short int   biPlanes;        //Ŀ���豸�ļ��𣬱���Ϊ1 
        short int   biBitcount;      //��ɫ��ȣ�ÿ����������Ҫ��λ��
        long        biCompression;   //λͼ��ѹ������
        long        biSizeImage;     //λͼ�Ĵ�С�����ֽ�Ϊ��λ
        long        biXPelsPermeter; //λͼˮƽ�ֱ��ʣ�ÿ��������
        long        biYPelsPermeter; //λͼ��ֱ�ֱ��ʣ�ÿ��������
        long        biClrUsed;       //λͼʵ��ʹ�õ���ɫ���е���ɫ��
        long        biClrImportant;  //λͼ��ʾ��������Ҫ����ɫ��
    };
};

#endif
