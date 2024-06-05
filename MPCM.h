#pragma once
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp> 
using namespace std;

class Corner {
public:
    int x;
    int y;
    Corner(int x , int y) {
        this->x = x;
        this->y = y;
    };
    ~Corner() {};
};

class mpcm {
public:

    int height;
    int width;
    int kernelScales;
    int lamda;

    unsigned int* Intergral;
    unsigned int* Colsum;
    unsigned int* resp;
    float* contrast_map;

    mpcm(int height, int width);
    ~mpcm();
    void GetIntgImg(unsigned char* src, unsigned int* Integral, unsigned int* ColSum, int Width, int Height);

    int getBlobMean(unsigned int* integralImg, int imgw, int imgh, int posx, int posy, int blobSize);

    void Get_9_Near_Blobs(unsigned int* integralImg, int* neighbor, int width, int height, int x, int y, int blobSize);

    unsigned int res_Perpoint(int* neighbor, int* d, int row, int col);

    unsigned int SegThresh(unsigned int* resp, int w, int h, float lamda);

    void Revalue(unsigned int* resp, int h, int w, unsigned int mpcm_thresh);
};

