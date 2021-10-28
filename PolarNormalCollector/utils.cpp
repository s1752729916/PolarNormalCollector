#include "utils.h"
Mat CV_16UC1ToCV_8UC1(Mat& input)
{
	//保证亮度不变的格式转换
	if (input.type() != CV_16UC1)
	{
		printf_s("utils::CV_16UC1ToCV_8UC1 input type is not CV_16UC1\n");
		return input;
	}

	Mat out;
	out = input / 257;
	out.convertTo(out, CV_8UC1);


	//返回函数内部定义的Mat是可行的
	//Mat由计数器来控制释放与否，因此函数结束后不会清除函数内部定义的Mat
	return out.clone();

}
Mat CV_8UC1ToCV_16UC1(Mat& input)
{
	//保证亮度不变的格式转换
	if (input.type() != CV_8UC1)
	{
		printf_s("utils::CV_8UC1ToCU_16UC1 input type is not CV_8UC1\n");
		return input;
	}
	Mat out;
	input.convertTo(out, CV_16UC1);
	out = out * 257;//先转换格式再乘倍数，数值不会溢出
	return out;
}

Mat CV_16UC1ToCV_8UC3(Mat& input)
{

	if (input.type() != CV_16UC1)
	{
		printf_s("utils::CV_16UC1ToCV_8UC3 input type is not CV_16UC1\n");
		return input;
	}
	Mat _8UC1;
	Mat _8UC3;
	_8UC1 = CV_16UC1ToCV_8UC1(input);
	cvtColor(_8UC1, _8UC3, COLOR_GRAY2RGB);//转换成8UC3
	return _8UC3;
	

}
Mat CV_32FC1ToCV_8UC3(Mat& input)
{
	if (input.type() != CV_32FC1)
	{
		printf_s("utils::CV_32FC1ToCV_8UC3 input type is not CV_32FC1\n");
		return input;
	}
	Mat _8UC1;
	Mat _8UC3;
	input.convertTo(_8UC1,CV_8UC1,255.0);
	cvtColor(_8UC1, _8UC3, COLOR_GRAY2RGB);//转换成8UC3
	return _8UC3;
}

