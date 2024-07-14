#pragma once
#include "Common.h"
class image_object //������ image object�� OpenCV library�� cv::Mat �� ȣȯ�� �Ǿ�� �Ѵ�.
{
public:
	image_object();	//������. ������� �ʱ�ȭ
	~image_object();//�Ҹ���. �޸� ����

	void set_image_object(const vector<uchar>& data, int width, int height);	//�ȼ���, ���� ũ�� ������ �޾� �¾�
	int getWidth();																//width ��ȯ
	int getHeight();															//height ��ȯ
	const unsigned char* getBuffer() const;										//buffer ��ȯ
	unsigned char** setBuffer(int width, int height);							//���� ũ�⸦ �޾� ���� ũ�⿡ �´� ���� ��ȯ
private:
	unsigned char* buffer;	//pixel ���� ���Ե� buffer
	int width;				//���� ũ��(width)
	int height;				//���� ũ��(height)
};

