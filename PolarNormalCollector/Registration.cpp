/**************************************************************************

Author: SMQ

Date:2021-10-23

Description:
对Registrar类的实现
**************************************************************************/

#include "Registration.h"

using namespace std;
Registrar::Registrar()
{
	//构造函数，配置分辨率等参数
	polarWidth = 1232;
	polarHeight = 1028;
	rgbWidth = 640;
	rgbHeight = 480;
	
	//棋盘格参数

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
	//通过两个相机拍摄同一个标定物来计算两个相机的变换矩阵
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
	
	//将分辨率统一(偏振缩小到rgb图像大小的分辨率)
	Mat polarMat;
	resize(polarImg,polarMat,Size(rgbWidth,rgbHeight),0,0, INTER_AREA); //缩小图像用AREA插值法
	
	////调整偏振图像的亮度
	//int bias = 0;
	//double min, max;
	//minMaxLoc(polarMat,&min,&max);
	////bias = 255 - max;
	//polarMat = polarMat + bias;




	//定义参量
	bool found = false;
	vector<Point2f> polarPoints;
	vector<Point2f> rgbPoints;
	polarCorners = polarMat.clone();
	rgbCorners = rgbImg.clone();

	//寻找棋盘格角点
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
	//画找到的角点
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

