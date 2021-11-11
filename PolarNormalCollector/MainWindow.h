#pragma once
#include <QtWidgets/QMainWindow>
#include "GeneratedFiles/ui_MainWindow.h"
#include "FrameManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);

private:
    void InitConnects();
    FrameManager* pFrameManager;
    HANDLE hMutex; //用于和Displayer同步的互斥量

private:
    Ui::MainWindow ui;
    
};
