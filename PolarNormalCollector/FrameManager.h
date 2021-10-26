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
#include "PolarImagesAcquistion.h"
#include "RealSenseAcquisition.h"
#include "Registration.h"

class FrameManager
{
public:
	FrameManager();
	~FrameManager();
	void Freeze();
	void RegistrationInit();
	void Processing();




private:
	//参数
	bool isProcessingEnabled;




private:
	//类指针
	AcquirePolarImages* pPolarAcquirer;
	AcquireRealSense* pRealSenseAcquirer;
	Registrar* pRegistrar;
	//Displayer* pDisplayer;//TODO: Displayer

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
	Mat I_sum_freeze;
	Mat I_0_freeze;
	Mat I_45_freeze;
	Mat I_90_freeze;
	Mat I_135_freeze;
	Mat AoLP_freeze;
	Mat DoLP_freeze;
	Mat rgb_freeze;
	Mat depth_freeze;
	Mat colorDepth_freeze;
	Mat normal_freeze;
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
