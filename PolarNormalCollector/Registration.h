#pragma once
/**************************************************************************

Author: SMQ

Date:2021-10-24

Description:
这个类用于偏振图像和深度图像的配准
1、输入是两张不同分辨率的图像(极化和rgb-d图像)，然后对齐到同一个尺寸，再把两张分辨率图像进行配准
2、偏振图像分辨率比rgb-d图像高，所以就以rgb-d图像分辨率为基准，把偏振图像resize到rgb-d中，并把偏振配准到rgb-d的rgb图像中
3、两个功能，一个用来计算变换矩阵，一个是利用已经计算好的变换矩阵
4、要求输入图像必须是uint8格式的
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
	void Process(Mat& polarImg);//这个函数是已有变换矩阵后对图像流进行处理的函数，应当在主pipeline中进行调用，这个函数对输入格式无要求
	void CalculateTransform(Mat& polarImg, Mat& rgbImg);//这个函数是用来计算两个图像之间的变换矩阵的，两张图像应包含相同的棋盘格信息,注意这里的输入偏振图像必须要转换成CV_8UC1类型
	void ReadConfig();//读取配置文件，例如透视变换矩阵，畸变参数等等
	void UndistortPolarImg(Mat& polarImg);//对偏振图像去畸变
private:
	//偏振图像分辨率
	int polarWidth;
	int polarHeight;
	Config* mpConfig;
	//rgb图像分辨率
	int rgbWidth;
	int rgbHeight;

	//用于配准的棋盘格参数
	Size boardSize;
	
	//棋盘角点号，棋盘的角点选择会很大程度上影响配准效果
	int pt_0;
	int pt_1;
	int pt_2;
	int pt_3;


	Mat transformMat; //两张图片间的仿射变换矩阵
	Mat polarInstrinsic;//相机内参
	Mat polarDistortCoeffs;//畸变参数,1x4矩阵，(kc1,kc2,kc3,kc4)，RadialDistortion(kc1,kc2)，TangentialDistortion(kc3,kc4)

public:
	//画提取到的角点
	Mat polarCorners;
	Mat rgbCorners;
	//配准后的图像
	Mat registered_poarImg; //配准过的偏振图像
	Mat registered_rgbImg; //以rgb配准的话这个与输入的rgbImg相等



	//畸变修正
	Mat undistorted_polarImg;

};

