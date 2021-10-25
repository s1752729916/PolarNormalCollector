/**************************************************************************

Author: SMQ

Date:2021-10-24

Description:
���ļ��ṩ���õ�API���粻ͬ��ʽ֮���ת���ȣ����ȵ�����
**************************************************************************/
#include <opencv2/opencv.hpp>
using namespace cv;

Mat CV_16UC1ToCV_8UC1(Mat& input)
{
	//��֤���Ȳ���ĸ�ʽת��
	if (input.type() != CV_16UC1)
	{
		printf_s("utils::CV_16UC1ToCV_8UC1 input type is not CV_16UC1\n");
		return input;
	}

	Mat out; 
	out = input / 257;
	out.convertTo(out,CV_8UC1);


	//���غ����ڲ������Mat�ǿ��е�
	//Mat�ɼ������������ͷ������˺��������󲻻���������ڲ������Mat
	return out;

}
Mat CV_8UC1ToCU_16UC1(Mat& input)
{
	//��֤���Ȳ���ĸ�ʽת��
	if (input.type() != CV_8UC1)
	{
		printf_s("utils::CV_8UC1ToCU_16UC1 input type is not CV_8UC1\n");
		return input;
	}
	Mat out;
	input.convertTo(out,CV_16UC1);
	out = out * 257;//��ת����ʽ�ٳ˱�������ֵ�������
	return out;
}
Mat CV_8UC1_Brighter(Mat& input)
{
	//��CV_8UC1��ͼ����м�������:����min-max���ŵ�0��255��Χ��
	if (input.type() != CV_8UC1)
	{
		printf_s("utils::CV_8UC1_Brighter input type is not CV_8UC1\n");
		return input;
	}

	Mat out;
	
	normalize(input,out,0,255,NORM_MINMAX);


	return out;
}
