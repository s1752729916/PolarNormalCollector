#pragma once
/**************************************************************************

Author: SMQ

Date:2021-10-21

Description:
1���������Ҫ���ڴ�RealSense D435i�������ж�ȡ��ȣ�
2��������ɶ�ȡ�����⣬��������ͼ��һЩ���������ռ䡢ʱ���˲��Լ��׶���Ȳ���

**************************************************************************/
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <Windows.h>


using namespace cv;
enum class direction
{
	//���뵽����
	to_depth,
	to_color
};
class AcquireRealSense
{
public:
	AcquireRealSense();
	~AcquireRealSense();
	void GetPictures(); //��ȡ���µ�RBGͼ�����ͼ(1֡)���������Ӧ�������߳̽��е���
	void Processing();//pipeline,���ڽ��в�ɫͼ�����ͼ����׼�ͺ���Ĳ������������Ӧ��������һ���߳̽��д���
	void start(); //���ڿ���Processing�̵߳ĺ���
	Mat Frame2Mat(const rs2::frame& frame);//��frameת��ΪMat��ʽ

public:
	//Mat��ʽͼ��,�ڹ��캯���г�ʼ������С��width��height��ͬ
	Mat raw_depth_mat;  //���ͼ��ʽΪ
	Mat filtered_depth_mat;
	Mat raw_rgb_mat;
	Mat color_filtered_depth;
	Mat color_raw_depth;

private:
	int width; //�ֱ���
	int height; //�ֱ���
	rs2::pipeline pipe;
	rs2::config cfg;
	direction align_direction;
	
	//�����˲�������
	bool isPostProcessingEnabled; //�Ƿ���к���
	rs2::threshold_filter thr_filter;   // Threshold  - removes values outside recommended range
	rs2::spatial_filter spat_filter;    // Spatial    - edge-preserving spatial smoothing
	rs2::temporal_filter temp_filter;   // Temporal   - reduces temporal noise

	//����֮��Ķ��к�ԭʼ����(���ͼ),Ϊ�˱���rgb�����ͼ��һ�£���Ҫͬʱpush��������
	rs2::frame_queue raw_depth_queue; 
	rs2::frame_queue raw_rgb_queue;
	rs2::frame_queue filtered_depth_queue;//����֮������ͼ�������������е���
	
	std::thread processing_thread; //�߳̾�����뱣��
};

//���Դ��룺
//AcquireRealSense captcher;
//std::thread processing_thread([&]()
//	{
//
//		captcher.Processing();
//
//	});
//
//while (1)
//{
//	captcher.GetPictures();
//	imshow("color_raw_depth", captcher.color_raw_depth);
//	imshow("color_filtered_depth", captcher.color_filtered_depth);
//	imshow("raw_rgb", captcher.raw_rgb_mat);
//	imshow("filtered_depth", captcher.filtered_depth_mat);
//
//	Mat syn_img = Mat::zeros(640, 480, CV_8UC3);
//	addWeighted(captcher.color_filtered_depth, 0.5, captcher.raw_rgb_mat, 0.5, 0.0, syn_img);
//
//	imshow("syn", syn_img);
//
//	waitKey(0);
//}

