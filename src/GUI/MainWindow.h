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
 * @brief Main window class for the application
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // File operations
    void openFile();
    void saveFile();
    void exportImage();
    void exit();

    // View operations
    void resetView();
    void resetParameters();

    // Help operations
    void showAbout();
    void showHelp();

    // Data operations
    void onImageDataChanged();
    void onMetaDataChanged();

private:
    void setupUI();
    void setupMenuBar();
    // void setupToolBar();
    void setupStatusBar();
    void setupConnections();
    void updateUI();

    class MainWindowPrivate;
    std::unique_ptr<MainWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MainWindow)
};

#endif // MAINWINDOW_H
