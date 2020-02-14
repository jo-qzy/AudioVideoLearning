#include "yuv_parser.h"
#include <fstream>
#include <cmath>

bool YuvParser::yuv420_split(const std::string input_url, int width, int height, int frame_num)
{
    FILE *input_file = fopen(input_url.c_str(), "rb+");
    FILE *output_y = fopen("output_420_y.y", "wb+");
    FILE *output_u = fopen("output_420_u.y", "wb+");
    FILE *output_v = fopen("output_420_v.y", "wb+");

    unsigned char *picture = new unsigned char[width * height * 3 / 2];

    for (int i = 0; i < frame_num; i++) {
        fread(picture, 1, width * height * 3 / 2, input_file);

        fwrite(picture, 1, width * height, output_y);
        fwrite(picture + width * height, 1, width * height / 4, output_u);
        fwrite(picture + width * height * 5 / 4, 1, width * height / 4, output_v);
    }

    delete[] picture;
    fclose(input_file);
    fclose(output_y);
    fclose(output_u);
    fclose(output_v);
    return true;
}

bool YuvParser::yuv444_split(const std::string input_url, int width, int height, int frame_num)
{
    FILE *input_file = fopen(input_url.c_str(), "rb+");
    FILE *output_y = fopen("output_444_y.y", "wb+");
    FILE *output_u = fopen("output_444_u.y", "wb+");
    FILE *output_v = fopen("output_444_v.y", "wb+");

    unsigned char *picture = new unsigned char[width * height * 3];

    for (int i = 0; i < frame_num; i++) {
        fread(picture, 1, width * height * 3, input_file);

        fwrite(picture, 1, width * height, output_y);
        fwrite(picture + width * height, 1, width * height, output_u);
        fwrite(picture + width * height * 2, 1, width * height, output_v);
    }

    delete[] picture;
    fclose(input_file);
    fclose(output_y);
    fclose(output_u);
    fclose(output_v);
    return true;
}

bool YuvParser::yuv420_gray(const std::string input_url, int width, int height, int frame_num)
{
    FILE *input_file = fopen(input_url.c_str(), "rb+");
    FILE *output_gray = fopen("output_420_gray.yuv", "wb+");

    unsigned char *picture = new unsigned char[width * height * 3 / 2];

    for (int i = 0; i < frame_num; i++) {
        fread(picture, 1, width * height * 3 / 2, input_file);
        memset(picture + width * height, 128, width * height / 2);
        fwrite(picture, 1, width * height * 3 / 2, output_gray);
    }

    delete[] picture;
    fclose(input_file);
    fclose(output_gray);
    return true;
}

bool YuvParser::yuv420_half_bright(const std::string input_url, int width, int height, int frame_num)
{
    FILE *input_file = fopen(input_url.c_str(), "rb+");
    FILE *output_half_bright = fopen("output_420_half_bright.yuv", "wb+");

    unsigned char *picture = new unsigned char[width * height * 3 / 2];

    for (int i = 0; i < frame_num; i++) {
        fread(picture, 1, width * height * 3 / 2, input_file);
        for (int cur_pixel = 0; cur_pixel < width * height; cur_pixel++) {
            // half Y
            picture[cur_pixel] /= 2;
        }
        fwrite(picture, 1, width * height * 3 / 2, output_half_bright);
    }

    delete[] picture;
    fclose(input_file);
    fclose(output_half_bright);
    return true;
}

bool YuvParser::yuv420_border(const std::string input_url, int width, int height, int border_length, int frame_num)
{
    FILE *input_file = fopen(input_url.c_str(), "rb+");
    FILE *output_border = fopen("output_420_border.yuv", "wb+");

    unsigned char *picture = new unsigned char[width * height * 3 / 2];

    for (int i = 0; i < frame_num; i++) {
        fread(picture, 1, width * height * 3 / 2, input_file);
        for (int cur_height = 0; cur_height < height; cur_height++) {
            for (int cur_width = 0; cur_width < width; cur_width++) {
                if (cur_width < border_length || cur_width > width - border_length ||
                    cur_height < border_length || cur_height > height - border_length) {
                    picture[cur_height * width + cur_width] = 255;
                }
            }
        }
        fwrite(picture, 1, width * height * 3 / 2, output_border);
    }

    delete[] picture;
    fclose(input_file);
    fclose(output_border);
    return true;
}

bool YuvParser::yuv420_graybar(int width, int height, int y_min, int y_max, int bar_num)
{
    FILE *output_graybar = fopen("output_420_graybar.yuv", "wb+");

    if (bar_num == 1 && y_max != y_min) {
        return false;
    }

    unsigned char *picture = new unsigned char[width * height * 3 / 2];

    float luma_range = (float)(y_max - y_min) / (float)(bar_num > 1 ? bar_num - 1 : bar_num);
    unsigned char cur_luma = y_min;
    int cur_block = 0;
    int bar_width = width / bar_num;

    // write Y
    for (int cur_height = 0; cur_height < height; cur_height++) {
        for (int cur_width = 0; cur_width < width; cur_width++) {
            cur_block = (cur_width / bar_width == bar_num) ? (bar_num - 1) : (cur_width / bar_width);
            cur_luma = y_min + (unsigned char)(cur_block * luma_range);
            picture[cur_height * width + cur_width] = cur_luma;
        }
    }

    // NOTE: write U and write V can use memset to set,
    //       write them separately to make them easier
    //       to understand
    
    // write U
    for (int cur_height = 0; cur_height < height / 2; cur_height++) {
        for (int cur_width = 0; cur_width < width / 2; cur_width++) {
            picture[height * width + cur_height * width / 2 + cur_width] = 128;
        }
    }

    // write V
    for (int cur_height = 0; cur_height < height / 2; cur_height++) {
        for (int cur_width = 0; cur_width < width / 2; cur_width++) {
            picture[height * width * 5 / 4 + cur_height * width / 2 + cur_width] = 128;
        }
    }

    fwrite(picture, 1, width * height * 3 / 2, output_graybar);

    delete[] picture;
    fclose(output_graybar);
    return true;
}

bool YuvParser::yuv420_psnr(const std::string input_url1, const std::string input_url2, int width, int height, int frame_num)
{
    FILE *input_file1 = fopen(input_url1.c_str(), "rb+");
    FILE *input_file2 = fopen(input_url2.c_str(), "rb+");

    unsigned char *picture1 = new unsigned char[width * height * 3 / 2];
    unsigned char *picture2 = new unsigned char[width * height * 3 / 2];

    for (int i = 0; i < frame_num; i++) {
        fread(picture1, 1, width * height * 3 / 2, input_file1);
        fread(picture2, 1, width * height * 3 / 2, input_file2);

        double mse_total = 0, mse = 0, psnr = 0;
        for (int cur_pixel = 0; cur_pixel < width * height; cur_pixel++) {
            mse_total += pow((double)(picture1[cur_pixel] - picture2[cur_pixel]), 2);
        }
        mse = mse_total / (width * height);
        psnr = 10 * log10(255.0 * 255.0 / mse);
        printf("frame_num=%d psnr=%5.3f\n", frame_num, psnr);

        // Skip the UV component
        fseek(input_file1, width * height / 2, SEEK_CUR);
        fseek(input_file2, width * height / 2, SEEK_CUR);
    }

    delete[] picture1;
    delete[] picture2;
    fclose(input_file1);
    fclose(input_file2);
    return true;
}
