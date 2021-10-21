// PolarNormalCollector.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
int main()
{
    Mat img = imread("D:\\OneDrive\\OneDrive - mails.tsinghua.edu.cn\\桌面\\desk.png");
    imshow("Desk", img);
    waitKey(0);
    std::cout << "Hello World!\n";
}

