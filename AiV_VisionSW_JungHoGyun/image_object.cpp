#pragma once
#include "image_object.h"

//생성자. 멤버변수 초기화
image_object::image_object()	
{
	buffer = nullptr;
	width = 0;
	height = 0;
}
//소멸자. 메모리 해제
image_object::~image_object()	
{
	if (buffer)
		delete[] buffer;
}
//픽셀값, 영상 크기 정보를 받아 셋업
void image_object::set_image_object(const vector<uchar>& data, int width, int height)	
{
	if (buffer) 
		delete[] buffer;
	
	this->width = width;
	this->height = height;
	buffer = new unsigned char[width * height];
	memcpy(buffer, data.data(), width * height);
}
//width 반환
int image_object::getWidth()	
{
	return width;
}
//height 반환
int image_object::getHeight()	
{
	return height;
}
//buffer 반환
const unsigned char* image_object::getBuffer() const	
{
	return buffer;
}
//영상 크기를 받아 영상 크기에 맞는 버퍼 반환
unsigned char** image_object::setBuffer(int width, int height) 
{
	if (buffer)
		delete[] buffer;
	
	this->width = width;
	this->height = height;
	buffer = new unsigned char[width * height];
	return &buffer;
}


