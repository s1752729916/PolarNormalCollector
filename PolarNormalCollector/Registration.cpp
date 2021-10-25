/**************************************************************************

Author: SMQ

Date:2021-10-23

Description:
��Registrar���ʵ��
**************************************************************************/

#include "Registration.h"

using namespace std;
Registrar::Registrar()
{
	//���캯�������÷ֱ��ʵȲ���
	polarWidth = 1232;
	polarHeight = 1028;
	rgbWidth = 640;
	rgbHeight = 480;
	
	//���̸����

	boardSize.width = 9;
	boardSize.height = 6;

	polarCorners = Mat::zeros(rgbHeight, rgbWidth,CV_8UC1);
	rgbCorners = Mat::zeros(rgbHeight,rgbWidth,CV_8UC3);
}
Registrar::~Registrar()
{

}

void Registrar::CalculateTransform(Mat& polarImg, Mat& rgbImg)
{
	//ͨ�������������ͬһ���궨����������������ı任����
	printf_s("type: %d\n", polarImg.type());
	printf_s("cv_16UC1:%d\n", CV_8UC1);
	if (polarImg.rows != polarHeight || polarImg.cols != polarWidth)
	{
		printf_s("[-] Registrar::CalculateTransform failed: polarImg size is different with polarWidth(Height)\n");
		return;
	}
	if (rgbImg.rows != rgbHeight || rgbImg.cols != rgbWidth)
	{
		printf_s("[-] Registrar::CalculateTransform failed: rgbImg size is different with rgbWidth(Height)\n");
		return;
	}
	
	//���ֱ���ͳһ(ƫ����С��rgbͼ���С�ķֱ���)
	Mat polarMat;
	resize(polarImg,polarMat,Size(rgbWidth,rgbHeight),0,0, INTER_AREA); //��Сͼ����AREA��ֵ��
	
	////����ƫ��ͼ�������
	//int bias = 0;
	//double min, max;
	//minMaxLoc(polarMat,&min,&max);
	////bias = 255 - max;
	//polarMat = polarMat + bias;




	//�������
	bool found = false;
	vector<Point2f> polarPoints;
	vector<Point2f> rgbPoints;
	polarCorners = polarMat.clone();
	rgbCorners = rgbImg.clone();

	//Ѱ�����̸�ǵ�
	found = findChessboardCornersSB(polarMat,boardSize,polarPoints,CALIB_CB_EXHAUSTIVE|CALIB_CB_ACCURACY);
	if (!found)
	{
		printf_s("[-] Registrar::CalculateTransform failed: polarImg findChessboardCornersSB not found\n");
		return;
	}
	found = findChessboardCornersSB(rgbImg, boardSize, rgbPoints, CALIB_CB_EXHAUSTIVE | CALIB_CB_ACCURACY);
	if (!found)
	{
		printf_s("[-] Registrar::CalculateTransform failed: rgbImg findChessboardCornersSB not found\n");
		return;
	}
	//���ҵ��Ľǵ�
	drawChessboardCorners(polarCorners,boardSize,Mat(polarPoints),found);
	drawChessboardCorners(rgbCorners,boardSize,Mat(rgbPoints),found);
	
	printf_s("[+] Registrar::CalculateTransform succeed\n");
}
void Registrar::Process(Mat& polarImg, Mat& rgbImg)
{

}

void Registrar::ReadTransform()
{

}

