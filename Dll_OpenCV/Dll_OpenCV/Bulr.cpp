#include "pch.h"
#include "Bulr.h"
#define EXPORTDLL extern "C" __declspec(dllexport)

void ImgBlur(const unsigned char* imageData, int width, int height, int kernel, unsigned char** outData)
{
    //커널이 짝수거나 1보다 작다면
    if (kernel % 2 == 0 || kernel < 1)
        return;
    //이미지 데이터 받아오기
	Mat imgMat(height, width, CV_8UC1, const_cast<uchar*>(imageData));

    if (imgMat.empty())
        return;
 
    Mat blurred;
    //OpenCV blur함수 이용
    blur(imgMat, blurred, Size(kernel, kernel));

    int imageSize = blurred.cols * blurred.rows * blurred.channels();
    *outData = new unsigned char[imageSize];

    //반환
    memcpy(*outData, blurred.data, imageSize);
}
