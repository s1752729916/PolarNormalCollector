/**************************************************************************

Author: SMQ

Date:2021-10-25

Description:
�������֡���������������pipeline��ͳ��ƫ���������������Ƶ���Ķ�ȡ����׼����ʾ�ȹ���

���๲���ĸ���Ҫ������
1��RegistrationInit()//���ڼ���͸�Ӿ��󣬸ú�������Freeze()���ʹ��
2��Processing()��ɶ�ȡ����׼�����Լ���ʾ�ӿڵ�������������񣬸ú���Ӧ�������ڶ������߳��У��ɸ���־λ���Ƹ�������Ĵ������
3��Freeze() ���ᵱǰ֡��ͨ�����ñ�־λ����ͣProcessing()�и�������������У������浱ǰ֡��״̬
4��Capture() ��Ҫ����ɱ���ȹ�������Ҫ��Freeze()���
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
	void Continue();//Freeze�������ʾFreeze״̬�뵱ǰ״̬
	void Capture();
	void RegistrationInit();
	void Processing();
	void Save();
	void start();//����֡������pipeline���߳�


private:
	//����
	bool isProcessingEnabled;
	bool isFreeze;//�����ǩλ���������Ƿ������Freeze״̬��Freeze�������ñ�ǩ���棬Processing����Capture�������ú�����λ��



private:
	//��ָ��
	AcquirePolarImages* pPolarAcquirer;
	AcquireRealSense* pRealSenseAcquirer;
	Registrar* pRegistrar;
private:
	std::thread processing_thread; //֡������pipeline�����߳̾��
public://��Ҫ��¶��MainWindows�����źŲ�
	Displayer* pDisplayer;

private:
	//��Ƶ���еĸ�ͼ�񻺴���
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
	//freeze���滺����
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
	//capture������
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
