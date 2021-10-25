#pragma once
/**************************************************************************

Author: SMQ

Date:2021-10-24

Description:
���������ƫ��ͼ������ͼ�����׼
1�����������Ų�ͬ�ֱ��ʵ�ͼ��(������rgb-dͼ��)��Ȼ����뵽ͬһ���ߴ磬�ٰ����ŷֱ���ͼ�������׼
2��ƫ��ͼ��ֱ��ʱ�rgb-dͼ��ߣ����Ծ���rgb-dͼ��ֱ���Ϊ��׼����ƫ��ͼ��resize��rgb-d�У�����ƫ����׼��rgb-d��rgbͼ����
3���������ܣ�һ����������任����һ���������Ѿ�����õı任����
4��Ҫ������ͼ�������uint8��ʽ��
**************************************************************************/
#include <opencv2/opencv.hpp>
using namespace cv;
class Registrar
{
public:
	Registrar();
	~Registrar();
	void Process(Mat& polarImg, Mat& rgbImg);//������������б任������ͼ�������д���ĺ�����Ӧ������pipeline�н��е���
	void CalculateTransform(Mat& polarImg, Mat& rgbImg);//���������������������ͼ��֮��ı任����ģ�����ͼ��Ӧ������ͬ�����̸���Ϣ
	void ReadTransform();//�������ļ��ж�ȡ�任����(�����ټ�)
private:
	//ƫ��ͼ��ֱ���
	int polarWidth;
	int polarHeight;
	
	//rgbͼ��ֱ���
	int rgbWidth;
	int rgbHeight;

	//������׼�����̸����
	Size boardSize;
	
	//�任����
	Mat transformMat; //����ͼƬ��ķ���任����
public:
	//����ȡ���Ľǵ�
	Mat polarCorners;
	Mat rgbCorners;
	//��׼���ͼ��
	Mat registered_poarImg; //��׼����ƫ��ͼ��
	Mat registered_rgbImg; //��rgb��׼�Ļ�����������rgbImg���

};

