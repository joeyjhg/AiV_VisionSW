#pragma once
#include "Common.h"
#include "image_object.h"
typedef void(__cdecl* LPImgBlur)(const unsigned char*, int, int, int, unsigned char**);

class Process
{
public:
	Process();	//생성자, 프로세스 시작

	string readFile(const string& filePath, int i);	//i번째의 줄을 읽어들임
	void writeFile(const string& log);				//image_blur.log로 log를 남김
	string returnTime();							//현재시간 리턴

private:
	string img_path;		//이미지 경로
	int kernel;				//커널 수
	string resultimg_path;	//결과 이미지 저장 경로

	image_object imageobject_Origin;	//imageobject 원본
	image_object imageobject_OpenCV;	//imageobject OpenCV Bulr 처리	
	image_object imageobject_Custom;	//imageobject Custom Bulr 처리

	//dll 파일
	LPImgBlur ImgBlur_OpenCV;
	LPImgBlur ImgBlur_Custom;
	HMODULE hModule_OpenCV;
	HMODULE hModule_Custom;

	bool initialize();			//초기화. 프로그램 실행 시 설정 파일 정보 읽기
	bool LoadDLLFunctions();	//dll Load
	bool processImage();		//이미지 셋업
	bool CompareImage();		//이미지 비교
	void SaveImage();			//이미지 저장

};

