// This file is C++
#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <memory>

class DataModel;
class RenderingEngine;
class ImageProcessor;
class ViewportWidget;
class ParameterPanel;

/**
 * @brief 涓荤獥鍙ｇ被锛屽簲鐢ㄧ▼搴忕殑涓荤晫闈?
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 鏂囦欢鎿嶄綔
    void openFile();
    void saveFile();
    void exportImage();
    void exit();

    // 缂栬緫鎿嶄綔
    void resetView();
    void resetParameters();

    // 甯姪鎿嶄綔
    void showAbout();
    void showHelp();

    // 鏁版嵁鍙樺寲鍝嶅簲
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
