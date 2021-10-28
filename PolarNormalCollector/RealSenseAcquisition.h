#pragma once
/**************************************************************************

Author: SMQ

Date:2021-10-21

Description:
1、这个类主要用于从RealSense D435i深度相机中读取深度；
2、除了完成读取功能外，还完成深度图的一些后处理，包括空间、时间滤波以及孔洞填补等操作

**************************************************************************/
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <Windows.h>


using namespace cv;
enum class direction
{
	//对齐到哪里
	to_depth,
	to_color
};
class AcquireRealSense
{
public:
	AcquireRealSense();
	~AcquireRealSense();
	void GetPictures(); //获取最新的RBG图和深度图(1帧)，这个函数应当由主线程进行调用
	void Processing();//pipeline,用于进行彩色图和深度图的配准和后处理的操作，这个函数应当单独在一个线程进行处理
	void start(); //用于开启Processing线程的函数
	Mat Frame2Mat(const rs2::frame& frame);//将frame转换为Mat格式

public:
	//Mat格式图像,在构造函数中初始化，大小与width，height相同
	Mat raw_depth_mat;  //深度图格式为
	Mat filtered_depth_mat;
	Mat raw_rgb_mat;
	Mat color_filtered_depth;
	Mat color_raw_depth;

private:
	int width; //分辨率
	int height; //分辨率
	rs2::pipeline pipe;
	rs2::config cfg;
	direction align_direction;
	
	//后处理滤波器设置
	bool isPostProcessingEnabled; //是否进行后处理
	rs2::threshold_filter thr_filter;   // Threshold  - removes values outside recommended range
	rs2::spatial_filter spat_filter;    // Spatial    - edge-preserving spatial smoothing
	rs2::temporal_filter temp_filter;   // Temporal   - reduces temporal noise

	//后处理之后的队列和原始队列(深度图),为了保持rgb和深度图的一致，需要同时push到队列中
	rs2::frame_queue raw_depth_queue; 
	rs2::frame_queue raw_rgb_queue;
	rs2::frame_queue filtered_depth_queue;//处理之后的深度图将存放在这个队列当中
	
	std::thread processing_thread; //线程句柄必须保存
};

//测试代码：
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

