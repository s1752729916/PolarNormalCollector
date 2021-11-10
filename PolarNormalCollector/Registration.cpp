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

}
Registrar::Registrar(Config* pConfig)
{
	//构造函数，配置分辨率等参数
	polarWidth = 1232;
	polarHeight = 1028;
	rgbWidth = 640;
	rgbHeight = 480;
	
	//棋盘格参数

	boardSize.width = 11;
	boardSize.height = 8;

	polarCorners = Mat::zeros(rgbHeight, rgbWidth,CV_8UC1);
	rgbCorners = Mat::zeros(rgbHeight,rgbWidth,CV_8UC3);

	//角点选择
	pt_0 = 0;
	pt_1 = 9;
	pt_2 = 25;
	pt_3 = 43;

	//初始化透视变换矩阵(单位阵)
	transformMat = Mat::eye(Size(3, 3), CV_64F);
	//初始化偏振相机内参矩阵
	polarInstrinsic = Mat::eye(Size(3, 3), CV_64F);
	//初始化偏振相机畸变参数
	polarDistortCoeffs = Mat::zeros(Size(1, 4), CV_64F);

	//读取配置文件
	mpConfig = pConfig;
	ReadConfig();


	
}
Registrar::~Registrar()
{

}

void Registrar::CalculateTransform(Mat& polarImg, Mat& rgbImg)
{
	//通过两个相机拍摄同一个标定物来计算两个相机的变换矩阵
	//printf_s("type: %d\n", polarImg.type());
	//printf_s("cv_16UC1:%d\n", CV_8UC1);
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
	
	//调整偏振图像的亮度
	//int bias = 0;
	//double min, max;
	//minMaxLoc(polarMat,&min,&max);
	//bias = 255 - max;
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
	
	//计算透视变换矩阵
	Point2f srcPoints[4];//原图中的4个点，即偏振图中的4个点
	Point2f dstPoints[4];//目标图中的四个点,即rgb图中的4个点
	srcPoints[0] = polarPoints[pt_0];
	srcPoints[1] = polarPoints[pt_1];
	srcPoints[2] = polarPoints[pt_2];
	srcPoints[3] = polarPoints[pt_3];
	dstPoints[0] = rgbPoints[pt_0];
	dstPoints[1] = rgbPoints[pt_1];
	dstPoints[2] = rgbPoints[pt_2];
	dstPoints[3] = rgbPoints[pt_3];
	transformMat = getPerspectiveTransform(srcPoints,dstPoints);

	//保存透视变换矩阵
	std::vector<double> tf;

	for (int row = 0; row < transformMat.rows; row++)
	{
		for (int col = 0; col < transformMat.cols; col++)
		{
			tf.push_back(transformMat.at<double>(row, col));
		}
	}

	mpConfig->Add("TransformMatrix",tf);
	


	printf_s("[+] Registrar::CalculateTransform succeed\n");
}
void Registrar::Process(Mat& polarImg)
{
	//这里输入的图像既可以是CV_16UC1，也可以是CV_8UC1
	if (polarImg.rows != polarHeight || polarImg.cols != polarWidth)
	{
		printf_s("[-] Registrar::Process failed: polarImg size is different with polarWidth(Height)\n");
		return;
	}
	//统一分辨率
	Mat polarMat;
	resize(polarImg, polarMat, Size(rgbWidth, rgbHeight), 0, 0, INTER_AREA); //缩小图像用AREA插值法

	//进行透视变换
	warpPerspective(polarMat,registered_poarImg,transformMat, Size(polarMat.cols, polarMat.rows));
	
	//registered_poarImg.convertTo(registered_poarImg,registered_poarImg.type(),40,0);  //为了显示更方便
}
void Registrar::ReadConfig()
{
	//尝试读取配置文件中的变换矩阵
	std::vector<double> tf;
	if (mpConfig->ReadInto(tf, "TransformMatrix"))
	{
		for (int row = 0; row < transformMat.rows; row++)
		{
			for (int col = 0; col < transformMat.cols; col++)
			{
				transformMat.at<double>(row, col) = tf[row * transformMat.cols + col];
			}
		}
	}
	else
	{
		printf_s("[-] Registrar::ReadConfig: TransformMatrix load failed.\n");

	}

	//读取偏振相机内参
	if (mpConfig->ReadInto(tf, "PolarIntrinsicMatrix"))
	{
		for (int row = 0; row < polarInstrinsic.rows; row++)
		{
			for (int col = 0; col < polarInstrinsic.cols; col++)
			{
				polarInstrinsic.at<double>(row, col) = tf[row * polarInstrinsic.cols + col];
			}
		}
	}
	else
	{
		printf_s("[-] Registrar::ReadConfig: PolarIntrinsicMatrix load failed.\n");

	}

	//读取相机畸变参数
	if (mpConfig->ReadInto(tf, "PolarDistortionCoeffs"))
	{
		for (int i = 0; i < 4; i++)
		{
			polarDistortCoeffs.at<double>(i, 0) = tf[i];
		}
	}
	else
	{
		printf_s("[-] Registrar::ReadConfig: PolarDistortionCoeffs load failed.\n");
	}
	printf_s("[+] Registrar::ReadConfig: Load params succeed.\n");
	



}
void Registrar::UndistortPolarImg(Mat& polarImg)
{
	if (polarImg.empty())
	{
		printf_s("[+] Registrar::UndistortPolarImg: Input polarImg is empty.\n");
		return;
	}
	undistort(polarImg,undistorted_polarImg,polarInstrinsic,polarDistortCoeffs);
}


//测试代码：
//Mat polarImg;
//Mat rgbImg;
//polarImg = imread("D:\\OneDrive\\OneDrive - mails.tsinghua.edu.cn\\桌面\\偏振相机\\test\\polar_MONO_12bit_Angle_001_Sum.tif", -1);
//imshow("polarImg", polarImg);
////std::cout << polarImg << std::endl;
//Mat polarMat;
//
//polarMat = CV_16UC1ToCV_8UC1(polarImg); //转换格式
//
//rgbImg = imread("D:\\OneDrive\\OneDrive - mails.tsinghua.edu.cn\\桌面\\偏振相机\\test\\Rgb_Color.png", -1);
//Registrar test;
//test.CalculateTransform(polarMat, rgbImg);
//test.Process(polarImg);
//imshow("rgbImg", rgbImg);
////配准后的图像
//imshow("regiteredPolar", test.registered_poarImg);
//imshow("polarPoints", test.polarCorners);
//imshow("rgbPoints", test.rgbCorners);
//
////亮化图像
//Mat bright_polar;
//polarImg.convertTo(bright_polar, CV_8UC1);
//imshow("Bright Polar", bright_polar);
//test.Process(bright_polar);
//imshow("Bright regiteredPolar", test.registered_poarImg);
//
//
//
//std::cout << test.transformMat << std::endl;
//waitKey(0);
