/**************************************************************************

Author: SMQ

Date:2021-10-21

Description: 
1、这个类主要用于从GigeDemo中获取五副偏振图像，读取的方法与GigeDemo进程使用共享内存进行数据交换；
2、除了完成读取功能外，还完成图像的后处理，包括计算斯托克斯矢量，计算AoLP和DoLP

**************************************************************************/
#pragma once
#include <Windows.h>
#include <opencv2/opencv.hpp>

class AcquirePolarImages
{
public:
	AcquirePolarImages();
	~AcquirePolarImages();
	void ReadPolarImages();
	void OpenShareMemory();//构造函数中调用
	void CloseShareMemory();//析构函数中调用
	void PostProcessing();

	
public:
	cv::Mat I_sum;
	cv::Mat I_0;
	cv::Mat I_45;
	cv::Mat I_90;
	cv::Mat I_135;
	cv::Mat S_0;
	cv::Mat S_1; //for display
	cv::Mat S_2; //for display
	cv::Mat DoLP; //CV_32FU1
	cv::Mat AoLP; //CV_32FU1
private:
	HANDLE hMapFile; //共享文件句柄
	LPVOID lpBase; //指向共享内存的指针
	HANDLE hMutex; //互斥锁

	int size_pixel;//每个像素占内存大小,两种取值:sizeof(UINT8),sizeof(UINT16)
	int width;
	int height;
	SIZE_T buf_size; //共享内存大小(5张图片)
};