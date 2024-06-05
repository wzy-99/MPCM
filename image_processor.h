#ifndef MPCM_H
#define MPCM_H

// 包含 OpenCV 头文件
#include <opencv2/core/core.hpp>

// 函数声明
unsigned int* process_image(unsigned char* src, int width, int height, int scale, int lambda);

#endif // MPCM_H