#include "PolarImagesAcquistion.h"
#include <iostream>
AcquirePolarImages::AcquirePolarImages()
{
	//���캯��
	hMapFile = NULL;
	lpBase = NULL;
	hMutex = NULL;
	OpenShareMemory();

	width = 1232;
	height = 1028;
	size_pixel = sizeof(UINT16);//ÿ������ռ���ڴ��С�����ں����޸ĸ�ʽ
	buf_size = width * height * 5 * size_pixel;//5��ͼƬ
	//��ʼ��ͼ��
	I_sum = cv::Mat::zeros(height,width,CV_16UC1); //ע��rows=height, cols = width֮���ת����ϵ
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
	//��������
	CloseShareMemory();
}

void AcquirePolarImages::ReadPolarImages()
{
	//��ȡ�ڴ�
	if (hMapFile != NULL && lpBase != NULL && hMutex != NULL)
	{
		//����ռ�û�����
		DWORD mutex_state = WaitForSingleObject(hMutex,INFINITE);
		if (mutex_state == WAIT_OBJECT_0)
		{

			//ռ�л���������ʼ��ȡ
			//printf_s("[debug] AcquirePolarImages::ReadPolarImages START\n");
			UINT16* pSum = (UINT16*)lpBase;
			UINT16* pI_0 = (UINT16*)((long long)lpBase + width * height * size_pixel * 1); //���Ųһ��ͼƬ��λ��
			UINT16* pI_45 = (UINT16*)((long long)lpBase + width * height * size_pixel * 2); 
			UINT16* pI_90 = (UINT16*)((long long)lpBase + width * height * size_pixel * 3); 
			UINT16* pI_135 = (UINT16*)((long long)lpBase + width * height * size_pixel * 4);

			for (int row = 0; row < height; row++)
			{
				for (int col = 0; col < width; col++)
				{
					
					I_sum.at<UINT16>(row, col) = *(pSum + ((long long)width * row + col)); //pSum������UINT16*�ˣ������������ϼӵ�ֵ���Զ���sizeof(UINT16)
					I_0.at<UINT16>(row, col) = *(pI_0 + ((long long)width * row + col)); //pSum������UINT16*�ˣ������������ϼӵ�ֵ���Զ���sizeof(UINT16)
					I_45.at<UINT16>(row, col) = *(pI_45 + ((long long)width * row + col)); //pSum������UINT16*�ˣ������������ϼӵ�ֵ���Զ���sizeof(UINT16)
					I_90.at<UINT16>(row, col) = *(pI_90 + ((long long)width * row + col)); //pSum������UINT16*�ˣ������������ϼӵ�ֵ���Զ���sizeof(UINT16)
					I_135.at<UINT16>(row, col) = *(pI_135 + ((long long)width * row + col)); //pSum������UINT16*�ˣ������������ϼӵ�ֵ���Զ���sizeof(UINT16)


				}
			}

			//��ȡ��ϣ��ͷŻ�����
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
	
	hMapFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS,NULL, L"ShareMemoryForDataCollenction"); //�򿪹����ļ�����
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
	
	//�򿪻�����
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
	//��������������ͷ���һ��������ɣ���˲����ڴ˽������ͷŻ�����
	//if (hMutex != NULL)
	//	CloseHandle(hMutex);
}

