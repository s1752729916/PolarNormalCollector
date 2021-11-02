/**************************************************************************

Author: SMQ

Date:2021-10-25

Description:
这个类是帧管理器，完成整个pipeline，统筹偏振相机和深度相机视频流的读取、配准、显示等工作

该类共有四个主要函数：
1、RegistrationInit()//用于计算透视矩阵，该函数需与Freeze()配合使用
2、Processing()完成读取和配准任务以及显示接口的输出三个子任务，该函数应当运行在独立的线程中，由各标志位控制各子任务的处理与否
3、Freeze() 冻结当前帧，通过设置标志位来暂停Processing()中各个子任务的运行，并保存当前帧的状态
4、Capture() 主要是完成保存等工作，需要与Freeze()配合
**************************************************************************/
#ifndef FrameManager_H
#define FrameManager_H
#include "PolarImagesAcquistion.h"
#include "RealSenseAcquisition.h"
#include "Registration.h"
#include "Displayer.h"
class FrameManager
{
public:
	FrameManager();
	~FrameManager();
	void Freeze();
	void Continue();//Freeze后叠加显示Freeze状态与当前状态
	void Capture();
	void RegistrationInit();
	void Processing();
	void Save();
	void start();//开启帧管理器pipeline的线程


private:
	//参数
	bool isProcessingEnabled;
	bool isFreeze;//这个标签位用来控制是否进入了Freeze状态，Freeze函数将该标签置真，Processing或者Capture函数将该函数置位假



private:
	//类指针
	AcquirePolarImages* pPolarAcquirer;
	AcquireRealSense* pRealSenseAcquirer;
	Registrar* pRegistrar;
private:
	std::thread processing_thread; //帧管理器pipeline所在线程句柄
public://需要暴露给MainWindows连接信号槽
	Displayer* pDisplayer;

private:
	//视频流中的各图像缓存区
	Mat I_sum;
	Mat I_0;
	Mat I_45;
	Mat I_90;
	Mat I_135;
	Mat AoLP;
	Mat DoLP;
	Mat rgb;
	Mat depth;
	Mat colorDepth;
	Mat normal;
private:
	//freeze保存缓冲区
	Mat I_sum_freeze; //CV_16UC1
	Mat I_0_freeze;   //CV_16UC1
	Mat I_45_freeze;  //CV_16UC1
	Mat I_90_freeze;  //CV_16UC1
	Mat I_135_freeze; //CV_16UC1
	Mat AoLP_freeze;  //CV_32FC1
	Mat DoLP_freeze;  //CV_32FC1
	Mat rgb_freeze;   //CV_8UC3
	Mat depth_freeze; //CV_16UC1
	Mat colorDepth_freeze; //CV_8UC3
	Mat normal_freeze;     //CV_32FC3
private:
	//capture缓冲区
	Mat I_sum_capture;
	Mat I_0_capture;
	Mat I_45_capture;
	Mat I_90_capture;
	Mat I_135_capture;
	Mat AoLP_capture;
	Mat DoLP_capture;
	Mat rgb_capture;
	Mat depth_capture;
	Mat colorDepth_capture;
	Mat normal_capture;

};

#endif 
