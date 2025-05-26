#ifndef VIEWPORTWIDGET_H
#define VIEWPORTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <memory>

// VTK鍓嶅悜澹版槑
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkRenderer;
class vtkImageData;
class vtkImageViewer2;
class QVTKOpenGLNativeWidget;

namespace MedicalImaging {

/**
 * @brief 瑙嗗彛绐楀彛閮ㄤ欢绫?
 * 
 * 璐熻矗鏄剧ず鍜屼氦浜掑尰瀛﹀浘鍍忕殑瑙嗗彛缁勪欢锛屾敮鎸佸绉嶈鍥炬ā寮?
 */
class ViewportWidget : public QWidget {
    Q_OBJECT

public:
    enum ViewType {
        AXIAL,      ///< 杞翠綅瑙嗗浘
        CORONAL,    ///< 鍐犵姸浣嶈鍥?
        SAGITTAL,   ///< 鐭㈢姸浣嶈鍥?
        VOLUME_3D   ///< 3D浣撶Н娓叉煋
    };

    explicit ViewportWidget(ViewType viewType = AXIAL, QWidget* parent = nullptr);
    ~ViewportWidget();

    // 鍥惧儚鏄剧ず鐩稿叧
    void setImageData(vtkImageData* imageData);
    void updateDisplay();
    void resetView();
    void fitToWindow();

    // 瑙嗗浘鎺у埗
    void setViewType(ViewType type);
    ViewType getViewType() const;
    void setSlicePosition(int slice);
    int getSlicePosition() const;
    int getSliceCount() const;

    // 绐楀绐椾綅鎺у埗
    void setWindowLevel(double window, double level);
    void getWindowLevel(double& window, double& level) const;

    // 缂╂斁鍜屽钩绉?
    void setZoom(double zoom);
    double getZoom() const;
    void resetZoom();

    // VTK缁勪欢璁块棶
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
