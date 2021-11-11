#include "MainWindow.h"
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //初始化
    pFrameManager = new FrameManager;
    pFrameManager->start();//启动帧管理器
    
    //打开Displayer的互斥量，因为FrameManager已经创建完成，所以互斥量已经创建
    hMutex = OpenMutexW(MUTEX_ALL_ACCESS, NULL, L"DisplayerMutex");
    if (NULL == hMutex)
        printf_s("[-] MainWindow::MainWindow: OpenMutexW failed.\n");



    //初始化信号槽
    InitConnects();

    
    
}

void MainWindow::InitConnects()
{
    connect(ui.btn_freeze, &QPushButton::clicked, [&]() 
        {
            pFrameManager->Freeze();
        });
    connect(ui.btn_capture, &QPushButton::clicked, [&]()
        {
            pFrameManager->Capture();;
        });
    connect(ui.btn_registration, &QPushButton::clicked, [&]()
        {
            pFrameManager->RegistrationInit();
        });
    connect(ui.btn_continue, &QPushButton::clicked, [&]()
        {
            pFrameManager->Continue();
        });
    connect(ui.btn_save, &QPushButton::clicked, [&]()
        {
            pFrameManager->Save();
        });
    connect(ui.btn_save_calibrate, &QPushButton::clicked, [&]()
        {
            pFrameManager->SaveCalibration();

        });
    
    connect(pFrameManager->pDisplayer, &Displayer::displayImgs, [&]()
        {
            //在这个域里可以以引用的方式使用MainWindows作用于的变量
            //通过this->pFrameManager->Displayer->Imgs来得到所有需要的图像
            
            
            //先获取对Displayer成员变量的互斥量再进行操作
            if (hMutex != NULL)
            {
                DWORD mutex_state = WaitForSingleObject(hMutex, INFINITE);  //等待互斥量
                if (mutex_state = WAIT_OBJECT_0)
                {
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
                    ReleaseMutex(hMutex);
                }
            }
            












        }
    );

}

