#include "utils.h"
Mat CV_16UC1ToCV_8UC1(Mat& input)
{
	//16位转8位，像素的位数必须是16位，例如深度图;偏振图像的格式虽然为16UC1,
	if (input.type() != CV_16UC1)
	{
		printf_s("utils::CV_16UC1ToCV_8UC1 input type is not CV_16UC1\n");
		return input;
	}

	Mat out;
	out = input / 257;  
	out.convertTo(out, CV_8UC1);


	//返回函数内部定义的Mat是可行的
	//Mat由计数器来控制释放与否，因此函数结束后不会清除函数内部定义的Mat
	return out.clone();

}
Mat CV_16UC12CV_8UC1_12Bit(Mat & input)
{
	//12位的转换，用于偏振图像从CV_16UC1转换成CV_8UC1的格式
	//保证亮度不变的格式转换
	if (input.type() != CV_16UC1)
	{
		printf_s("utils::CV_16UC12CV_8UC1_12Bit input type is not CV_16UC1\n");
		return input;
	}

	Mat out;
	out = input / 16;  //图片实际上是12位的，因此只需要除16即可
	out.convertTo(out, CV_8UC1);


	//返回函数内部定义的Mat是可行的
	//Mat由计数器来控制释放与否，因此函数结束后不会清除函数内部定义的Mat
	return out.clone();

}


Mat CV_16UC1ToCV_8UC3(Mat& input)
{

	if (input.type() != CV_16UC1)
	{
		printf_s("utils::CV_16UC1ToCV_8UC3 input type is not CV_16UC1\n");
		return input;
	}
	Mat _8UC1;
	Mat _8UC3;
	_8UC1 = CV_16UC1ToCV_8UC1(input);
	cvtColor(_8UC1, _8UC3, COLOR_GRAY2RGB);//转换成8UC3
	return _8UC3;
	

}
Mat CV_32FC1ToCV_8UC3(Mat& input)
{
	if (input.type() != CV_32FC1)
	{
		printf_s("utils::CV_32FC1ToCV_8UC3 input type is not CV_32FC1\n");
		return input;
	}
	Mat _8UC1;
	Mat _8UC3;
	input.convertTo(_8UC1,CV_8UC1,255.0);
	cvtColor(_8UC1, _8UC3, COLOR_GRAY2RGB);//转换成8UC3
	return _8UC3;
}
Mat CV_32FC3ToCV_8UC3(Mat& input)
{
	//主要是用于法线图的显示
	//法线图输入格式为CV_32FC3，每个通道值在-1到1之间，所以需要将-1到1之间转换到0到255进显示
	if (input.type() != CV_32FC3)
	{
		printf_s("utils::CV_32FC3ToCV_8UC3 input type is not CV_32FC3\n");
		return input;
	}
	Mat _8UC3;
	//imshow("normals", input);
	waitKey(0);
	input.convertTo(_8UC3, CV_8UC3, 127.5,127.5);
	Mat _rgb;
	cvtColor(_8UC3,_rgb,COLOR_BGR2RGB);
	return _rgb;
}

Mat depth2normal(Mat& depth, float depth_scale)
{
	/*

	*Summary: 将深度图转换为法线图

	*Parameters:

	*     depth: 格式为CV_16UC1的深度图，由RealSenseAcquirer输出的数据

	*Return : 格式为CV_32FC3的法线图，三个通道代表(x,y,z),范围为(-1,1)

	*/

	Mat norm = Mat::zeros(depth.size(),CV_32FC3);
	for (int x = 1; x < depth.cols - 1; ++x)
	{
		for (int y = 1; y < depth.rows - 1; ++y)
		{
			//printf_s("depth: %d\n",depth.at<uint16_t>(y,x));
			float dx = depth.at<uint16_t>(y, x) - depth.at<uint16_t>(y, x - 1);
			float dy = depth.at<uint16_t>(y, x) - depth.at<uint16_t>(y - 1, x);
			if (depth.at<uint16_t>(y, x) == 0)
			{
				//无效深度值的法向量设为(-1,-1,-1)，这样的话在RGB深度图中是(0,0,0)
				norm.at<Vec3f>(y, x) = Vec3f(-1,-1,-1);

			}
			else
			{
				Vec3f d(dx, -dy, 1);
				norm.at<Vec3f>(y, x) = normalize(d);
			}

			
		}
	}



	return norm;
	



}


void Save16UC1(String& path,Mat& img)
{
	if (img.type() != CV_16UC1)
	{
		printf_s("[-] utils::Save16UC1 Failed: Type of img is not CV_16UC1.\n");
		return;
	}
	imwrite(path, img);
	
}

void Save8UC3(String& path, Mat& img)
{
	if (img.type() != CV_8UC3)
	{
		printf_s("[-] utils::CV_8UC3 Failed: Type of img is not CV_8UC3.\n");
		return;
	}
	imwrite(path, img);

}

void Save32FC1(String& path, Mat& img)
{
	if (img.type() != CV_32FC1)
	{
		printf_s("[-] utils::CV_32FC1 Failed: Type of img is not CV_32FC1.\n");
		return;
	}
	imwrite(path, img);
}
void Save8UC1(String& path, Mat& img)
{
	if (img.type() != CV_8UC1)
	{
		printf_s("[-] utils::Save8UC1 Failed: Type of img is not 8UC1.\n");
		return;
	}
	imwrite(path, img);
}

void Save32FC3(String& path, Mat& img)
{
	if (img.type() != CV_32FC3)
	{
		printf_s("[-] utils::CV_32FC3 Failed: Type of img is not CV_32FC3.\n");
		return;
	}
	imwrite(path, img);
}