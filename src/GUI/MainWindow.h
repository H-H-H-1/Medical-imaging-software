// This file is C++
#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class DataModel;
class RenderingEngine;
class ImageProcessor;
class ViewportWidget;
class ParameterPanel;

/**
 * @brief 主窗口类，应用程序的主界面
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 文件操作
    void openFile();
    void saveFile();
    void exportImage();
    void exit();

    // 编辑操作
    void resetView();
    void resetParameters();

    // 帮助操作
    void showAbout();
    void showHelp();

    // 数据变化响应
    void onImageDataChanged();
    void onMetaDataChanged();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupConnections();
    void updateUI();

    class MainWindowPrivate;
    std::unique_ptr<MainWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MainWindow)
};

#endif // MAINWINDOW_H
