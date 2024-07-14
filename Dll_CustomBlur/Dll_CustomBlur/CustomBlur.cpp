#include "pch.h"
#include "CustomBlur.h"
#define EXPORTDLL extern "C" __declspec(dllexport)

void ImgBlur_Custom(const unsigned char* imageData, int width, int height, int kernel, unsigned char** outData)
{
    //커널이 짝수거나 1보다 작다면
    if (kernel % 2 == 0 || kernel < 1)
        return;

    //이미지 크기 계산 - default 값인 BORDER_REFLECT_101을 계산하기 위해 (kernel/2)*2 만큼 가로, 세로길이 늘림
    int halfKernel = kernel / 2;
    int paddedWidth = width + 2 * halfKernel;
    int paddedHeight = height + 2 * halfKernel;

    //패딩된 데이터 할당
    vector<vector<unsigned char>> paddedData(paddedHeight, vector<unsigned char>(paddedWidth, 0));

    //원본 데이터를 중앙에 복사
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            paddedData[i + halfKernel][j + halfKernel] = imageData[i * width + j];
    

    //경계 처리 - BORDER_REFLECT_101은 경계값이 안쪽에 있는 값과 대칭되도록 해야함
    for (int i = 0; i < halfKernel; ++i)
        for (int j = 0; j < width; ++j) 
        {
            paddedData[i][j + halfKernel] = imageData[(halfKernel - i) * width + j];  //상단 패딩
            paddedData[height + halfKernel + i][j + halfKernel] = imageData[(height - 2 - i) * width + j];  //하단 패딩
        }
 

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < halfKernel; ++j) 
        {
            paddedData[i + halfKernel][j] = imageData[i * width + (halfKernel - j)];  //좌측 패딩
            paddedData[i + halfKernel][width + halfKernel + j] = imageData[i * width + (width - 2 - j)];  //우측 패딩
        }

    for (int i = 0; i < halfKernel; ++i)
        for (int j = 0; j < halfKernel; ++j) 
        {
            paddedData[i][j] = imageData[halfKernel * width + halfKernel - j];  //좌상단
            paddedData[i][width + halfKernel + j] = imageData[halfKernel * width + width - 2 - j];  //우상단
            paddedData[height + halfKernel + i][j] = imageData[(height - 2 - i) * width + halfKernel - j];  //좌하단
            paddedData[height + halfKernel + i][width + halfKernel + j] = imageData[(height - 2 - i) * width + width - 2 - j];  //우하단
        }

    //블러 처리된 데이터를 위한 메모리 할당
    *outData = new unsigned char[width * height];

    //블러 처리
    for (int i = halfKernel; i < height + halfKernel; ++i)
        for (int j = halfKernel; j < width + halfKernel; ++j) 
        {
            float sum = 0.0f;   //패딩한 값을 모두 더하는 sum 변수 선언
            for (int ki = -halfKernel; ki <= halfKernel; ++ki)
                for (int kj = -halfKernel; kj <= halfKernel; ++kj)
                    sum += paddedData[i + ki][j + kj];  //패딩한 값을 모두 더함
            sum /= (kernel * kernel);   //그 값을 kernel의 제곱, 즉 모두 더한 숫자만큼 나눔
            (*outData)[(i - halfKernel) * width + (j - halfKernel)] = static_cast<unsigned char>(round(sum)); //outData에 반올림한 sum값을 저장
        }
}

