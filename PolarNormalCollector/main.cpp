// PolarNormalCollector.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "PolarImagesAcquistion.h"
#include <librealsense2/rs.hpp>
#include "RealSenseAcquisition.h"
using namespace cv;
int main(int argc, char* argv[]) 
{
    AcquireRealSense captcher;
    std::thread processing_thread([&]() 
        {
         
                captcher.Processing();
            
        });
    
    while (1)
    {
        captcher.GetPictures();
        imshow("raw_depth", captcher.color_depth);
        imshow("raw_rgb", captcher.raw_rgb_mat);
        Mat syn_img = Mat::zeros(640,480, CV_8UC3);
        addWeighted(captcher.color_depth, 0.5, captcher.raw_rgb_mat, 0.5, 0.0, syn_img);

        imshow("syn", syn_img);

        waitKey(0);
   }

    




    return 0;
}
