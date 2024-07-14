#pragma once
#include "image_object.h"

//������. ������� �ʱ�ȭ
image_object::image_object()	
{
	buffer = nullptr;
	width = 0;
	height = 0;
}
//�Ҹ���. �޸� ����
image_object::~image_object()	
{
	if (buffer)
		delete[] buffer;
}
//�ȼ���, ���� ũ�� ������ �޾� �¾�
void image_object::set_image_object(const vector<uchar>& data, int width, int height)	
{
	if (buffer) 
		delete[] buffer;
	
	this->width = width;
	this->height = height;
	buffer = new unsigned char[width * height];
	memcpy(buffer, data.data(), width * height);
}
//width ��ȯ
int image_object::getWidth()	
{
	return width;
}
//height ��ȯ
int image_object::getHeight()	
{
	return height;
}
//buffer ��ȯ
const unsigned char* image_object::getBuffer() const	
{
	return buffer;
}
//���� ũ�⸦ �޾� ���� ũ�⿡ �´� ���� ��ȯ
unsigned char** image_object::setBuffer(int width, int height) 
{
	if (buffer)
		delete[] buffer;
	
	this->width = width;
	this->height = height;
	buffer = new unsigned char[width * height];
	return &buffer;
}


