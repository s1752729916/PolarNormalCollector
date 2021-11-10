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
#include "Config.h"

using namespace cv;
class Registrar
{
public:
	Registrar();
	Registrar(Config* pConfig);
	~Registrar();
	void Process(Mat& polarImg);//������������б任������ͼ�������д���ĺ�����Ӧ������pipeline�н��е��ã���������������ʽ��Ҫ��
	void CalculateTransform(Mat& polarImg, Mat& rgbImg);//���������������������ͼ��֮��ı任����ģ�����ͼ��Ӧ������ͬ�����̸���Ϣ,ע�����������ƫ��ͼ�����Ҫת����CV_8UC1����
	void ReadConfig();//��ȡ�����ļ�������͸�ӱ任���󣬻�������ȵ�
	void UndistortPolarImg(Mat& polarImg);//��ƫ��ͼ��ȥ����
private:
	//ƫ��ͼ��ֱ���
	int polarWidth;
	int polarHeight;
	Config* mpConfig;
	//rgbͼ��ֱ���
	int rgbWidth;
	int rgbHeight;

	//������׼�����̸����
	Size boardSize;
	
	//���̽ǵ�ţ����̵Ľǵ�ѡ���ܴ�̶���Ӱ����׼Ч��
	int pt_0;
	int pt_1;
	int pt_2;
	int pt_3;


	Mat transformMat; //����ͼƬ��ķ���任����
	Mat polarInstrinsic;//����ڲ�
	Mat polarDistortCoeffs;//�������,1x4����(kc1,kc2,kc3,kc4)��RadialDistortion(kc1,kc2)��TangentialDistortion(kc3,kc4)

public:
	//����ȡ���Ľǵ�
	Mat polarCorners;
	Mat rgbCorners;
	//��׼���ͼ��
	Mat registered_poarImg; //��׼����ƫ��ͼ��
	Mat registered_rgbImg; //��rgb��׼�Ļ�����������rgbImg���



	//��������
	Mat undistorted_polarImg;

};

