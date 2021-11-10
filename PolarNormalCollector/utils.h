/**************************************************************************

Author: SMQ

Date:2021-10-24

Description:
该文件提供常用的API，如不同格式之间的转换，保存等
**************************************************************************/
#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
Mat CV_16UC1ToCV_8UC1(Mat& input);
Mat CV_16UC1ToCV_8UC3(Mat& input);
Mat CV_32FC1ToCV_8UC3(Mat& input);
Mat CV_16UC12CV_8UC1_12Bit(Mat& input);
Mat CV_32FC3ToCV_8UC3(Mat& input);

Mat depth2normal(Mat& depth, float depth_scale);  //深度图转法线图，深度图输入是CV_16UC1，法线图输出是CV_32FUC3
void Save16UC1(String& path, Mat& img); //虽然下面四个用的代码基本相同，但是分开写是为了区分不同图片的类型，以便后续查阅
void Save8UC3(String& path, Mat& img);
void Save32FC1(String& path, Mat& img);
void Save32FC3(String& path, Mat& img);
void Save8UC1(String& path, Mat& img);

template<typename ... Args>
static std::string str_format(const std::string& format, Args ... args)
{
	auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

	if (!buf)
		return std::string("");

	std::snprintf(buf.get(), size_buf, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size_buf - 1);
}