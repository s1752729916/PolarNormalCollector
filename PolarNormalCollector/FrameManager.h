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
	//����
	bool isProcessingEnabled;




private:
	//��ָ��
	AcquirePolarImages* pPolarAcquirer;
	AcquireRealSense* pRealSenseAcquirer;
	Registrar* pRegistrar;
	//Displayer* pDisplayer;//TODO: Displayer

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
