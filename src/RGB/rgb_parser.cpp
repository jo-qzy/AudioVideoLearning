#include "rgb_parser.h"

bool RgbParser::rgb24_split(const std::string input_url, int width, int height, int frame_num)
{
    FILE *input_file = fopen(input_url.c_str(), "rb+");
    FILE *output_r = fopen("output_rgb_r.y", "wb+");
    FILE *output_g = fopen("output_rgb_g.y", "wb+");
    FILE *output_b = fopen("output_rgb_b.y", "wb+");

    unsigned char *picture = new unsigned char[width * height * 3];

    for (int i = 0; i < frame_num; i++) {
        fread(picture, 1, width * height * 3, input_file);
        for (int cur_pixel = 0; cur_pixel < width * height * 3; cur_pixel += 3) {
            // Read R
            fwrite(picture + cur_pixel, 1, 1, output_r);
            // Read G
            fwrite(picture + cur_pixel + 1, 1, 1, output_g);
            // Read B
            fwrite(picture + cur_pixel + 2, 1, 1, output_b);
        }
    }

    delete[] picture;
    fclose(input_file);
    fclose(output_r);
    fclose(output_g);
    fclose(output_b);
    return true;
}

bool RgbParser::rgb24_to_bmp(const std::string input_url, int width, int height)
{
    FILE *input_file = fopen(input_url.c_str(), "rb+");
    FILE *output_bmp = fopen("output_bmp.bmp", "wb+");

    unsigned char *picture = new unsigned char[width * height * 3];

    BMP::BIT_MAP_FILE_HEADER bmp_file_header;
    BMP::BIT_MAP_INFO_HEADER bmp_info_header;

    // Fill bit map file header
    // Note: the bfType is only useful in little endian machine.
    bmp_file_header.bfType = ((unsigned short int)('M') << 8) + 'B';
    bmp_file_header.bfSize = sizeof(bmp_file_header) + sizeof(bmp_info_header) + width * height * 3;
    bmp_file_header.bfReserverd1 = 0;
    bmp_file_header.bfReserverd2 = 0;
    bmp_file_header.bfbfOffBits = sizeof(bmp_file_header) + sizeof(bmp_info_header);

    // Fill bit map info header
    bmp_info_header.biSize = sizeof(bmp_info_header);
    bmp_info_header.biWidth = width;
    bmp_info_header.biHeight = -height;
    bmp_info_header.biPlanes = 1;        // Must be 1
    bmp_info_header.biBitcount = 24;     // RGB24 need 24 bit to description one pixel
    bmp_info_header.biCompression = 0;   // 0 means without compression
    bmp_info_header.biSizeImage = width * height * 3; // RGB24 can set it to 0
    bmp_info_header.biXPelsPermeter = 0; // Most time it set to 0
    bmp_info_header.biYPelsPermeter = 0; // Most time it set to 0
    bmp_info_header.biClrUsed = 0;       // 0 means use all palette
    bmp_info_header.biClrImportant = 0;  // 0 means all color is important

    unsigned char temp_val = 0;

    fread(picture, 1, width * height * 3, input_file);
    for (int cur_pixel = 0; cur_pixel < width * height * 3; cur_pixel += 3) {
        temp_val = picture[cur_pixel];
        picture[cur_pixel] = picture[cur_pixel + 2];
        picture[cur_pixel + 2] = temp_val;
    }

    fwrite(&bmp_file_header, 1, sizeof(bmp_file_header), output_bmp);
    fwrite(&bmp_info_header, 1, sizeof(bmp_info_header), output_bmp);
    fwrite(picture, 1, width * height * 3, output_bmp);

    delete[] picture;
    fclose(input_file);
    fclose(output_bmp);
    return true;
}

bool RgbParser::rgb24_to_yuv420p(const std::string input_url, int width, int height)
{
    FILE *input_file = fopen(input_url.c_str(), "rb+");
    FILE *output_yuv = fopen("output_rgb_to_yuv.yuv", "wb+");

    unsigned char *rgb_picture = new unsigned char[width * height * 3];
    unsigned char *yuv_picture = new unsigned char[width * height * 3 / 2];

    int cur_y = 0, cur_u = width * height, cur_v = width * height * 5 / 4;
    int cur_width = 0, cur_height = 0;
    unsigned char r = 0, g = 0, b = 0;

    fread(rgb_picture, 1, width * height * 3, input_file);
    for (int cur_pixel = 0; cur_pixel < width * height * 3; cur_pixel += 3) {
        r = rgb_picture[cur_pixel];
        g = rgb_picture[cur_pixel + 1];
        b = rgb_picture[cur_pixel + 2];
        yuv_picture[cur_y++] = (unsigned char)(0.299 * (double)r + 0.587 * (double)g + 0.114 * (double)b);

        cur_width = (cur_pixel / 3) % width;
        cur_height = (cur_pixel / 3) / width;
        // Every four
        if (cur_width % 2 == 0 && cur_height % 2 == 0) {
            yuv_picture[cur_u++] = (unsigned char)(-0.169 * (double)r - 0.331 * (double)g + 0.5 * (double)b + 128);
        }
        else {
            if (cur_width % 2 == 0) {
                yuv_picture[cur_v++] = (unsigned char)(0.5 * (double)r - 0.419 * (double)g - 0.081 * (double)b + 128);
            }
        }
    }

    fwrite(yuv_picture, 1, width * height * 3 / 2, output_yuv);

    delete[] rgb_picture;
    delete[] yuv_picture;
    fclose(input_file);
    fclose(output_yuv);
    return true;
}

bool RgbParser::rgb24_colorbar(int width, int height)
{
    FILE *output_rgb = fopen("output_colorbar.rgb", "wb+");
    unsigned char *picture = new unsigned char[width * height * 3];

    int width_range = width / 8;
    int cur_width_max = width_range;

    char color[8][3] = {
        { 255, 255, 255 }, { 255, 255, 0 }, { 0, 255, 255 },
        { 0, 255, 0 }, { 255, 0, 255 }, { 255, 0, 0 },
        { 0, 0, 255 }, { 0, 0, 0 }
    };

    for (int cur_width = 0; cur_width < width; cur_width++) {
        // NOTE: All the extra pixels in the end are in black.
        int cur_color_pos = cur_width / width_range == 8 ? 7 : cur_width / width_range;
        for (int cur_height = 0; cur_height < height; cur_height++) {
            int cur_pixel = width * cur_height * 3 + cur_width * 3;
            picture[cur_pixel] = color[cur_color_pos][0];
            picture[cur_pixel + 1] = color[cur_color_pos][1];
            picture[cur_pixel + 2] = color[cur_color_pos][2];
        }
    }

    fwrite(picture, 1, width * height * 3, output_rgb);

    delete[] picture;
    fclose(output_rgb);
    return true;
}
