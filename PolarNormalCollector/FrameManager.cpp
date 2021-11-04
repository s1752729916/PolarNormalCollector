/**************************************************************************

Author: SMQ

Date:2021-10-26

Description:
对FrameManager类的实现

**************************************************************************/
#include "FrameManager.h"
#include <vector>

FrameManager::FrameManager()
{
	printf_s("[<-] FrameManager Starting...\n");
	//初始化各个类
	pConfig = new Config("config.txt");


	pPolarAcquirer = new AcquirePolarImages;
	pRealSenseAcquirer = new AcquireRealSense;
	pRegistrar = new Registrar(pConfig);
	pDisplayer = new Displayer;

	//启动子模块线程
	pRealSenseAcquirer->start();
	
	
	//设置参数
	isProcessingEnabled = true;
	rootSave = "";
	count = 0;
	rootSave = pConfig->Read("SaveRoot",rootSave);//读取保存路径
	count = pConfig->Read("Count",count);//读取已保存数量

	
	
	


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
				//需要缓存叠加显示
				Mat I_sum_temp;
				Mat AoLP_temp;
				Mat DoLP_temp;
				Mat rgb_temp;
				Mat depth_temp;
				Mat colorDepth_temp;
				Mat normal_temp;
				addWeighted(I_sum_freeze, 0.5, I_sum, 0.5, 0, I_sum_temp);
				addWeighted(DoLP_freeze, 0.5, DoLP, 0.5, 0, DoLP_temp);
				addWeighted(AoLP_freeze, 0.5, AoLP, 0.5, 0, AoLP_temp);
				addWeighted(rgb_freeze, 0.5, rgb, 0.5, 0, rgb_temp);
				addWeighted(depth_freeze, 0.5, depth, 0.5, 0, depth_temp);
				addWeighted(colorDepth_freeze, 0.5, colorDepth, 0.5, 0, colorDepth_temp);
				addWeighted(normal_freeze, 0.5, normal, 0.5, 0, normal_temp);
				pDisplayer->Display(I_sum_temp, DoLP_temp, AoLP_temp, rgb_temp, depth_temp, colorDepth_temp, normal_temp);

			}
			else
			{
				//直接显示原图
				pDisplayer->Display(I_sum, DoLP, AoLP, rgb, depth, colorDepth, normal);

			}


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
	//需要捕捉当前时刻状态的图片
	I_sum_capture = I_sum.clone();
	I_0_capture = I_0.clone();
	I_45_capture = I_45.clone();
	I_90_capture = I_90.clone();
	I_135_capture = I_135.clone();
	AoLP_capture = AoLP.clone();
	DoLP_capture = DoLP.clone();
	rgb_capture = rgb.clone();

	//深度和法线图使用Freeze保存的状态
	depth_capture = depth_freeze.clone();
	colorDepth_capture = colorDepth_freeze.clone();
	normal_capture = normal_freeze.clone();

	//TODO:Save操作


	//恢复状态
	isFreeze = false;
	pRealSenseAcquirer->EnableEmitter();//开启发射器

	printf_s("[+] FrameManager::Capture capture succeed\n");

	
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
	I_sum_raw = pPolarAcquirer->I_sum.clone();
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
	Mat polarImg;
	I_sum_raw.convertTo(polarImg,CV_8UC1);
	imshow("polarImg", polarImg);
	waitKey(0);
	pRegistrar->CalculateTransform(polarImg,rgb_freeze);
	
}

void FrameManager::Save()
{
	if (I_sum_capture.empty() || I_0_capture.empty() || I_45_capture.empty() || I_90_capture.empty() || I_135_capture.empty()
		|| AoLP_capture.empty() || DoLP_capture.empty() || rgb_capture.empty() || depth_capture.empty() || colorDepth_capture.empty() || normal_capture.empty()
		)
	{
		//查看捕捉的图像是否有空图
		printf_s("[-] FrameManager::Save Failed: At least one captured image is empty.\n");
		return;
	}

	//读取路径与参数
	String path;
	//int count = 0;  //TODO: 这个count后续需要从config文件中读取，每保存一个文件自动加一

	//先保存格式为CV_16UC1的图片,I_sum,I_0,I_45,I_90,I_135,depth

	path = str_format("%09d-I-Sum.png",count);
	path = rootSave + path;
	Save16UC1(path,I_sum_capture);
	

	path = str_format("%09d-I-0.png", count);
	path = rootSave + path;
	Save16UC1(path, I_0_capture);

	path = str_format("%09d-I-45.png", count);
	path = rootSave + path;
	Save16UC1(path, I_45_capture);


	path = str_format("%09d-I-90.png", count);
	path = rootSave + path;
	Save16UC1(path, I_90_capture);


	path = str_format("%09d-I-135.png", count);
	path = rootSave + path;
	Save16UC1(path, I_135_capture);

	path = str_format("%09d-depth.png", count);
	path = rootSave + path;
	Save16UC1(path, depth_capture);

	//保存CV_8UC3格式
	path = str_format("%09d-RGB.png",count);
	path = rootSave + path;
	Save8UC3(path,rgb_capture);

	path = str_format("%09d-color-depth.png", count);
	path = rootSave + path;
	Save8UC3(path, colorDepth_capture);

	//保存CV_32FC1格式(.exr文件)：AoLP，DoLP
	path = str_format("%09d-AoLP.exr", count);
	path = rootSave + path;
	Save32FC1(path, AoLP_capture);

	path = str_format("%09d-DoLP.exr", count);
	path = rootSave + path;
	Save32FC1(path, DoLP_capture);

	//保存CV_32FC3格式(.exr文件)：normals
	path = str_format("%09d-normals.exr", count);
	path = rootSave + path;
	cvtColor(normal_capture, normal_capture, CV_BGR2RGB);
	Save32FC3(path, normal_capture);

	//更改计数
	count += 1;
	pConfig->Add("Count",count);
	printf_s("[+] FrameManager::Save succeed.\n");

	
}


FrameManager::~FrameManager()
{
	//
	delete pPolarAcquirer;
	delete pRealSenseAcquirer;
	delete pRegistrar;
	delete pDisplayer;
	delete pConfig;
}