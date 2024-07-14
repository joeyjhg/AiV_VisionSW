#pragma once
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

extern "C"
{
	// OpenCV��� Bulró�� �Լ�
	__declspec(dllexport) void ImgBlur(const unsigned char* imageData, int width, int height, int kernel, unsigned char** outData);
}

