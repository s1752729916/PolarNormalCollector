/**************************************************************************

Author: SMQ

Date:2021-10-24

Description:
该文件提供常用的API，如不同格式之间的转换等，亮度调整等
**************************************************************************/
#include <opencv2/opencv.hpp>
using namespace cv;
Mat CV_16UC1ToCV_8UC1(Mat& input);
Mat CV_8UC1ToCV_16UC1(Mat& input);
Mat CV_16UC1ToCV_8UC3(Mat& input);
Mat CV_32FC1ToCV_8UC3(Mat& input);

