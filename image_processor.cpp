#define _CRT_SECURE_NO_WARNINGS
#include "MPCM.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <opencv2/core/core.hpp> 

using namespace std;
using namespace cv;


Mat my_conv3(Mat& img, int height, int width) {
    Mat res = Mat::zeros(height, width, CV_8UC1);
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int tem = 0;
            for (int row = -1; row <= 1; row++) {
                for (int col = -1; col <= 1; col++) {
                    tem += (int)img.ptr(i + row)[j + col] / 9;
                }
            }
            res.ptr(i)[j] = tem;
        }
    }
    return res;
}



unsigned int* process_image(unsigned char* src, int width, int height, int scale, int lambda) {
    mpcm* mpcm_Solution = new mpcm(height, width);
    mpcm_Solution->kernelScales = scale;
    mpcm_Solution->lamda = lambda;

    mpcm_Solution->GetIntgImg(src, mpcm_Solution->Intergral, mpcm_Solution->Colsum, mpcm_Solution->width, mpcm_Solution->height);

    int kernel = mpcm_Solution->kernelScales;
    string kernels = to_string(kernel);
    int neighbor[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int dir[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    int start = kernel + kernel / 2;

    unsigned int res;
    for (int y = start; y < mpcm_Solution->height - start + 1; y++) {
        for (int x = start; x < mpcm_Solution->width - start + 1; x++) {
            mpcm_Solution->Get_9_Near_Blobs(mpcm_Solution->Intergral, neighbor, mpcm_Solution->width, mpcm_Solution->height, x, y, kernel);
            res = mpcm_Solution->res_Perpoint(neighbor, dir, y, x);
            if (res > mpcm_Solution->resp[y * mpcm_Solution->width + x]) {
                mpcm_Solution->resp[y * mpcm_Solution->width + x] = res;
            }
        }
    }

    return mpcm_Solution->resp;
}
