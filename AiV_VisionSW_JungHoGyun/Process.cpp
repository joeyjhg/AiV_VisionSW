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
        cout << "같음";
    else
        cout << "다름";
    SaveImage();
}


//i번째의 줄을 읽어들임
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
    //dll load 실패한다면
    if (!hModule_OpenCV || !hModule_Custom) 
    {
        return false;
        writeFile("Dll Load Fail"); //log
    }
    //dll 함수 load
    ImgBlur_OpenCV = (LPImgBlur)GetProcAddress(hModule_OpenCV, "ImgBlur");
    ImgBlur_Custom = (LPImgBlur)GetProcAddress(hModule_Custom, "ImgBlur_Custom");
    //dll 함수 load 실패한다면
    if (!ImgBlur_OpenCV || !ImgBlur_Custom) 
    {
        FreeLibrary(hModule_OpenCV);
        FreeLibrary(hModule_Custom);
        writeFile("Dll Load Fail"); //log
        return false;
    }
    //dll 전체 load 성공
    writeFile("Dll Load Success");  //log
    return true;
}

//이미지 셋업
bool Process::processImage() {
    Mat img = imread(img_path, IMREAD_GRAYSCALE);   //img_path에 있는 이미지를 GRAYSCALE로 읽어들임
    //이미지가 비었거나, 흑백이 아닐경우
    if (img.empty() || img.channels() != 1)
    {
        writeFile("Image Load Fail");   //log
        return false;
    }

    vector<uchar> data(img.datastart, img.dataend); //픽셀값을 저장하는 data
    imageobject_Origin.set_image_object(data, img.cols, img.rows);  //imageobject_Origin에 픽셀값, 크기 정보를 담음

    //imageobject_OpenCV,imageobject_Custom에 Bulr 처리한 픽셀값과 크기정보를 담음
    ImgBlur_OpenCV(imageobject_Origin.getBuffer(), imageobject_Origin.getWidth(), imageobject_Origin.getHeight(), kernel, imageobject_OpenCV.setBuffer(imageobject_Origin.getWidth(), imageobject_Origin.getHeight()));
    ImgBlur_Custom(imageobject_Origin.getBuffer(), imageobject_Origin.getWidth(), imageobject_Origin.getHeight(), kernel, imageobject_Custom.setBuffer(imageobject_Origin.getWidth(), imageobject_Origin.getHeight()));

    return true;
}
//이미지 비교
bool Process::CompareImage()
{
    //각 픽셀값을 비교할 unsigned char*형 변수 선언후 할당
    const unsigned char* img_OpenCV = imageobject_OpenCV.getBuffer();
    const unsigned char* img_Custom = imageobject_Custom.getBuffer();

    int width_OpenCV = imageobject_OpenCV.getWidth();
    int height_OpenCV = imageobject_OpenCV.getHeight();
    int width_Custom = imageobject_Custom.getWidth();
    int height_Custom = imageobject_Custom.getHeight();

    //이때 unsigned char*형은 1차원 배열이므로 [n][0]은 [n-1][0]보다 height만큼 떨어져있음.
    for (int i = 0; i < imageobject_OpenCV.getHeight(); ++i) 
    {
        for (int j = 0; j < imageobject_OpenCV.getWidth(); ++j) 
        {
            int idx = i * imageobject_OpenCV.getWidth() + j;    //idx값을 i*height + j로 설정하여 차례로 비교
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

//이미지 저장
void Process::SaveImage()
{
    //imageobject에서 Mat과 호환되는 매개변수들 리턴
    Mat blurredImg_OpenCV(imageobject_OpenCV.getHeight(), imageobject_OpenCV.getWidth(), CV_8UC1, const_cast<uchar*>(imageobject_OpenCV.getBuffer()));
    Mat blurredImg_Custom(imageobject_Custom.getHeight(), imageobject_Custom.getWidth(), CV_8UC1, const_cast<uchar*>(imageobject_Custom.getBuffer()));

    string time_str = returnTime();

    //결과 이미지 경로에 날짜와 시간을 추가
    string resultimg_path_OpenCV = "." + resultimg_path + "/blurred_OpenCV_" + time_str + ".png";
    string resultimg_path_Custom = "." + resultimg_path + "/blurred_Custom_" + time_str + ".png";

    //파일 저장
    imwrite(resultimg_path_OpenCV, blurredImg_OpenCV);
    imwrite(resultimg_path_Custom, blurredImg_Custom);
    writeFile("Save Image_"+ resultimg_path_OpenCV);    //log
    writeFile("Save Image_"+ resultimg_path_Custom);    //log

    //이미지 출력
    Mat Origin(imageobject_Origin.getHeight(), imageobject_Origin.getWidth(), CV_8UC1, const_cast<uchar*>(imageobject_Origin.getBuffer()));
    imshow("origin", Origin);
    imshow("blurredImg_OpenCV", blurredImg_OpenCV);
    imshow("blurredImg_Custom", blurredImg_Custom);
    waitKey(0);
}

//i번째의 줄을 읽어들임
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
    // 현재 날짜와 시간을 문자열로 변환
    time_t timer = time(nullptr);
    struct tm* t = localtime(&timer);
    ostringstream oss;
    oss << put_time(t, "%Y%m%d_%H%M%S");
    return oss.str();
}