/**************************************************************************

Author: SMQ

Date:2021-10-24

Description:
该文件提供常用的API，如不同格式之间的转换等，亮度调整等
**************************************************************************/
#include <opencv2/opencv.hpp>
using namespace cv;

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
	out.convertTo(out,CV_8UC1);


	//返回函数内部定义的Mat是可行的
	//Mat由计数器来控制释放与否，因此函数结束后不会清除函数内部定义的Mat
	return out;

}
Mat CV_8UC1ToCU_16UC1(Mat& input)
{
	//保证亮度不变的格式转换
	if (input.type() != CV_8UC1)
	{
		printf_s("utils::CV_8UC1ToCU_16UC1 input type is not CV_8UC1\n");
		return input;
	}
	Mat out;
	input.convertTo(out,CV_16UC1);
	out = out * 257;//先转换格式再乘倍数，数值不会溢出
	return out;
}
Mat CV_8UC1_Brighter(Mat& input)
{
	//对CV_8UC1的图像进行加亮处理:即把min-max缩放到0到255范围内
	if (input.type() != CV_8UC1)
	{
		printf_s("utils::CV_8UC1_Brighter input type is not CV_8UC1\n");
		return input;
	}

	Mat out;
	
	normalize(input,out,0,255,NORM_MINMAX);


	return out;
}
