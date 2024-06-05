#include "MPCM.h"


void rangeCheck(Corner* point, int imgh, int imgw)
{

    if (point->y < 0) 		    point->y = 0;
    if (point->y >= imgh) 		point->y = imgh;
    if (point->x < 0)           point->x = 0;
    if (point->x >= imgw)       point->x = imgw;
 
}

mpcm::mpcm(int height, int width) {
    this->height = height;
    this->width = width;
    this->Intergral = new unsigned int[(this->height + 1) * (this->width + 1)];
    memset(Intergral, 0, (static_cast<unsigned long long>((this->height + 1) * (this->width + 1)) * sizeof(unsigned int)));
    this->Colsum = new unsigned int[this->width];
    memset(Colsum, 0, this->width * sizeof(unsigned int));
    this->resp = new unsigned int[this->height * this->width];
    memset(resp, 0, (static_cast<unsigned long long>(this->height * this->width) * sizeof(unsigned int)));
    this->contrast_map = new float[this->height * this->width];
    memset(contrast_map, 0, (static_cast<unsigned long long>(this->height * this->width) * sizeof(float)));
};
mpcm :: ~mpcm() {};

void mpcm::GetIntgImg(unsigned char* src, unsigned int* Integral, unsigned int* ColSum, int Width, int Height)
{
    int Y, X;
    unsigned char* LinePS;
    unsigned int* LinePD;

    for (Y = 0; Y < Height; Y++)
    {
        LinePS = src + Y * Width;
        LinePD = Integral + (Y + 1) * (Width + 1) + 1;
        LinePD[-1] = 0;
        for (X = 0; X < Width; X++)
        {
            ColSum[X] += (unsigned int)LinePS[X];
            LinePD[X] = LinePD[X - 1] + ColSum[X];
        }
    }
}

int mpcm::getBlobMean(unsigned int* integralImg, int imgw, int imgh, int posx, int posy, int blobSize)
{
    Corner lefttop = Corner(0, 0), righttop(0, 0), leftbottom(0, 0), rightbottom(0, 0);
    int halfScale, intw;
    int blobSum = 0;
    halfScale = blobSize / 2;
    lefttop.x = posx - halfScale;
    lefttop.y = posy - halfScale;

    righttop.x = posx + halfScale + 1;
    righttop.y = posy - halfScale;

    leftbottom.x = posx - halfScale;
    leftbottom.y = posy + halfScale + 1;

    rightbottom.x = posx + halfScale + 1;
    rightbottom.y = posy + halfScale + 1;

    rangeCheck(&leftbottom, imgh, imgw);
    rangeCheck(&lefttop, imgh, imgw);
    rangeCheck(&rightbottom, imgh, imgw);
    rangeCheck(&righttop, imgh, imgw);

    intw = imgw + 1;
    blobSum = integralImg[rightbottom.y * intw + rightbottom.x] - integralImg[righttop.y * intw + righttop.x] - \
        integralImg[leftbottom.y * intw + leftbottom.x] + integralImg[lefttop.y * intw + lefttop.x];

    return blobSum / (blobSize * blobSize);
}


void mpcm::Get_9_Near_Blobs(unsigned int* integralImg, int* neighbor, int width, int height, int x, int y, int blobSize)
{
    int m, n;
    int blob_x, blob_y;
    int blob = 0;
    int blobMean = 0;
    for (m = -1; m < 2; m++)
    {
        for (n = -1; n < 2; n++)
        {
            blob_x = x + n * blobSize;
            blob_y = y + m * blobSize;


            blobMean = getBlobMean(integralImg, width, height, blob_x, blob_y, blobSize);
            neighbor[blob++] = blobMean;
        }
    }
}
unsigned int mpcm::res_Perpoint(int* neighbor, int* d, int row, int col)
{
    d[0] = neighbor[4] - neighbor[0];
    d[1] = neighbor[4] - neighbor[1];
    d[2] = neighbor[4] - neighbor[2];
    d[3] = neighbor[4] - neighbor[3];
    d[4] = neighbor[4] - neighbor[5];
    d[5] = neighbor[4] - neighbor[6];
    d[6] = neighbor[4] - neighbor[7];
    d[7] = neighbor[4] - neighbor[8];

    int temp[4] = {0};
    float contra[4] = {0};
    for (int i = 0; i < 4; i++)
    {
        if ((d[i] > 0 && d[7 - i] > 0) || (d[i] < 0 && d[7 - i] < 0)) {
            temp[i] = d[i] * d[7 - i];
            contra[i] = d[i] < d[7 - i] ? (float)d[i] / d[7 - i] : (float)d[7 - i] / d[i];
        }
        else {
            temp[i] = 0;
        }
    }
    float min_contra = contra[0], max_contra = contra[0];
    for (int i = 1; i < 4; i++) {
        if (contra[i] > max_contra) max_contra = contra[i];
        else if (contra[i] < min_contra) min_contra = contra[i];
    }

    int min = temp[0];
    for (int i = 1; i < 4; i++) {
        if (temp[i] < min) {
            min = temp[i];
            min_contra = contra[i];
        }
    }
    this->contrast_map[row * this->width + col] =  min_contra;
    return min;
}

unsigned int  mpcm::SegThresh(unsigned int* resp, int w, int h, float lamda)
{
    unsigned long long  sumx, sumxx;
    int ave;
    int var;
    unsigned int thresh = 0;
    int n = 0;
    sumx = 0;
    sumxx = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++)
        {
            if (resp[i * w + j] != 0)  
            {
                sumx += resp[i * w + j];
                sumxx += resp[i * w + j] * resp[i * w + j];
                n++;
            }
        }
    }

    if (n == 0) n = 1;

    ave = sumx / n;
    var = sqrt((sumxx - sumx * sumx / n) / n);
    thresh = (unsigned int)ave + lamda * var;
    return thresh;
}

void mpcm::Revalue(unsigned int* resp, int h, int w, unsigned int mpcm_thresh)
{
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (resp[i * w + j] <= mpcm_thresh) {
                resp[i * w + j] = 0;
            }
            else {
                resp[i * w + j] = 255;
            }
        }
    }
}



