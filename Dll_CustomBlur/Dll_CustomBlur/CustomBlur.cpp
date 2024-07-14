#include "pch.h"
#include "CustomBlur.h"
#define EXPORTDLL extern "C" __declspec(dllexport)

void ImgBlur_Custom(const unsigned char* imageData, int width, int height, int kernel, unsigned char** outData)
{
    //Ŀ���� ¦���ų� 1���� �۴ٸ�
    if (kernel % 2 == 0 || kernel < 1)
        return;

    //�̹��� ũ�� ��� - default ���� BORDER_REFLECT_101�� ����ϱ� ���� (kernel/2)*2 ��ŭ ����, ���α��� �ø�
    int halfKernel = kernel / 2;
    int paddedWidth = width + 2 * halfKernel;
    int paddedHeight = height + 2 * halfKernel;

    //�е��� ������ �Ҵ�
    vector<vector<unsigned char>> paddedData(paddedHeight, vector<unsigned char>(paddedWidth, 0));

    //���� �����͸� �߾ӿ� ����
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            paddedData[i + halfKernel][j + halfKernel] = imageData[i * width + j];
    

    //��� ó�� - BORDER_REFLECT_101�� ��谪�� ���ʿ� �ִ� ���� ��Ī�ǵ��� �ؾ���
    for (int i = 0; i < halfKernel; ++i)
        for (int j = 0; j < width; ++j) 
        {
            paddedData[i][j + halfKernel] = imageData[(halfKernel - i) * width + j];  //��� �е�
            paddedData[height + halfKernel + i][j + halfKernel] = imageData[(height - 2 - i) * width + j];  //�ϴ� �е�
        }
 

    for (int i = 0; i < height; ++i)
        for (int j = 0; j < halfKernel; ++j) 
        {
            paddedData[i + halfKernel][j] = imageData[i * width + (halfKernel - j)];  //���� �е�
            paddedData[i + halfKernel][width + halfKernel + j] = imageData[i * width + (width - 2 - j)];  //���� �е�
        }

    for (int i = 0; i < halfKernel; ++i)
        for (int j = 0; j < halfKernel; ++j) 
        {
            paddedData[i][j] = imageData[halfKernel * width + halfKernel - j];  //�»��
            paddedData[i][width + halfKernel + j] = imageData[halfKernel * width + width - 2 - j];  //����
            paddedData[height + halfKernel + i][j] = imageData[(height - 2 - i) * width + halfKernel - j];  //���ϴ�
            paddedData[height + halfKernel + i][width + halfKernel + j] = imageData[(height - 2 - i) * width + width - 2 - j];  //���ϴ�
        }

    //�� ó���� �����͸� ���� �޸� �Ҵ�
    *outData = new unsigned char[width * height];

    //�� ó��
    for (int i = halfKernel; i < height + halfKernel; ++i)
        for (int j = halfKernel; j < width + halfKernel; ++j) 
        {
            float sum = 0.0f;   //�е��� ���� ��� ���ϴ� sum ���� ����
            for (int ki = -halfKernel; ki <= halfKernel; ++ki)
                for (int kj = -halfKernel; kj <= halfKernel; ++kj)
                    sum += paddedData[i + ki][j + kj];  //�е��� ���� ��� ����
            sum /= (kernel * kernel);   //�� ���� kernel�� ����, �� ��� ���� ���ڸ�ŭ ����
            (*outData)[(i - halfKernel) * width + (j - halfKernel)] = static_cast<unsigned char>(round(sum)); //outData�� �ݿø��� sum���� ����
        }
}

