/**************************************************************************

Author: SMQ

Date:2021-10-21

Description: 
1���������Ҫ���ڴ�GigeDemo�л�ȡ�帱ƫ��ͼ�񣬶�ȡ�ķ�����GigeDemo����ʹ�ù����ڴ�������ݽ�����
2��������ɶ�ȡ�����⣬�����ͼ��ĺ�����������˹�п�˹ʸ��������AoLP��DoLP

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
	void OpenShareMemory();//���캯���е���
	void CloseShareMemory();//���������е���
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
	HANDLE hMapFile; //�����ļ����
	LPVOID lpBase; //ָ�����ڴ��ָ��
	HANDLE hMutex; //������

	int size_pixel;//ÿ������ռ�ڴ��С,����ȡֵ:sizeof(UINT8),sizeof(UINT16)
	int width;
	int height;
	SIZE_T buf_size; //�����ڴ��С(5��ͼƬ)
};