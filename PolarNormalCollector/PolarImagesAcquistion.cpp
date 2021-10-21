#include "PolarImagesAcquistion.h"
#include <iostream>
AcquirePolarImages::AcquirePolarImages()
{
	//构造函数
	hMapFile = NULL;
	lpBase = NULL;
	hMutex = NULL;
	OpenShareMemory();

	width = 1232;
	height = 1028;
	size_pixel = sizeof(UINT16);//每个像素占的内存大小，便于后面修改格式
	buf_size = width * height * 5 * size_pixel;//5张图片
	//初始化图像
	I_sum = cv::Mat::zeros(height,width,CV_16UC1); //注意rows=height, cols = width之间的转换关系
	I_0 = cv::Mat::zeros(height, width, CV_16UC1);
	I_45 = cv::Mat::zeros(height, width, CV_16UC1);
	I_90 = cv::Mat::zeros(height, width, CV_16UC1);
	I_135 = cv::Mat::zeros(height, width, CV_16UC1);
	S_0 = cv::Mat::zeros(height, width, CV_16UC1);
	S_1 = cv::Mat::zeros(height, width, CV_16UC1);
	S_2 = cv::Mat::zeros(height, width, CV_16UC1);
	AoLP = cv::Mat::zeros(height, width, CV_32FC1);
	DoLP = cv::Mat::zeros(height, width, CV_32FC1);

}
AcquirePolarImages::~AcquirePolarImages()
{
	//析构函数
	CloseShareMemory();
}

void AcquirePolarImages::ReadPolarImages()
{
	//读取内存
	if (hMapFile != NULL && lpBase != NULL && hMutex != NULL)
	{
		//申请占用互斥量
		DWORD mutex_state = WaitForSingleObject(hMutex,INFINITE);
		if (mutex_state == WAIT_OBJECT_0)
		{

			//占有互斥量，开始读取
			//printf_s("[debug] AcquirePolarImages::ReadPolarImages START\n");
			UINT16* pSum = (UINT16*)lpBase;
			UINT16* pI_0 = (UINT16*)((long long)lpBase + width * height * size_pixel * 1); //向后挪一个图片的位置
			UINT16* pI_45 = (UINT16*)((long long)lpBase + width * height * size_pixel * 2); 
			UINT16* pI_90 = (UINT16*)((long long)lpBase + width * height * size_pixel * 3); 
			UINT16* pI_135 = (UINT16*)((long long)lpBase + width * height * size_pixel * 4);

			for (int row = 0; row < height; row++)
			{
				for (int col = 0; col < width; col++)
				{
					
					I_sum.at<UINT16>(row, col) = *(pSum + ((long long)width * row + col)); //pSum本身是UINT16*了，因此在其基础上加的值会自动乘sizeof(UINT16)
					I_0.at<UINT16>(row, col) = *(pI_0 + ((long long)width * row + col)); //pSum本身是UINT16*了，因此在其基础上加的值会自动乘sizeof(UINT16)
					I_45.at<UINT16>(row, col) = *(pI_45 + ((long long)width * row + col)); //pSum本身是UINT16*了，因此在其基础上加的值会自动乘sizeof(UINT16)
					I_90.at<UINT16>(row, col) = *(pI_90 + ((long long)width * row + col)); //pSum本身是UINT16*了，因此在其基础上加的值会自动乘sizeof(UINT16)
					I_135.at<UINT16>(row, col) = *(pI_135 + ((long long)width * row + col)); //pSum本身是UINT16*了，因此在其基础上加的值会自动乘sizeof(UINT16)


				}
			}

			//读取完毕，释放互斥量
			ReleaseMutex(hMutex);
		}



	}
	else
	{
		printf_s("[-] AcqiorePolarImages::ReadPolarImages failed: share memory is incorrect\n");
		return;
	}
	//printf_s("[debug] AcquirePolarImages::ReadPolarImages Done\n");


}

void AcquirePolarImages::OpenShareMemory()
{
	
	hMapFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS,NULL, L"ShareMemoryForDataCollenction"); //打开共享文件对象
	if (hMapFile == NULL)
	{
		printf_s("[-] AcqiorePolarImages::OpenShareMemory failed: hMapFile is NULL\n");
		return;
	}
	
	lpBase = MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,0);
	if (lpBase == NULL)
	{
		printf_s("[-] AcqiorePolarImages::OpenShareMemory failed: lpBase is NULL\n");
		return;
	}
	
	//打开互斥锁
	hMutex = OpenMutexW(MUTEX_ALL_ACCESS,NULL,L"ShareMemoryMutex");
	if (hMutex == NULL)
	{
		printf_s("[-] AcqiorePolarImages::OpenShareMemory failed: hMutex is NULL\n");
		return;
	}
	printf_s("[+] AcqiorePolarImages::OpenShareMemory succeed\n");



}


void AcquirePolarImages::CloseShareMemory()
{
	if (lpBase != NULL)
		UnmapViewOfFile(lpBase);
	if (hMapFile != NULL)
		CloseHandle(hMapFile);
	//互斥量的申请和释放由一个进程完成，因此不能在此进程内释放互斥量
	//if (hMutex != NULL)
	//	CloseHandle(hMutex);
}

