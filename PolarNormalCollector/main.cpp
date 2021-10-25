// PolarNormalCollector.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "PolarImagesAcquistion.h"
#include <librealsense2/rs.hpp>
#include "RealSenseAcquisition.h"
#include "Registration.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
int main(int argc, char* argv[]) 
{
    Mat polarImg;
    Mat rgbImg;
    Mat polarMat;
    polarImg = imread("D:\\OneDrive\\OneDrive - mails.tsinghua.edu.cn\\桌面\\偏振相机\\test\\polar_MONO_12bit_Angle_001_Sum.tif",-1);
    polarImg.convertTo(polarMat,CV_8UC1);
    rgbImg = imread("D:\\OneDrive\\OneDrive - mails.tsinghua.edu.cn\\桌面\\偏振相机\\test\\Rgb_Color.png", -1);
    Registrar test;
    test.CalculateTransform(polarMat,rgbImg);
    
    imshow("polarPoints", polarMat);
    imshow("rgbPoints", test.rgbCorners);
    waitKey(0);


    return 0;
}
