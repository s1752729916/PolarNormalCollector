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
#include "FrameManager.h"

#include "MainWindow.h"
#include <QtWidgets/qapplication.h>

using namespace cv;

int main(int argc, char* argv[]) 
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    return a.exec();
}
