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

}
Registrar::Registrar(Config* pConfig)
{
	//���캯�������÷ֱ��ʵȲ���
	polarWidth = 1232;
	polarHeight = 1028;
	rgbWidth = 640;
	rgbHeight = 480;
	
	//���̸����

	boardSize.width = 11;
	boardSize.height = 8;

	polarCorners = Mat::zeros(rgbHeight, rgbWidth,CV_8UC1);
	rgbCorners = Mat::zeros(rgbHeight,rgbWidth,CV_8UC3);

	//�ǵ�ѡ��
	pt_0 = 0;
	pt_1 = 9;
	pt_2 = 25;
	pt_3 = 43;

	//��ʼ��͸�ӱ任����(��λ��)
	transformMat = Mat::eye(Size(3, 3), CV_64F);
	//��ʼ��ƫ������ڲξ���
	polarInstrinsic = Mat::eye(Size(3, 3), CV_64F);
	//��ʼ��ƫ������������
	polarDistortCoeffs = Mat::zeros(Size(1, 4), CV_64F);

	//��ȡ�����ļ�
	mpConfig = pConfig;
	ReadConfig();


	
}
Registrar::~Registrar()
{

}

void Registrar::CalculateTransform(Mat& polarImg, Mat& rgbImg)
{
	//ͨ�������������ͬһ���궨����������������ı任����
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
	
	//���ֱ���ͳһ(ƫ����С��rgbͼ���С�ķֱ���)
	Mat polarMat;
	resize(polarImg,polarMat,Size(rgbWidth,rgbHeight),0,0, INTER_AREA); //��Сͼ����AREA��ֵ��
	
	//����ƫ��ͼ�������
	//int bias = 0;
	//double min, max;
	//minMaxLoc(polarMat,&min,&max);
	//bias = 255 - max;
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
	
	//����͸�ӱ任����
	Point2f srcPoints[4];//ԭͼ�е�4���㣬��ƫ��ͼ�е�4����
	Point2f dstPoints[4];//Ŀ��ͼ�е��ĸ���,��rgbͼ�е�4����
	srcPoints[0] = polarPoints[pt_0];
	srcPoints[1] = polarPoints[pt_1];
	srcPoints[2] = polarPoints[pt_2];
	srcPoints[3] = polarPoints[pt_3];
	dstPoints[0] = rgbPoints[pt_0];
	dstPoints[1] = rgbPoints[pt_1];
	dstPoints[2] = rgbPoints[pt_2];
	dstPoints[3] = rgbPoints[pt_3];
	transformMat = getPerspectiveTransform(srcPoints,dstPoints);

	//����͸�ӱ任����
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
	//���������ͼ��ȿ�����CV_16UC1��Ҳ������CV_8UC1
	if (polarImg.rows != polarHeight || polarImg.cols != polarWidth)
	{
		printf_s("[-] Registrar::Process failed: polarImg size is different with polarWidth(Height)\n");
		return;
	}
	//ͳһ�ֱ���
	Mat polarMat;
	resize(polarImg, polarMat, Size(rgbWidth, rgbHeight), 0, 0, INTER_AREA); //��Сͼ����AREA��ֵ��

	//����͸�ӱ任
	warpPerspective(polarMat,registered_poarImg,transformMat, Size(polarMat.cols, polarMat.rows));
	
	//registered_poarImg.convertTo(registered_poarImg,registered_poarImg.type(),40,0);  //Ϊ����ʾ������
}
void Registrar::ReadConfig()
{
	//���Զ�ȡ�����ļ��еı任����
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

	//��ȡƫ������ڲ�
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

	//��ȡ����������
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


//���Դ��룺
//Mat polarImg;
//Mat rgbImg;
//polarImg = imread("D:\\OneDrive\\OneDrive - mails.tsinghua.edu.cn\\����\\ƫ�����\\test\\polar_MONO_12bit_Angle_001_Sum.tif", -1);
//imshow("polarImg", polarImg);
////std::cout << polarImg << std::endl;
//Mat polarMat;
//
//polarMat = CV_16UC1ToCV_8UC1(polarImg); //ת����ʽ
//
//rgbImg = imread("D:\\OneDrive\\OneDrive - mails.tsinghua.edu.cn\\����\\ƫ�����\\test\\Rgb_Color.png", -1);
//Registrar test;
//test.CalculateTransform(polarMat, rgbImg);
//test.Process(polarImg);
//imshow("rgbImg", rgbImg);
////��׼���ͼ��
//imshow("regiteredPolar", test.registered_poarImg);
//imshow("polarPoints", test.polarCorners);
//imshow("rgbPoints", test.rgbCorners);
//
////����ͼ��
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
