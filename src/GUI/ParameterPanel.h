#ifndef PARAMETERPANEL_H
#define PARAMETERPANEL_H

#include <iostream>
#include <QCoreApplication>
#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QTabWidget>
#include <memory>

namespace MedicalImaging {

/**
 * @brief 参数控制面板类
 * 
 * 提供医学图像处理和渲染参数的用户界面控制
 */
class ParameterPanel : public QWidget {
    Q_OBJECT

public:
    explicit ParameterPanel(QWidget* parent = nullptr);
    ~ParameterPanel();

    // 窗宽窗位控制
    void setWindowLevel(double window, double level);
    void getWindowLevel(double& window, double& level) const;
    void setWindowLevelRange(double minWindow, double maxWindow,
                           double minLevel, double maxLevel);

    // 阈值控制
    void setThreshold(double lower, double upper);
    void getThreshold(double& lower, double& upper) const;
    void setThresholdRange(double min, double max);

    // 透明度控制
    void setOpacity(double opacity);
    double getOpacity() const;

    // 颜色映射
    void setColormap(const QString& colormap);
    QString getColormap() const;

    // 滤波参数
    void setGaussianSigma(double sigma);
    double getGaussianSigma() const;
    
    void setMedianRadius(int radius);
    int getMedianRadius() const;

    // 配准参数
    void setRegistrationMethod(const QString& method);
    QString getRegistrationMethod() const;
    
    void setRegistrationTolerance(double tolerance);
    double getRegistrationTolerance() const;

    // UI控制
    void enableWindowLevelControls(bool enabled);
    void enableThresholdControls(bool enabled);
    void enableFilterControls(bool enabled);
    void enableRegistrationControls(bool enabled);

signals:
    // 窗宽窗位信号
    void windowLevelChanged(double window, double level);
    
    // 阈值信号
    void thresholdChanged(double lower, double upper);
    
    // 透明度信号
    void opacityChanged(double opacity);
    
    // 颜色映射信号
    void colormapChanged(const QString& colormap);
    
    // 滤波信号
    void gaussianFilterRequested(double sigma);
    void medianFilterRequested(int radius);
    void edgeDetectionRequested();
    void morphologyRequested(const QString& operation, int radius);
    
    // 配准信号
    void registrationRequested(const QString& method, double tolerance);
    
    // 复位信号
    void resetViewRequested();
    void resetParametersRequested();

public slots:
    void onWindowLevelChanged(double window, double level);
    void onThresholdChanged(double lower, double upper);
    void onOpacityChanged(double opacity);
    void resetParameters();

protected:
    void setupUI();
    void setupWindowLevelTab();
    void setupFilterTab();
    void setupRegistrationTab();
    void setupRenderingTab();
    void connectSignals();

private slots:
    void onWindowChanged(int value);
    void onLevelChanged(int value);
    void onLowerThresholdChanged(double value);
    void onUpperThresholdChanged(double value);
    void onOpacitySliderChanged(int value);
    void onColormapChanged(const QString& colormap);
    void onGaussianSigmaChanged(double sigma);
    void onMedianRadiusChanged(int radius);
    void onApplyGaussianFilter();
    void onApplyMedianFilter();
    void onApplyEdgeDetection();
    void onApplyMorphology();
    void onApplyRegistration();

private:
    struct Impl;
    std::unique_ptr<Impl> d;
};

} // namespace MedicalImaging

#endif // PARAMETERPANEL_H
