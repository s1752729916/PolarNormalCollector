#include "MainWindow.h"
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //��ʼ��
    pFrameManager = new FrameManager;
    pFrameManager->start();//����֡������
    
    //��ʼ���źŲ�
    InitConnects();
}

void MainWindow::InitConnects()
{
    connect(pFrameManager->pDisplayer, &Displayer::displayImgs, [&]()
        {
            //�����������������õķ�ʽʹ��MainWindows�����ڵı���
            //ͨ��this->pFrameManager->Displayer->Imgs���õ�������Ҫ��ͼ��
            
            if (!pFrameManager->pDisplayer->Intensity_qt.isNull())
            {
                ui.label_Intensity->setPixmap(QPixmap::fromImage(pFrameManager->pDisplayer->Intensity_qt));
                ui.label_Intensity->show();
            }
            if (!pFrameManager->pDisplayer->Intensity_bright_qt.isNull())
            {
                ui.label_Intensity_bright->setPixmap(QPixmap::fromImage(pFrameManager->pDisplayer->Intensity_bright_qt));
                ui.label_Intensity_bright->show();
            }

            if (!pFrameManager->pDisplayer->AoLP_qt.isNull())
            {
                ui.label_AoLP->setPixmap(QPixmap::fromImage(pFrameManager->pDisplayer->AoLP_qt));
                ui.label_AoLP->show();
            }
            if (!pFrameManager->pDisplayer->DoLP_qt.isNull())
            {
                ui.label_DoLP->setPixmap(QPixmap::fromImage(pFrameManager->pDisplayer->DoLP_qt));
                ui.label_DoLP->show();
            }

            if (!pFrameManager->pDisplayer->RGB_qt.isNull())
            {
                ui.label_RGB->setPixmap(QPixmap::fromImage(pFrameManager->pDisplayer->RGB_qt));
                ui.label_RGB->show();
            }

            if (!pFrameManager->pDisplayer->Depth_qt.isNull())
            {
                ui.label_Depth->setPixmap(QPixmap::fromImage(pFrameManager->pDisplayer->Depth_qt));
                ui.label_Depth->show();
            }
            
            if (!pFrameManager->pDisplayer->colorDepth_pt.isNull())
            {
                ui.label_colorDepth->setPixmap(QPixmap::fromImage(pFrameManager->pDisplayer->colorDepth_pt));
                ui.label_colorDepth->show();
            }

            if (!pFrameManager->pDisplayer->Normal_qt.isNull())
            {
                ui.label_Normal->setPixmap(QPixmap::fromImage(pFrameManager->pDisplayer->Normal_qt));
                ui.label_Normal->show();
            }
            update();












        }
    );

}

