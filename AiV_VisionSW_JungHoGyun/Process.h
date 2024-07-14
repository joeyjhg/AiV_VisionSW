#pragma once
#include "Common.h"
#include "image_object.h"
typedef void(__cdecl* LPImgBlur)(const unsigned char*, int, int, int, unsigned char**);

class Process
{
public:
	Process();	//������, ���μ��� ����

	string readFile(const string& filePath, int i);	//i��°�� ���� �о����
	void writeFile(const string& log);				//image_blur.log�� log�� ����
	string returnTime();							//����ð� ����

private:
	string img_path;		//�̹��� ���
	int kernel;				//Ŀ�� ��
	string resultimg_path;	//��� �̹��� ���� ���

	image_object imageobject_Origin;	//imageobject ����
	image_object imageobject_OpenCV;	//imageobject OpenCV Bulr ó��	
	image_object imageobject_Custom;	//imageobject Custom Bulr ó��

	//dll ����
	LPImgBlur ImgBlur_OpenCV;
	LPImgBlur ImgBlur_Custom;
	HMODULE hModule_OpenCV;
	HMODULE hModule_Custom;

	bool initialize();			//�ʱ�ȭ. ���α׷� ���� �� ���� ���� ���� �б�
	bool LoadDLLFunctions();	//dll Load
	bool processImage();		//�̹��� �¾�
	bool CompareImage();		//�̹��� ��
	void SaveImage();			//�̹��� ����

};

