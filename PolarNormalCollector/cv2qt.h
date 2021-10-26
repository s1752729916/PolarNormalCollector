#ifndef CV2QT_H
#define CV2QT_H

#include <QtWidgets/QMainWindow>

#include "opencv2/opencv.hpp"

class CV2Qt
{
public:
    CV2Qt();
    QImage  cvMatToQImage(const cv::Mat& inMat);
    QPixmap cvMatToQPixmap(const cv::Mat& inMat);
};

#endif // CV2QT_H
