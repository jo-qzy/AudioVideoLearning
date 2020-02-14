### 8. 分离RGB24像素数据中的R、G、B分量

```c++
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
```

分离后查看图片用yuvplayer查看，选择500*500，Y分量查看。

RGB24的每个像素三个分量都是连续存储的，先存储第一个像素的R、G、B，再存第二个R、G、B，所以宽（width）和高（height）的RGB图片大小为$width * height * 3 Byte$，这种存储方式成为Packed方式。

R、G、B三张图分辨率如下（此处有疑问。。没搞懂）

- output_rgb_r.y：R数据，分辨率256 *256
- output_rgb_g.y：G数据，分辨率256 *256
- output_rgb_b.y：B数据，分辨率256 *256

原图为雷神提供的CIE 1931标准图：

<center>
	<img src="./pic/YUV/cie1931_500x500.png" width="300">
	<center>cie1931_500x500.rgb</center>
</center>

效果图如下：

<center>
	<img src="./pic/YUV/output_rgb_r.png" width="300">
	<center>output_rgb_r.y</center>
</center>

<center>
	<img src="./pic/YUV/output_rgb_g.png" width="300">
	<center>output_rgb_g.y</center>
</center>

<center>
	<img src="./pic/YUV/output_rgb_b.png" width="300">
	<center>output_rgb_b.y</center>
</center>

### 9. 将RGB格式像素数据封装为BMP图像

将RGB转换为BMP图像，就可以使用Windows自带的图片查看器查看，对于BMP来说，如果是未压缩的格式，那么除了头部信息外，剩下的数据和RGB类似，只不过是以B、G、R的顺序存储，所以只需要加上头并且略加修改即可。

首先了解下BMP的格式:

#### BMP_FILE_HEADER

BMP文件有文件头，在Windows中定义如下：

```c++
typedef struct tagBITMAPFILEHEADER
{
    UINT16 bfType;
    DWORD  bfSize;
    UINT16 bfReserved1;
    UINT16 bfReserved2;
    DWORD  bfOffBits;
} BITMAPFILEHEADER;
```

|    变量名    | 地址偏移 |  大小  |                    作用                    |
| :----------: | :------: | :----: | :----------------------------------------: |
|    bfType    | 0x0000H  | 2 Byte | 一般为'BM'，在小端的机器时要注意字节序问题 |
|    bfSize    | 0x0002H  | 4 Byte |         位图文件的大小，字节为单位         |
| bfReserverd1 | 0x0006H  | 2 Byte |             保留，必须设置为0              |
| bfReserverd2 | 0x0008H  | 2 Byte |             保留，必须设置为0              |
| bfbfOffBits  | 0x000AH  | 4 Byte |   说明从文件头开始到实际数据之间的偏移量   |

#### BMP_INFO_HEADER

```c++
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;
```

|     变量名      | 地址偏移 |  大小  |                             作用                             |
| :-------------: | :------: | :----: | :----------------------------------------------------------: |
|     biSize      | 0x000EH  | 4 Byte |                BITMAPINFOHEADER结构需要的字数                |
|     biWidth     | 0x0012H  | 4 Byte |                 说明图像的宽度，以像素为单位                 |
|    biHeight     | 0x0016H  | 4 Byte | 说明图像的高度，以像素为单位。<br />若为正值，则图像为倒向；<br />若为负值，则图像为正向 |
|    biPlanes     | 0x001AH  | 2 Byte |              为目标设备说明颜色平面数，一般为1               |
|   biBitCount    | 0x001CH  | 2 Byte |          一像素需要的比特数，可以为1、4、8、16、24           |
|  biCompression  | 0x001EH  | 4 Byte |           说明图像数据压缩类型，最常用为0，不压缩            |
|   biSizeImage   | 0x0022H  | 4 Byte |                       说明图像字节大小                       |
| biXPelsPerMeter | 0x0026H  | 4 Byte |                 说明水平分辨率，可以设置为0                  |
| biYPelsPerMeter | 0x002AH  | 4 Byte |                 说明垂直分辨率，可以设置为0                  |
|    biClrUsed    | 0x002EH  | 4 Byte |         说明位图使用的颜色索引数，为0表示所有调色板          |
| biClrImportant  | 0x0032H  | 4 Byte |         对图像显示有影响的颜色索引数目，0表示都重要          |

#### color palette

代码中没有用到调色板，不过可以有。

调色板其实是一张映射表，标识颜色索引号与其代表的颜色的对应关系。它在文件中的布局就像一个二维数组`palette[N][4]`，其中N表示总的颜色索引数，每行的四个元素分别表示该索引对应的B、G、R和Alpha的值，每个分量占一个字节。如不设透明通道时，Alpha为0。

#### 代码

```c++
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
    bmp_file_header.bfSize = sizeof(bmp_file_header) + sizeof(bmp_info_header) + width * height * 3 ;
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
```

使用lena512的rgb24图片生成bmp效果如下：

<center>
	<img src="./pic/YUV/output_bmp.png" width="300">
	<center>output_bmp.bmp</center>
</center>

### 10. 将RGB24格式像素数据转换为YUV420P格式像素数据