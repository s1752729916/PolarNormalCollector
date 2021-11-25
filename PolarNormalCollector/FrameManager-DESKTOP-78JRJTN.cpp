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

	//创建互斥量
	hMutex = CreateMutexW(NULL, FALSE, L"FrameManagerMutex");
	if (NULL == hMutex)
	{
		printf_s("[-] FrameManager::FrameManager: CreateMutexW failed.\n");
	}




	//启动子模块线程
	pRealSenseAcquirer->start();
	
	
	//设置参数
	isProcessingEnabled = true;
	
	//数据保存设置
	AoLPPath= "";
	colorDepthPath = "";
	depthPath = "";
	DoLPPath = "";
	normalPath = "";
	I_0_Path = "";
	I_45_Path = "";
	I_90_Path = "";
	I_135_Path = "";
	I_sum_Path = "";
	rgbPath = "";

	AoLPPath = pConfig->Read("AoLPPath", AoLPPath);
	colorDepthPath = pConfig->Read("colorDepthPath", colorDepthPath);
	depthPath = pConfig->Read("depthPath", depthPath);
	DoLPPath = pConfig->Read("DoLPPath", DoLPPath);
	normalPath = pConfig->Read("normalPath", normalPath);
	I_0_Path = pConfig->Read("I_0_Path", I_0_Path);
	I_45_Path = pConfig->Read("I_45_Path", I_45_Path);
	I_90_Path = pConfig->Read("I_90_Path", I_90_Path);
	I_135_Path = pConfig->Read("I_135_Path", I_135_Path);
	I_sum_Path = pConfig->Read("I_sum_Path", I_sum_Path);
	rgbPath = pConfig->Read("rgbPath", rgbPath);


	//读取上次保存的数量
	count = 0;
	count = pConfig->Read("Count",count);//读取已保存数量

	//标定文件保存设置
	calibrationCount = 0;
	calibrationPath = pConfig->Read("CalibrationPath", calibrationPath);

	

	
	
	


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

			//对成员变量的操作都需要在拥有互斥量的情况下进行
			if (hMutex != NULL)
			{
				printf_s("[debug] Processing start\n");
				DWORD mutex_state = WaitForSingleObject(hMutex, INFINITE);  //等待互斥量
				if (mutex_state == WAIT_OBJECT_0)
				{
					//配准操作(包括去畸变)
					pRegistrar->UndistortPolarImg(pPolarAcquirer->I_sum);  //先去畸变，再进行配准
					pRegistrar->Process(pRegistrar->undistorted_polarImg);
					I_sum = pRegistrar->registered_poarImg.clone();

					pRegistrar->UndistortPolarImg(pPolarAcquirer->I_0);
					pRegistrar->Process(pRegistrar->undistorted_polarImg);
					I_0 = pRegistrar->registered_poarImg.clone();

					pRegistrar->UndistortPolarImg(pPolarAcquirer->I_45);
					pRegistrar->Process(pRegistrar->undistorted_polarImg);
					I_45 = pRegistrar->registered_poarImg.clone();

					pRegistrar->UndistortPolarImg(pPolarAcquirer->I_90);
					pRegistrar->Process(pRegistrar->undistorted_polarImg);
					I_90 = pRegistrar->registered_poarImg.clone();

					pRegistrar->UndistortPolarImg(pPolarAcquirer->I_135);
					pRegistrar->Process(pRegistrar->undistorted_polarImg);
					I_135 = pRegistrar->registered_poarImg.clone();

					pRegistrar->UndistortPolarImg(pPolarAcquirer->AoLP);
					pRegistrar->Process(pRegistrar->undistorted_polarImg);
					AoLP = pRegistrar->registered_poarImg.clone();


					pRegistrar->UndistortPolarImg(pPolarAcquirer->DoLP);
					pRegistrar->Process(pRegistrar->undistorted_polarImg);
					DoLP = pRegistrar->registered_poarImg.clone();

					rgb = pRealSenseAcquirer->raw_rgb_mat.clone();
					depth = pRealSenseAcquirer->filtered_depth_mat.clone();
					colorDepth = pRealSenseAcquirer->color_filtered_depth.clone();
					normal = depth2normal(depth, pRealSenseAcquirer->GetDepthScale());

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
						addWeighted(I_sum_freeze, 0.3, I_sum, 0.7, 0, I_sum_temp);
						addWeighted(DoLP_freeze, 0.3, DoLP, 0.7, 0, DoLP_temp);
						addWeighted(AoLP_freeze, 0.3, AoLP, 0.7, 0, AoLP_temp);
						addWeighted(rgb_freeze, 0.3, rgb, 0.7, 0, rgb_temp);
						addWeighted(depth_freeze, 0.3, depth, 0.7, 0, depth_temp);
						addWeighted(colorDepth_freeze, 0.3, colorDepth, 0.7, 0, colorDepth_temp);
						addWeighted(normal_freeze, 0.3, normal, 0.7, 0, normal_temp);
						pDisplayer->Display(I_sum_temp, DoLP_temp, AoLP_temp, rgb_temp, depth_temp, colorDepth_temp, normal_temp);

					}
					else
					{
						//直接显示原图
						pDisplayer->Display(I_sum, DoLP, AoLP, rgb, depth, colorDepth, normal);

					}

					ReleaseMutex(hMutex);
					printf_s("[debug] Processing done\n");

				}
			}





		}
		Sleep(20);

	}

	
	
}
void FrameManager::Capture()
{
	//这个函数是在Freeze、Continue之后，把非透物体替换成透明物体后，最后需要完成的步骤
	//保存Freeze状态的RGB-D信息，以及当前时刻的偏振信息



	//保证拥有互斥量的情况下进行
	if (hMutex != NULL)
	{

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

		DWORD mutex_state = WaitForSingleObject(hMutex, INFINITE);  //等待互斥量
		if (mutex_state == WAIT_OBJECT_0)
		{
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
			ReleaseMutex(hMutex);
		}
	}


	
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



	//保证拥有互斥量的情况下进行
	if (hMutex != NULL)
	{


		//printf_s("[debug] trace point0 \n");
		DWORD mutex_state = WaitForSingleObject(hMutex, INFINITE);  //等待互斥量
		//printf_s("[debug] trace point1 \n");

		if (mutex_state == WAIT_OBJECT_0)
		{
			//暂停pipeline
			isProcessingEnabled = false;
			isFreeze = true;

			//深度图已经缓存，为了不影响偏振相机，关闭RGB-D的红外发射器
			pRealSenseAcquirer->DisableEmitter();


			//保存当前状态
			//printf_s("[debug] trace point2 \n");

			I_sum_raw = pPolarAcquirer->I_sum.clone(); //对齐之前的图像保存下来
			I_sum_freeze = I_sum.clone();
			I_0_freeze = I_0.clone();
			I_45_freeze = I_45.clone();
			I_90_freeze = I_90.clone();
			I_135_freeze = I_135.clone();
			//printf_s("[debug] trace point3\n");
			AoLP_freeze = AoLP.clone();
			DoLP_freeze = DoLP.clone();
			rgb_freeze = rgb.clone();
			depth_freeze = depth.clone();
			colorDepth_freeze = colorDepth.clone();
			normal_freeze = normal.clone();
			printf_s("[+] FrameManager:: Freeze freeze succeed\n");
			ReleaseMutex(hMutex);

		}
	}



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
	//保证拥有互斥量的情况下进行
	if (hMutex != NULL)
	{
		DWORD mutex_state = WaitForSingleObject(hMutex, INFINITE);  //等待互斥量
		if (mutex_state == WAIT_OBJECT_0)
		{

			Mat polarImg;
			polarImg = CV_16UC12CV_8UC1_12Bit(I_sum_raw);
			pRegistrar->CalculateTransform(polarImg, rgb_freeze);

			ReleaseMutex(hMutex);
		}
	}

	
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
	path = I_sum_Path + path;
	Save16UC1(path,I_sum_capture);
	

	path = str_format("%09d-I-0.png", count);
	path = I_0_Path + path;
	Save16UC1(path, I_0_capture);

	path = str_format("%09d-I-45.png", count);
	path = I_45_Path + path;
	Save16UC1(path, I_45_capture);


	path = str_format("%09d-I-90.png", count);
	path = I_90_Path + path;
	Save16UC1(path, I_90_capture);


	path = str_format("%09d-I-135.png", count);
	path = I_135_Path + path;
	Save16UC1(path, I_135_capture);

	path = str_format("%09d-depth.png", count);
	path = depthPath + path;
	Save16UC1(path, depth_capture);

	//保存CV_8UC3格式
	path = str_format("%09d-RGB.png",count);
	path = rgbPath + path;
	Save8UC3(path,rgb_capture);

	path = str_format("%09d-color-depth.png", count);
	path = colorDepthPath + path;
	Save8UC3(path, colorDepth_capture);

	//保存CV_32FC1格式(.exr文件)：AoLP，DoLP
	path = str_format("%09d-AoLP.exr", count);
	path = AoLPPath + path;
	Save32FC1(path, AoLP_capture);

	path = str_format("%09d-DoLP.exr", count);
	path = DoLPPath + path;
	Save32FC1(path, DoLP_capture);

	//保存CV_32FC3格式(.exr文件)：normals
	path = str_format("%09d-normals.exr", count);
	path = normalPath + path;
	cvtColor(normal_capture, normal_capture, CV_BGR2RGB);
	Save32FC3(path, normal_capture);


	printf_s("[+] FrameManager::Save: Save %09d succeed.\n", count);
	//更改计数
	count += 1;
	pConfig->Add("Count",count);

	
}
void FrameManager::SaveCalibration()
{
	if (!isFreeze)
	{
		printf_s("[-] FrameManager::SaveCalibration: isFreeze is false. Press Freeze first\n");
	}
	String path;
	
	//保存极化图像
	path = str_format("%09d-Calibration-Polar.png", calibrationCount);
	path = calibrationPath + path;
	Mat polarImg;
	polarImg = CV_16UC12CV_8UC1_12Bit(I_sum_raw);//12位极化图像转换成8位图像
	Save8UC1(path, polarImg);


	//保存RGB图像
	path = str_format("%09d-Calibration-RGB.png", calibrationCount);
	path = calibrationPath + path;
	Save8UC3(path,rgb_freeze);


	printf_s("[+] FrameManager::SaveCalibration: Save %09d succeed.\n", calibrationCount);

	calibrationCount += 1;





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