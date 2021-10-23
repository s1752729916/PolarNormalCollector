/**************************************************************************

Author: SMQ

Date:2021-10-21

Description:
��AcquireRealSense���ʵ��

**************************************************************************/
#include "RealSenseAcquisition.h"

AcquireRealSense::AcquireRealSense()
{
	//���캯���Ĺ���
	//1�������������
	//2�����ú����˲���

	//-- 1�����÷ֱ����ɲ���
	width = 640; 
	height = 480;
	cfg.enable_stream(RS2_STREAM_COLOR,width, height,RS2_FORMAT_RGB8,30); //uint8 RBG��ʽ, 30֡
	cfg.enable_stream(RS2_STREAM_DEPTH, width, height, RS2_FORMAT_Z16, 30); //16λ���ֵ����λ����

	//-- 2��������׼����
	align_direction = direction::to_color;//���뵽RGB���

	//-- 3�����ú����˲���
	thr_filter.set_option(RS2_OPTION_MIN_DISTANCE,0.5);//��С����
	thr_filter.set_option(RS2_OPTION_MAX_DISTANCE, 4);//������

	//�ռ��˲���
	spat_filter.set_option(RS2_OPTION_FILTER_MAGNITUDE,2);
	spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA,0.5);
	spat_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 20);
	spat_filter.set_option(RS2_OPTION_HOLES_FILL,1);

	//ʱ���˲���
	temp_filter.set_option(RS2_OPTION_HOLES_FILL, 2); //��ֵ2��ʾ��ȥ��֡����2�����ھ������ն�
	temp_filter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.4);
	temp_filter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 20);

	//-- 4����ʼ��Mat
	raw_depth_mat = Mat::zeros(height,width,CV_16UC1);
	raw_rgb_mat = Mat::zeros(height, width, CV_8UC3);
	filtered_depth_mat = Mat::zeros(height, width, CV_16UC1);
	color_depth = Mat::zeros(height, width, CV_8UC3);


	//-- 5����������
	isPostProcessingEnabled = true;
}

AcquireRealSense::~AcquireRealSense()
{
	pipe.stop();
}
void AcquireRealSense::Processing()
{
	//��pipeline���ȴ�֡������
	try
	{
		pipe.start(cfg);
	}
	catch (const rs2::error& e)
	{
		std::cerr << "[-] AcquireRealSense::Processing Error: RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
		return;
	}
	catch (const std::exception& e)
	{
		std::cerr <<"[-] AcquireRealSense::Processing Exception: " << e.what() << std::endl;
		return ;
	}
	
	printf_s("[+] AcquireRealSense::Processing pipeline started succeed\n");
	while (1)
	{
		//�������ȡ֡
		rs2::frameset frames = pipe.wait_for_frames();

		//�������
		rs2::align align_to_depth(RS2_STREAM_DEPTH);
		rs2::align align_to_color(RS2_STREAM_COLOR);
		if (align_direction == direction::to_depth)
		{
			// Align all frames to depth viewport
			frames = align_to_depth.process(frames);
		}
		else
		{
			// Align all frames to color viewport
			frames = align_to_color.process(frames);
		}
		//��ȡ֡

		rs2::depth_frame raw_depth = frames.get_depth_frame();
		rs2::frame raw_rgb = frames.get_color_frame();
		if (!raw_depth)
			continue;
		rs2::frame depth_filtered = raw_depth; // Does not copy the frame, only adds a reference
		
		//�����˲���
		if (isPostProcessingEnabled)
		{
			depth_filtered = thr_filter.process(depth_filtered);
			depth_filtered = spat_filter.process(depth_filtered);
			depth_filtered = temp_filter.process(depth_filtered);
		}
		
		
		//������ͼ�񶼼��뵽���е���ȥ
		filtered_depth_queue.enqueue(depth_filtered);
		raw_depth_queue.enqueue(raw_depth);
		raw_rgb_queue.enqueue(raw_rgb);
		

	}

	
}
void AcquireRealSense::GetPictures()
{
	//��ȡ���µ�RBGͼ�����ͼ(1֡)���������Ӧ�������߳̽��е���
	//���е�ͼ����������Mat������ȥ��ȡ
	rs2::frame raw_depth_frame;
	rs2::frame raw_rgb_frame;
 	rs2::frame filtered_depth_frame;
	try
	{
		if (raw_depth_queue.poll_for_frame(&raw_depth_frame))
		{
			raw_depth_mat = Frame2Mat(raw_depth_frame).clone();
		}
		if (raw_rgb_queue.poll_for_frame(&raw_rgb_frame))
		{
			raw_rgb_mat = Frame2Mat(raw_rgb_frame).clone();

		}
		if (filtered_depth_queue.poll_for_frame(&filtered_depth_frame))
		{
			filtered_depth_mat = Frame2Mat(filtered_depth_frame).clone(); //��λӦ����mm

			rs2::colorizer color_map;
			color_depth = Frame2Mat(rs2::depth_frame(filtered_depth_frame).apply_filter(color_map)).clone();
		}
	}
	catch (const rs2::error& e)
	{
		std::cerr << "[-] AcquireRealSense::GetPictures Error: RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
		return;
	}
	catch (const std::exception& e)
	{
		std::cerr << "[-] AcquireRealSense::GetPictures Exception: " << e.what() << std::endl;
		return;
	}




}








//����д�Ƿ����ڴ�й¶�Ŀ��ܣ�
Mat AcquireRealSense::Frame2Mat(const rs2::frame& frame)
{
	auto vf = frame.as<rs2::video_frame>();
	const int w = vf.get_width();
	const int h = vf.get_height();
	if (frame.get_profile().format() == RS2_FORMAT_BGR8)
	{
		return cv::Mat(cv::Size(w, h), CV_8UC3, (void*)frame.get_data(), cv::Mat::AUTO_STEP);
	}
	else if (frame.get_profile().format() == RS2_FORMAT_RGB8)
	{
		auto r = cv::Mat(cv::Size(w, h), CV_8UC3, (void*)frame.get_data(), cv::Mat::AUTO_STEP);
		cv::cvtColor(r, r, CV_RGB2BGR);
		return r;
	}
	else if (frame.get_profile().format() == RS2_FORMAT_Z16)
	{
		return cv::Mat(cv::Size(w, h), CV_16UC1, (void*)frame.get_data(), cv::Mat::AUTO_STEP);

	}
	else if (frame.get_profile().format() == RS2_FORMAT_Y8)
	{
		return cv::Mat(cv::Size(w, h), CV_8UC1, (void*)frame.get_data(), cv::Mat::AUTO_STEP);

	}

}