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
#include "utils.h"
using namespace cv;
int main(int argc, char* argv[]) 
{
    Mat polarImg;
    Mat rgbImg;
    polarImg = imread("D:\\OneDrive\\OneDrive - mails.tsinghua.edu.cn\\桌面\\偏振相机\\test\\polar_MONO_12bit_Angle_001_Sum.tif",-1);
    imshow("polarImg", polarImg);
    //std::cout << polarImg << std::endl;
    Mat polarMat;

    polarMat = CV_16UC1ToCV_8UC1(polarImg); //转换格式

    rgbImg = imread("D:\\OneDrive\\OneDrive - mails.tsinghua.edu.cn\\桌面\\偏振相机\\test\\Rgb_Color.png", -1);
    Registrar test;
    test.CalculateTransform(polarMat,rgbImg);
    test.Process(polarImg);
    imshow("rgbImg", rgbImg);
    //配准后的图像
    imshow("regiteredPolar", test.registered_poarImg);
    imshow("polarPoints", test.polarCorners);
    imshow("rgbPoints", test.rgbCorners);
    
    //亮化图像
    Mat bright_polar;
    polarImg.convertTo(bright_polar,CV_8UC1);
    imshow("Bright Polar", bright_polar);
    test.Process(bright_polar);
    imshow("Bright regiteredPolar", test.registered_poarImg);



    std::cout << test.transformMat << std::endl;
    waitKey(0);


    return 0;
}
