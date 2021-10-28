#include "utils.h"
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
	out.convertTo(out, CV_8UC1);


	//���غ����ڲ������Mat�ǿ��е�
	//Mat�ɼ������������ͷ������˺��������󲻻���������ڲ������Mat
	return out.clone();

}
Mat CV_8UC1ToCV_16UC1(Mat& input)
{
	//��֤���Ȳ���ĸ�ʽת��
	if (input.type() != CV_8UC1)
	{
		printf_s("utils::CV_8UC1ToCU_16UC1 input type is not CV_8UC1\n");
		return input;
	}
	Mat out;
	input.convertTo(out, CV_16UC1);
	out = out * 257;//��ת����ʽ�ٳ˱�������ֵ�������
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
	cvtColor(_8UC1, _8UC3, COLOR_GRAY2RGB);//ת����8UC3
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
	cvtColor(_8UC1, _8UC3, COLOR_GRAY2RGB);//ת����8UC3
	return _8UC3;
}

