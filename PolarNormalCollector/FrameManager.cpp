/**************************************************************************

Author: SMQ

Date:2021-10-26

Description:
��FrameManager���ʵ��

**************************************************************************/
#include "FrameManager.h"


FrameManager::FrameManager()
{
	printf_s("[<-] FrameManager Starting...\n");
	//��ʼ��������
	pPolarAcquirer = new AcquirePolarImages;
	pRealSenseAcquirer = new AcquireRealSense;
	pRegistrar = new Registrar;
	pDisplayer = new Displayer;

	//������ģ���߳�
	pRealSenseAcquirer->start();
	
	
	//���ò���
	isProcessingEnabled = true;
	
	
	


	printf_s("[+] FrameManager Started!\n");
}
void FrameManager::start()
{
	//�����߳�����void Processing()
	processing_thread =  std::thread(&FrameManager::Processing,this);

}
void FrameManager::Processing()
{
	printf_s("[<-] FrameManager::Processing is ongoing.\n");
	while (1)
	{
		if (isProcessingEnabled)
		{
			//��ȡ����ͼ�񲢽��к���
			pPolarAcquirer->ReadPolarImages();
			pPolarAcquirer->PostProcessing();

			//��ȡRealSenseͼ��
			pRealSenseAcquirer->GetPictures();
			


			//��׼����
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
			normal = depth2normal(pRealSenseAcquirer->filtered_depth_mat,pRealSenseAcquirer->GetDepthScale());//TODO�����߻�δʵ��

			//���Ӳ���
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


			//Displayer����...
			pDisplayer->Display(I_sum, DoLP, AoLP, rgb, depth, colorDepth, normal);
		}
		Sleep(20);

	}

	
	
}
void FrameManager::Capture()
{
	//�����������Freeze��Continue֮�󣬰ѷ�͸�����滻��͸������������Ҫ��ɵĲ���
	//����Freeze״̬��RGB-D��Ϣ���Լ���ǰʱ�̵�ƫ����Ϣ
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

	//��Ⱥͷ���ͼʹ��Freeze�����״̬
	depth_capture = depth_freeze.clone();
	colorDepth_capture = colorDepth_freeze.clone();
	normal_capture = normal_freeze.clone();

	//TODO:Save����


	//�ָ�״̬
	isFreeze = false;
	pRealSenseAcquirer->EnableEmitter();//����������


	
}
void FrameManager::Continue()
{
	//�����������������Freeze֮��ָ�pipeline������Freezeʱ�̵�֡�뵱ǰ֡�ĵ�����ʾ
	
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
	//��ͣpipeline
	isProcessingEnabled = false; 
	isFreeze = true; 
	
	//���ͼ�Ѿ����棬Ϊ�˲�Ӱ��ƫ��������ر�RGB-D�ĺ��ⷢ����
	pRealSenseAcquirer->DisableEmitter();

	//���浱ǰ״̬
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