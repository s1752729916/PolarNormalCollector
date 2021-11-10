/**************************************************************************

Author: SMQ

Date:2021-10-24

Description:
���ļ��ṩ���õ�API���粻ͬ��ʽ֮���ת���������
**************************************************************************/
#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
Mat CV_16UC1ToCV_8UC1(Mat& input);
Mat CV_16UC1ToCV_8UC3(Mat& input);
Mat CV_32FC1ToCV_8UC3(Mat& input);
Mat CV_16UC12CV_8UC1_12Bit(Mat& input);
Mat CV_32FC3ToCV_8UC3(Mat& input);

Mat depth2normal(Mat& depth, float depth_scale);  //���ͼת����ͼ�����ͼ������CV_16UC1������ͼ�����CV_32FUC3
void Save16UC1(String& path, Mat& img); //��Ȼ�����ĸ��õĴ��������ͬ�����Ƿֿ�д��Ϊ�����ֲ�ͬͼƬ�����ͣ��Ա��������
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