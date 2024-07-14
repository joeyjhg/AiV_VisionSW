#include "pch.h"
#include "Bulr.h"
#define EXPORTDLL extern "C" __declspec(dllexport)

void ImgBlur(const unsigned char* imageData, int width, int height, int kernel, unsigned char** outData)
{
    //Ŀ���� ¦���ų� 1���� �۴ٸ�
    if (kernel % 2 == 0 || kernel < 1)
        return;
    //�̹��� ������ �޾ƿ���
	Mat imgMat(height, width, CV_8UC1, const_cast<uchar*>(imageData));

    if (imgMat.empty())
        return;
 
    Mat blurred;
    //OpenCV blur�Լ� �̿�
    blur(imgMat, blurred, Size(kernel, kernel));

    int imageSize = blurred.cols * blurred.rows * blurred.channels();
    *outData = new unsigned char[imageSize];

    //��ȯ
    memcpy(*outData, blurred.data, imageSize);
}
