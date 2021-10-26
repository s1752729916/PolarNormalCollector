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
	pDisplayer = new Displayer;
	
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

	//读取RealSense图像
	pRealSenseAcquirer->GetPictures();

	

	//配准操作
	pRegistrar->Process(pPolarAcquirer->I_sum);
	I_sum = pRegistrar->registered_poarImg.clone();

	pRegistrar->Process(pPolarAcquirer->I_0);
	I_0 = pRegistrar->registered_poarImg.clone();

	pRegistrar->Process(pPolarAcquirer->I_45);
	I_45 = pRegistrar->registered_poarImg.clone();

	pRegistrar->Process(pPolarAcquirer->I_90);
	I_90 = pRegistrar->registered_poarImg.clone();

	pRegistrar->Process(pPolarAcquirer->I_135);
	I_135 = pRegistrar->registered_poarImg.clone();

	pRegistrar->Process(pPolarAcquirer->AoLP);
	AoLP = pRegistrar->registered_poarImg.clone();

	pRegistrar->Process(pPolarAcquirer->DoLP);
	DoLP = pRegistrar->registered_poarImg.clone();

	rgb = pRealSenseAcquirer->raw_rgb_mat;
	depth = pRealSenseAcquirer->filtered_depth_mat;
	colorDepth = pRealSenseAcquirer->color_filtered_depth;
	normal = pRealSenseAcquirer->color_filtered_depth;//TODO：法线还未实现

	//Displayer操作...
	pDisplayer->Display(I_sum,DoLP,AoLP,rgb,depth,colorDepth,normal);
	
	
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
	delete pDisplayer;
}