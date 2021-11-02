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

	//启动子模块线程
	pRealSenseAcquirer->start();
	
	
	//设置参数
	isProcessingEnabled = true;
	
	
	


	printf_s("[+] FrameManager Started!\n");
}
void FrameManager::start()
{
	//开启线程运行void Processing()
	processing_thread =  std::thread(&FrameManager::Processing,this);

}
void FrameManager::Processing()
{
	printf_s("[<-] FrameManager::Processing is ongoing.\n");
	while (1)
	{
		if (isProcessingEnabled)
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
			normal = depth2normal(pRealSenseAcquirer->filtered_depth_mat,pRealSenseAcquirer->GetDepthScale());//TODO：法线还未实现

			//叠加操作
			if (isFreeze)
			{
				addWeighted(I_sum_freeze, 0.5, I_sum, 0.5, 0, I_sum);
				addWeighted(DoLP_freeze, 0.5, DoLP, 0.5, 0, DoLP);
				addWeighted(AoLP_freeze, 0.5, AoLP, 0.5, 0, AoLP);
				addWeighted(rgb_freeze, 0.5, rgb, 0.5, 0, rgb);
				addWeighted(depth_freeze, 0.5, depth, 0.5, 0, depth);
				addWeighted(colorDepth_freeze, 0.5, colorDepth, 0.5, 0, colorDepth);
				addWeighted(normal_freeze, 0.5, normal, 0.5, 0, normal);
			}


			//Displayer操作...
			pDisplayer->Display(I_sum, DoLP, AoLP, rgb, depth, colorDepth, normal);
		}
		Sleep(20);

	}

	
	
}
void FrameManager::Capture()
{
	//这个函数是在Freeze、Continue之后，把非透物体替换成透明物体后，最后需要完成的步骤
	//保存Freeze状态的RGB-D信息，以及当前时刻的偏振信息
	if (!isFreeze)
	{
		printf_s("[-] FrameManager::Capture isFreeze is false. Press Freeze first\n");
		return;
	}
	if (!isProcessingEnabled)
	{
		printf_s("[-] FrameManager::Capture isProcessingEnabled is false. Press Continue first\n");
		return;
	}
	I_sum_capture = I_sum.clone();
	I_0_capture = I_0.clone();
	I_45_capture = I_45.clone();
	I_90_capture = I_90.clone();
	I_135_capture = I_135.clone();
	AoLP_capture = AoLP.clone();
	rgb_capture = rgb.clone();

	//深度和法线图使用Freeze保存的状态
	depth_capture = depth_freeze.clone();
	colorDepth_capture = colorDepth_freeze.clone();
	normal_capture = normal_freeze.clone();

	//TODO:Save操作


	//恢复状态
	isFreeze = false;
	pRealSenseAcquirer->EnableEmitter();//开启发射器


	
}
void FrameManager::Continue()
{
	//这个函数的作用是在Freeze之后恢复pipeline，保持Freeze时刻的帧与当前帧的叠加显示
	
	if (isFreeze == false)
	{
		printf_s("[-] FrameManager::Continue isFreeze is false. Press Freeze first\n");
		return;
	}
	isProcessingEnabled = true;
	printf_s("[+] FrameManager::Continue continue succeed\n");
}
void FrameManager::Freeze()
{
	//暂停pipeline
	isProcessingEnabled = false; 
	isFreeze = true; 
	
	//深度图已经缓存，为了不影响偏振相机，关闭RGB-D的红外发射器
	pRealSenseAcquirer->DisableEmitter();

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
	printf_s("[+] FrameManager:: Freeze freeze succeed\n");

}
void FrameManager::RegistrationInit()
{
	if (!isFreeze)
	{
		printf_s("[-] FrameManager::RegistrationInit isFreeze is false. Press Freeze first\n");
		return;
	}
	if (isProcessingEnabled)
	{
		printf_s("[-] FrameManager::RegistrationInit isProcessingEnabled is true.\n");
		return;
	}
	pRegistrar->CalculateTransform(I_sum_freeze,rgb_freeze);
		
}

void FrameManager::Save()
{
	
}


FrameManager::~FrameManager()
{
	//
	delete pPolarAcquirer;
	delete pRealSenseAcquirer;
	delete pRegistrar;
	delete pDisplayer;
}