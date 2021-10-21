// PolarNormalCollector.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "PolarImagesAcquistion.h"
using namespace cv;
int main()
{
    AcquirePolarImages AcqPolar;
    while (1)
    {
        AcqPolar.ReadPolarImages();
        AcqPolar.Processing();
        imshow("I_sum", AcqPolar.I_sum);
        imshow("I_0", AcqPolar.I_0);
        imshow("I_45", AcqPolar.I_45);
        imshow("I_90", AcqPolar.I_90);
        imshow("I_135", AcqPolar.I_135);
        imshow("S_0", AcqPolar.S_0);
        imshow("S_1", AcqPolar.S_1);
        imshow("S_2", AcqPolar.S_2);
        imshow("DoLP", AcqPolar.DoLP);
        imshow("AoLP", AcqPolar.AoLP);
        printf_s("%d %d %d %d",AcqPolar.I_0.at<UINT16>(500,500), AcqPolar.I_45.at<UINT16>(500, 500),AcqPolar.I_90.at<UINT16>(500, 500),AcqPolar.I_135.at<UINT16>(500, 500));
        printf_s("DoLP at (500,500): %f", AcqPolar.DoLP.at<float>(500, 500));
        printf_s("AoLP at (500,500): %f", AcqPolar.AoLP.at<float>(500, 500));

        waitKey(0);
    }

    std::cout << "Hello World!\n";
}

