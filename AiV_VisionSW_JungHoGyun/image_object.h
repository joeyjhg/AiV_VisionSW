#pragma once
#include "Common.h"
class image_object //구현한 image object는 OpenCV library의 cv::Mat 과 호환이 되어야 한다.
{
public:
	image_object();	//생성자. 멤버변수 초기화
	~image_object();//소멸자. 메모리 해제

	void set_image_object(const vector<uchar>& data, int width, int height);	//픽셀값, 영상 크기 정보를 받아 셋업
	int getWidth();																//width 반환
	int getHeight();															//height 반환
	const unsigned char* getBuffer() const;										//buffer 반환
	unsigned char** setBuffer(int width, int height);							//영상 크기를 받아 영상 크기에 맞는 버퍼 반환
private:
	unsigned char* buffer;	//pixel 값이 포함된 buffer
	int width;				//영상 크기(width)
	int height;				//영상 크기(height)
};

