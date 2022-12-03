#ifndef BMP_H
#define BMP_H
#include <stdlib.h>
#include <sys/types.h>
#pragma pack(1)
struct bmp_header{
	u_int16_t type;        //必为'BM'
	u_int32_t  size;        //文件字节数(2-5)
	u_int32_t  reserved;    //位图文件保留字，必为0(6-9)
	u_int32_t off_bits;   //像素数据偏移 (10-13)
	u_int32_t    struct_size;          // 结构体尺寸 (14-17)
	u_int32_t   image_width;         // 图像宽度  (18-21)
	u_int32_t    image_height;        // 图像高度  (22-25)
	u_int16_t  planes;        // 目标设备的级别，为1(26-27)
	u_int16_t  bit_count;      // 像素位数，为1、4、8或24(28-29)
	u_int32_t    compression;   // 位图压缩类型，0为不压缩、1为BI_RLE8、2为BI_RLE4(30-33)
	u_int32_t    size_image;     // 单像素数据大小,等于bfSize-bfOffBits (34-37)
	u_int32_t   x_pels_per_meter; // 水平分辨率，一般为0 (38-41)
	u_int32_t    y_pels_per_meter; // 垂直分辨率，一般为0 (42-45)
	u_int32_t    clr_used;       // 位图颜色表中的颜色数，0表示使用所有调色板项(46-49)
	u_int32_t    clr_important;  // 重要颜色索引的数目，0表示都重要(50-53)
};
struct pixel
{
	u_int8_t blue;
	u_int8_t green;
	u_int8_t red;
};
#pragma pack()
#endif
