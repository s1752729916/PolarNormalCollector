/**************************************************************************

Author: SMQ

Date:2021-10-26

Description:
对FrameManager类的实现

**************************************************************************/
#include "FrameManager.h"


FrameManager::FrameManager()
{
	printf_s("[<-] FrameManager Starting...\n");
	//初始化各个类
	pPolarAcquirer = new AcquirePolarImages;
	pRealSenseAcquirer = new AcquireRealSense;
	pRegistrar = new Registrar;
	//pDisplayer = new Displayer;
	
	//设置参数
	isProcessingEnabled = true;
	

	//开启线程运行void Processing()
	std::thread processing_thread([&]()
	{


		printf_s("[<-] FrameManager::Processing is ongoing.\n");
		if(isProcessingEnabled)
			Processing();
		Sleep(20); 

	});

	printf_s("[+] FrameManager Started!\n");
}
void FrameManager::Processing()
{
	//读取极化图像并进行后处理
	pPolarAcquirer->ReadPolarImages();
	pPolarAcquirer->PostProcessing();
	I_sum = pPolarAcquirer->I_sum; //因为这个类里的Mat是单独开的内存，所以不需要再clone开内存了
	I_0 = pPolarAcquirer->I_0;
	I_45 = pPolarAcquirer->I_45;
	I_90 = pPolarAcquirer->I_90;
	I_135 = pPolarAcquirer->I_135;
	AoLP = pPolarAcquirer->AoLP;
	DoLP = pPolarAcquirer->DoLP;

	//读取RealSense图像
	pRealSenseAcquirer->GetPictures();
	rgb = pRealSenseAcquirer->raw_rgb_mat;
	depth = pRealSenseAcquirer->filtered_depth_mat;
	colorDepth = pRealSenseAcquirer->color_filtered_depth;
	normal = pRealSenseAcquirer->color_filtered_depth;//TODO：法线还未实现

	//Displayer操作...
	
	
}
void FrameManager::Freeze()
{
	//暂停pipeline
	isProcessingEnabled = false;
	
	//保存当前状态
	I_sum_freeze = I_sum.clone();
	I_0_freeze = I_0.clone();
	I_45_freeze = I_45.clone();
	I_90_freeze = I_90.clone();
	I_135_freeze = I_135.clone();
	AoLP_freeze = AoLP.clone();
	DoLP_freeze = DoLP.clone();
	rgb_freeze = rgb.clone();
	depth_freeze = depth.clone();
	colorDepth_freeze = colorDepth.clone();
	normal_freeze = normal.clone();

}


FrameManager::~FrameManager()
{
	//
	delete pPolarAcquirer;
	delete pRealSenseAcquirer;
	delete pRegistrar;
	//delete pDisplayer;
}