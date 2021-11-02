/**************************************************************************

Author: SMQ

Date:2021-10-25

Description:
该类用于显示图像，应包含接口::Display
向下与FrameManager进行通信，向上与Qt进行通信
**************************************************************************/
#ifndef Displayer_H
#define Displayer_H
#include <QtCore/qobject.h>
#include <opencv2/opencv.hpp>
#include "cv2qt.h"
#include "utils.h"
using namespace cv;
class Displayer: public QObject
{
	Q_OBJECT
signals:
	void displayImgs(); 

public:
	void Display(Mat& Intensity, Mat&DoLP, Mat& AoLP, Mat&RGB, Mat&Depth,Mat& colorDepth, Mat& Normal)
	{
		//这个函数里把Mat类型转成Qt中能显示的QImage
		//Polar过来的图像全部为16位的，但Qt无法显示16位图片，所以在转成QImage前先全部转换成8位,再转换成8UC3进行显示，因为Qt对8UC1支持不好，容易崩溃
		CV2Qt converter;

		//Intensity
	
		Intensity_qt = converter.cvMatToQImage(CV_16UC1ToCV_8UC3(Intensity));

		//Intensity_bright
		Mat bright;
		Mat bright_8UC3;
		Intensity.convertTo(bright,CV_8UC1);
		cvtColor(bright,bright_8UC3,COLOR_GRAY2RGB);
		Intensity_bright_qt = converter.cvMatToQImage(bright_8UC3);
		DoLP_qt = converter.cvMatToQImage(CV_32FC1ToCV_8UC3(DoLP));
		AoLP_qt = converter.cvMatToQImage(CV_32FC1ToCV_8UC3(AoLP));

		//RealSense的图像都是8位，不需要再转换了
		RGB_qt = converter.cvMatToQImage(RGB);

		Depth_qt = converter.cvMatToQImage(CV_16UC1ToCV_8UC3(Depth));
		colorDepth_pt = converter.cvMatToQImage(colorDepth);


		Normal_qt = converter.cvMatToQImage(CV_32FC3ToCV_8UC3(Normal));


		//转换完成，发送信号进行显示

		emit displayImgs();
	}
public: //需要暴露给槽函数
	QImage Intensity_qt;
	QImage Intensity_bright_qt;
	QImage DoLP_qt;
	QImage AoLP_qt;
	QImage RGB_qt;
	QImage Depth_qt;
	QImage colorDepth_pt;
	QImage Normal_qt;



};
#endif Displayer_H