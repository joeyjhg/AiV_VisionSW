#pragma once
#include "Process.h"

Process::Process()
{
    
    if (!initialize())
        return;
    if (!LoadDLLFunctions())
        return;
    if (!processImage())
        return;
    if (CompareImage())
        cout << "����";
    else
        cout << "�ٸ�";
    SaveImage();
}


//i��°�� ���� �о����
bool Process::initialize()
{
    img_path = readFile("setting.txt", 0);
    try 
    {
        kernel = std::stoi(readFile("setting.txt", 1));
    }
    catch (const invalid_argument& e) 
    {
        kernel = 0;
    }
    catch (const out_of_range& e) 
    {
        kernel = 0;
    }
    resultimg_path = readFile("setting.txt", 2);
    if (img_path == "" || kernel == 0 || resultimg_path == "")
        return false;
    return true;
}

//dll Load
bool Process::LoadDLLFunctions() {
    //dll load
    hModule_OpenCV = LoadLibrary(TEXT("Dll_OpenCV.dll"));
    hModule_Custom = LoadLibrary(TEXT("Dll_CustomBlur.dll"));
    //dll load �����Ѵٸ�
    if (!hModule_OpenCV || !hModule_Custom) 
    {
        return false;
        writeFile("Dll Load Fail"); //log
    }
    //dll �Լ� load
    ImgBlur_OpenCV = (LPImgBlur)GetProcAddress(hModule_OpenCV, "ImgBlur");
    ImgBlur_Custom = (LPImgBlur)GetProcAddress(hModule_Custom, "ImgBlur_Custom");
    //dll �Լ� load �����Ѵٸ�
    if (!ImgBlur_OpenCV || !ImgBlur_Custom) 
    {
        FreeLibrary(hModule_OpenCV);
        FreeLibrary(hModule_Custom);
        writeFile("Dll Load Fail"); //log
        return false;
    }
    //dll ��ü load ����
    writeFile("Dll Load Success");  //log
    return true;
}

//�̹��� �¾�
bool Process::processImage() {
    Mat img = imread(img_path, IMREAD_GRAYSCALE);   //img_path�� �ִ� �̹����� GRAYSCALE�� �о����
    //�̹����� ����ų�, ����� �ƴҰ��
    if (img.empty() || img.channels() != 1)
    {
        writeFile("Image Load Fail");   //log
        return false;
    }

    vector<uchar> data(img.datastart, img.dataend); //�ȼ����� �����ϴ� data
    imageobject_Origin.set_image_object(data, img.cols, img.rows);  //imageobject_Origin�� �ȼ���, ũ�� ������ ����

    //imageobject_OpenCV,imageobject_Custom�� Bulr ó���� �ȼ����� ũ�������� ����
    ImgBlur_OpenCV(imageobject_Origin.getBuffer(), imageobject_Origin.getWidth(), imageobject_Origin.getHeight(), kernel, imageobject_OpenCV.setBuffer(imageobject_Origin.getWidth(), imageobject_Origin.getHeight()));
    ImgBlur_Custom(imageobject_Origin.getBuffer(), imageobject_Origin.getWidth(), imageobject_Origin.getHeight(), kernel, imageobject_Custom.setBuffer(imageobject_Origin.getWidth(), imageobject_Origin.getHeight()));

    return true;
}
//�̹��� ��
bool Process::CompareImage()
{
    //�� �ȼ����� ���� unsigned char*�� ���� ������ �Ҵ�
    const unsigned char* img_OpenCV = imageobject_OpenCV.getBuffer();
    const unsigned char* img_Custom = imageobject_Custom.getBuffer();

    int width_OpenCV = imageobject_OpenCV.getWidth();
    int height_OpenCV = imageobject_OpenCV.getHeight();
    int width_Custom = imageobject_Custom.getWidth();
    int height_Custom = imageobject_Custom.getHeight();

    //�̶� unsigned char*���� 1���� �迭�̹Ƿ� [n][0]�� [n-1][0]���� height��ŭ ����������.
    for (int i = 0; i < imageobject_OpenCV.getHeight(); ++i) 
    {
        for (int j = 0; j < imageobject_OpenCV.getWidth(); ++j) 
        {
            int idx = i * imageobject_OpenCV.getWidth() + j;    //idx���� i*height + j�� �����Ͽ� ���ʷ� ��
            if (img_OpenCV[idx] != img_Custom[idx]) 
            {
                writeFile("The gray values of the image pixels are different.");    //log
                return false;
            }
        }
    }
    writeFile("The gray values of the image pixels are identical.");    //log
    return true;
}

//�̹��� ����
void Process::SaveImage()
{
    //imageobject���� Mat�� ȣȯ�Ǵ� �Ű������� ����
    Mat blurredImg_OpenCV(imageobject_OpenCV.getHeight(), imageobject_OpenCV.getWidth(), CV_8UC1, const_cast<uchar*>(imageobject_OpenCV.getBuffer()));
    Mat blurredImg_Custom(imageobject_Custom.getHeight(), imageobject_Custom.getWidth(), CV_8UC1, const_cast<uchar*>(imageobject_Custom.getBuffer()));

    string time_str = returnTime();

    //��� �̹��� ��ο� ��¥�� �ð��� �߰�
    string resultimg_path_OpenCV = "." + resultimg_path + "/blurred_OpenCV_" + time_str + ".png";
    string resultimg_path_Custom = "." + resultimg_path + "/blurred_Custom_" + time_str + ".png";

    //���� ����
    imwrite(resultimg_path_OpenCV, blurredImg_OpenCV);
    imwrite(resultimg_path_Custom, blurredImg_Custom);
    writeFile("Save Image_"+ resultimg_path_OpenCV);    //log
    writeFile("Save Image_"+ resultimg_path_Custom);    //log

    //�̹��� ���
    Mat Origin(imageobject_Origin.getHeight(), imageobject_Origin.getWidth(), CV_8UC1, const_cast<uchar*>(imageobject_Origin.getBuffer()));
    imshow("origin", Origin);
    imshow("blurredImg_OpenCV", blurredImg_OpenCV);
    imshow("blurredImg_Custom", blurredImg_Custom);
    waitKey(0);
}

//i��°�� ���� �о����
string Process::readFile(const string& filePath, int i) 
{
    ifstream file(filePath);
    if (!file.is_open()) 
        return "";
    
    string line;
    int currentLine = 0;
    while (getline(file, line)) 
    {
        if (currentLine == i) 
        {
            size_t pos = line.find('=');
            if (pos != string::npos) 
                return line.substr(pos + 1);
            else 
                return "";
        }
        currentLine++;
    }
    return "";
}

void Process::writeFile(const string& log)
{
    string line = returnTime();
    ofstream file("image_blur.log", ios::app);
    if (file.is_open())
    {
        file << line << "_" << log + "\n";
        file.close();
    }
    else
        return;
}

string Process::returnTime()
{
    // ���� ��¥�� �ð��� ���ڿ��� ��ȯ
    time_t timer = time(nullptr);
    struct tm* t = localtime(&timer);
    ostringstream oss;
    oss << put_time(t, "%Y%m%d_%H%M%S");
    return oss.str();
}