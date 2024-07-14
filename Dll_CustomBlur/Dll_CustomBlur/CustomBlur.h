#pragma once
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;

extern "C"
{
	// Bulró�� �Լ�
	__declspec(dllexport) void ImgBlur_Custom(const unsigned char* imageData, int width, int height, int kernel, unsigned char** outData);
}