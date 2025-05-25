#ifndef VIEWPORTWIDGET_H
#define VIEWPORTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <memory>

// VTK前向声明
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkRenderer;
class vtkImageData;
class vtkImageViewer2;
class QVTKOpenGLNativeWidget;

namespace MedicalImaging {

/**
 * @brief 视口窗口部件类
 * 
 * 负责显示和交互医学图像的视口组件，支持多种视图模式
 */
class ViewportWidget : public QWidget {
    Q_OBJECT

public:
    enum ViewType {
        AXIAL,      ///< 轴位视图
        CORONAL,    ///< 冠状位视图
        SAGITTAL,   ///< 矢状位视图
        VOLUME_3D   ///< 3D体积渲染
    };

    explicit ViewportWidget(ViewType viewType = AXIAL, QWidget* parent = nullptr);
    ~ViewportWidget();

    // 图像显示相关
    void setImageData(vtkImageData* imageData);
    void updateDisplay();
    void resetView();
    void fitToWindow();

    // 视图控制
    void setViewType(ViewType type);
    ViewType getViewType() const;
    void setSlicePosition(int slice);
    int getSlicePosition() const;
    int getSliceCount() const;

    // 窗宽窗位控制
    void setWindowLevel(double window, double level);
    void getWindowLevel(double& window, double& level) const;

    // 缩放和平移
    void setZoom(double zoom);
    double getZoom() const;
    void resetZoom();

    // VTK组件访问
    vtkRenderWindow* getRenderWindow() const;
    vtkRenderer* getRenderer() const;

signals:
    void sliceChanged(int slice);
    void windowLevelChanged(double window, double level);
    void zoomChanged(double zoom);
    void mousePositionChanged(int x, int y, double value);

public slots:
    void onSliceChanged(int slice);
    void onWindowLevelChanged(double window, double level);
    void onZoomChanged(double zoom);

protected:
    void setupUI();
    void setupVTK();
    void connectSignals();

private slots:
    void updateSliceInfo();
    void updateWindowLevelInfo();

private:
    struct Impl;
    std::unique_ptr<Impl> d;
};

} // namespace MedicalImaging

#endif // VIEWPORTWIDGET_H
