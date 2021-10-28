
#include "cv2qt.h"

CV2Qt::CV2Qt()
{

}

// ��Matת��λQImage
QImage  CV2Qt::cvMatToQImage(const cv::Mat& inMat)
{
    switch (inMat.type())
    {
        // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_ARGB32);

        return image;
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_RGB888);

        return image.rgbSwapped();
    }
    case CV_16UC1:
    {
        cv::Mat temp;
        inMat.convertTo(temp,CV_8UC1);
        QImage image(temp.data,
            temp.cols, temp.rows,
            static_cast<int>(temp.step),
            QImage::Format_Grayscale8);//Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5
        return image;
    }


    // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_Grayscale8);//Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5
#else//���ﻹ��һ��д����������
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if (sColorTable.isEmpty())
        {
            sColorTable.resize(256);

            for (int i = 0; i < 256; ++i)
            {
                sColorTable[i] = qRgb(i, i, i);
            }
        }

        QImage image(inMat.data,
            inMat.cols, inMat.rows,
            static_cast<int>(inMat.step),
            QImage::Format_Indexed8);

        image.setColorTable(sColorTable);
#endif

        return image;
    }

    default:
       std::cout << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type()<<std::endl;
        break;
    }

    return QImage();
}

//��Matת��ΪQPixmap
QPixmap CV2Qt::cvMatToQPixmap(const cv::Mat& inMat)
{
    return QPixmap::fromImage(cvMatToQImage(inMat));
}

//��QImageת��ΪMat
cv::Mat QImageToCvMat(const QImage& inImage, bool inCloneImageData = true)
{
    switch (inImage.format())
    {
        // 8-bit, 4 channel
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat  mat(inImage.height(), inImage.width(),
            CV_8UC4,
            const_cast<uchar*>(inImage.bits()),
            static_cast<size_t>(inImage.bytesPerLine())
        );

        return (inCloneImageData ? mat.clone() : mat);
    }

    // 8-bit, 3 channel
    case QImage::Format_RGB32:
    case QImage::Format_RGB888:
    {
        if (!inCloneImageData)
        {
            std::cout << "CVS::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage" << std::endl;
        }

        QImage   swapped = inImage;

        if (inImage.format() == QImage::Format_RGB32)
        {
            swapped = swapped.convertToFormat(QImage::Format_RGB888);
        }

        swapped = swapped.rgbSwapped();

        return cv::Mat(swapped.height(), swapped.width(),
            CV_8UC3,
            const_cast<uchar*>(swapped.bits()),
            static_cast<size_t>(swapped.bytesPerLine())
        ).clone();
    }

    // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
        cv::Mat  mat(inImage.height(), inImage.width(),
            CV_8UC1,
            const_cast<uchar*>(inImage.bits()),
            static_cast<size_t>(inImage.bytesPerLine())
        );

        return (inCloneImageData ? mat.clone() : mat);
    }

    default:
        std::cout<<"CVS::QImageToCvMat() - QImage format not handled in switch:" << inImage.format()<<std::endl;
        break;
    }

    return cv::Mat();
}

//��QPixmapת��ΪMat
cv::Mat QPixmapToCvMat(const QPixmap& inPixmap, bool inCloneImageData = true)
{
    return QImageToCvMat(inPixmap.toImage(), inCloneImageData);
}
