#include "MainWindow.h"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("医学影像软件");
    // 显示窗口
    window.show();
    // 启动应用程序并返回退出码
    return app.exec();
}


