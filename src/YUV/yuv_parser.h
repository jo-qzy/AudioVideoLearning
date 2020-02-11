#ifndef __YUV_PARSER_H__
#define __YUV_PARSER_H__

#include <string>

class YuvParser
{
public:
    // Split Y, U, V planes in YUV420P file.
    // @param [in] Location of input yuv file.
    // @param [in] Width of input yuv file.
    // @param [in] Height of input yuv file.
    // @param [in] Number of frames to process.
    // @return True if success, else false.
    static bool yuv420_split(const std::string input_url, int width, int height, int frame_num);

    // Split Y, U, V planes in YUV444P file.
    // @param [in] Location of input yuv file.
    // @param [in] Width of input yuv file.
    // @param [in] Height of input yuv file.
    // @param [in] Number of frames to process.
    // @return True if success, else false.
    static bool yuv444_split(const std::string input_url, int width, int height, int frame_num);

    // Convert YUV420P file to gray picture.
    // @param [in] Location of input yuv file.
    // @param [in] Width of input yuv file.
    // @param [in] Height of input yuv file.
    // @param [in] Number of frames to process.
    // @return True if success, else false.
    static bool yuv420_gray(const std::string input_url, int width, int height, int frame_num);

    // Halving YUV420P image brightness.
    // @param [in] Location of input yuv file.
    // @param [in] Width of input yuv file.
    // @param [in] Height of input yuv file.
    // @param [in] Number of frames to process.
    // @return True if success, else false.
    static bool yuv420_half_bright(const std::string input_url, int width, int height, int frame_num);

    // Add border for YUV420P file.
    // @param [in] Location of input yuv file.
    // @param [in] Width of input yuv file.
    // @param [in] Height of input yuv file.
    // @param [in] Number of frames to process.
    // @return True if success, else false.
    static bool yuv420_border(const std::string input_url, int width, int height, int border_length, int frame_num);

    // Generate YUV420P gray scale bar.
    // @param [in] Width of output yuv file.
    // @param [in] Height of output yuv file.
    // @param [in] Minimum value of Y.
    // @param [in] Maximum value of Y.
    // @param [in] Number of gray bar.
    // @return True if success, else false.
    static bool yuv420_graybar(int width, int height, int y_min, int y_max, int bar_num);

    // Calculate PSNR between 2 YUV420P file.
    // @param [in] Location of first input yuv file.
    // @param [in] Location of second input yuv file.
    // @param [in] Width of input yuv file.
    // @param [in] Height of input yuv file.
    // @param [in] Number of frames to process.
    // @return True if success, else false.
    static bool yuv420_psnr(const std::string input_url1, const std::string input_url2, int width, int height, int frame_num);
};

#endif
