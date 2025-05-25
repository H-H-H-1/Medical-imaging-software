#include "ViewportWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>

// VTK includes (条件编译以避免编译错误)
#ifdef VTK_FOUND
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkImageViewer2.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleImage.h>
#include <QVTKOpenGLNativeWidget.h>
#else
// 空的替代类以避免编译错误
class vtkRenderWindow {};
class vtkRenderWindowInteractor {};
class vtkRenderer {};
class vtkImageData {};
class vtkImageViewer2 {};
class QVTKOpenGLNativeWidget : public QWidget {
public:
    QVTKOpenGLNativeWidget(QWidget* parent = nullptr) : QWidget(parent) {}
    void setRenderWindow(vtkRenderWindow*) {}
    vtkRenderWindow* renderWindow() { return nullptr; }
};
#endif

namespace MedicalImaging {

struct ViewportWidget::Impl {
    ViewType viewType;
    QVTKOpenGLNativeWidget* vtkWidget;
    vtkRenderWindow* renderWindow;
    vtkRenderer* renderer;
    vtkImageViewer2* imageViewer;
    vtkImageData* currentImageData;
    
    // UI控件
    QLabel* sliceLabel;
    QSlider* sliceSlider;
    QSpinBox* sliceSpinBox;
    QLabel* windowLevelLabel;
    QLabel* zoomLabel;
    QLabel* positionLabel;
    
    // 状态变量
    int currentSlice;
    int sliceCount;
    double windowWidth;
    double windowLevel;
    double currentZoom;
    
    Impl() : viewType(ViewportWidget::AXIAL),
             vtkWidget(nullptr),
             renderWindow(nullptr),
             renderer(nullptr),
             imageViewer(nullptr),
             currentImageData(nullptr),
             sliceLabel(nullptr),
             sliceSlider(nullptr),
             sliceSpinBox(nullptr),
             windowLevelLabel(nullptr),
             zoomLabel(nullptr),
             positionLabel(nullptr),
             currentSlice(0),
             sliceCount(0),
             windowWidth(255.0),
             windowLevel(127.5),
             currentZoom(1.0) {}
};

ViewportWidget::ViewportWidget(ViewType viewType, QWidget* parent)
    : QWidget(parent), d(std::make_unique<Impl>()) {
    d->viewType = viewType;
    setupUI();
    setupVTK();
    connectSignals();
}

ViewportWidget::~ViewportWidget() = default;

void ViewportWidget::setupUI() {
    setMinimumSize(400, 300);
    
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->setSpacing(2);
    
    // VTK渲染窗口
    d->vtkWidget = new QVTKOpenGLNativeWidget(this);
    d->vtkWidget->setMinimumSize(300, 200);
    mainLayout->addWidget(d->vtkWidget, 1);
    
    // 控制面板
    auto* controlPanel = new QGroupBox("视图控制", this);
    auto* controlLayout = new QGridLayout(controlPanel);
    
    // 切片控制
    controlLayout->addWidget(new QLabel("切片:"), 0, 0);
    d->sliceSlider = new QSlider(Qt::Horizontal, this);
    d->sliceSlider->setRange(0, 0);
    d->sliceSlider->setValue(0);
    controlLayout->addWidget(d->sliceSlider, 0, 1);
    
    d->sliceSpinBox = new QSpinBox(this);
    d->sliceSpinBox->setRange(0, 0);
    controlLayout->addWidget(d->sliceSpinBox, 0, 2);
    
    d->sliceLabel = new QLabel("0/0", this);
    controlLayout->addWidget(d->sliceLabel, 0, 3);
    
    // 复位按钮
    auto* resetButton = new QPushButton("复位视图", this);
    controlLayout->addWidget(resetButton, 0, 4);
    
    // 信息显示
    d->windowLevelLabel = new QLabel("窗宽/窗位: 255/127", this);
    controlLayout->addWidget(d->windowLevelLabel, 1, 0, 1, 2);
    
    d->zoomLabel = new QLabel("缩放: 100%", this);
    controlLayout->addWidget(d->zoomLabel, 1, 2, 1, 2);
    
    d->positionLabel = new QLabel("位置: (0, 0) 值: 0", this);
    controlLayout->addWidget(d->positionLabel, 2, 0, 1, 5);
    
    mainLayout->addWidget(controlPanel);
    
    // 连接按钮信号
    connect(resetButton, &QPushButton::clicked, this, &ViewportWidget::resetView);
}

void ViewportWidget::setupVTK() {
#ifdef VTK_FOUND
    try {
        // 创建VTK组件
        d->renderWindow = vtkRenderWindow::New();
        d->renderer = vtkRenderer::New();
        d->imageViewer = vtkImageViewer2::New();
        
        // 设置渲染窗口
        d->vtkWidget->setRenderWindow(d->renderWindow);
        d->renderWindow->AddRenderer(d->renderer);
        
        // 设置背景色
        d->renderer->SetBackground(0.1, 0.1, 0.1);
        
        // 设置图像查看器
        d->imageViewer->SetRenderWindow(d->renderWindow);
        d->imageViewer->SetRenderer(d->renderer);
        
        std::cout << "VTK组件初始化成功" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "VTK初始化失败: " << e.what() << std::endl;
    }
#else
    std::cout << "VTK未找到，使用占位符实现" << std::endl;
#endif
}

void ViewportWidget::connectSignals() {
    // 连接切片控制信号
    connect(d->sliceSlider, QOverload<int>::of(&QSlider::valueChanged),
            this, &ViewportWidget::onSliceChanged);
    connect(d->sliceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ViewportWidget::onSliceChanged);
}

void ViewportWidget::setImageData(vtkImageData* imageData) {
    d->currentImageData = imageData;
    
#ifdef VTK_FOUND
    if (d->imageViewer && imageData) {
        try {
            d->imageViewer->SetInputData(imageData);
            
            // 获取图像尺寸
            int* dims = imageData->GetDimensions();
            int sliceCount = 0;
            
            switch (d->viewType) {
                case AXIAL:
                    sliceCount = dims[2];
                    d->imageViewer->SetSliceOrientationToXY();
                    break;
                case CORONAL:
                    sliceCount = dims[1];
                    d->imageViewer->SetSliceOrientationToXZ();
                    break;
                case SAGITTAL:
                    sliceCount = dims[0];
                    d->imageViewer->SetSliceOrientationToYZ();
                    break;
                case VOLUME_3D:
                    sliceCount = dims[2];
                    break;
            }
            
            d->sliceCount = sliceCount;
            d->sliceSlider->setRange(0, sliceCount - 1);
            d->sliceSpinBox->setRange(0, sliceCount - 1);
            
            // 设置初始切片到中间
            int middleSlice = sliceCount / 2;
            setSlicePosition(middleSlice);
            
            updateDisplay();
            fitToWindow();
            
        } catch (const std::exception& e) {
            std::cerr << "设置图像数据失败: " << e.what() << std::endl;
        }
    }
#endif
    
    updateSliceInfo();
}

void ViewportWidget::updateDisplay() {
#ifdef VTK_FOUND
    if (d->renderWindow) {
        d->renderWindow->Render();
    }
#endif
    update();
}

void ViewportWidget::resetView() {
    if (d->sliceCount > 0) {
        setSlicePosition(d->sliceCount / 2);
    }
    resetZoom();
    updateDisplay();
}

void ViewportWidget::fitToWindow() {
#ifdef VTK_FOUND
    if (d->renderer && d->currentImageData) {
        d->renderer->ResetCamera();
        updateDisplay();
    }
#endif
}

void ViewportWidget::setViewType(ViewType type) {
    d->viewType = type;
    if (d->currentImageData) {
        setImageData(d->currentImageData); // 重新设置以应用新的视图类型
    }
}

ViewportWidget::ViewType ViewportWidget::getViewType() const {
    return d->viewType;
}

void ViewportWidget::setSlicePosition(int slice) {
    if (slice < 0 || slice >= d->sliceCount) return;
    
    d->currentSlice = slice;
    
#ifdef VTK_FOUND
    if (d->imageViewer) {
        d->imageViewer->SetSlice(slice);
        updateDisplay();
    }
#endif
    
    // 更新UI控件
    d->sliceSlider->blockSignals(true);
    d->sliceSpinBox->blockSignals(true);
    d->sliceSlider->setValue(slice);
    d->sliceSpinBox->setValue(slice);
    d->sliceSlider->blockSignals(false);
    d->sliceSpinBox->blockSignals(false);
    
    updateSliceInfo();
    emit sliceChanged(slice);
}

int ViewportWidget::getSlicePosition() const {
    return d->currentSlice;
}

int ViewportWidget::getSliceCount() const {
    return d->sliceCount;
}

void ViewportWidget::setWindowLevel(double window, double level) {
    d->windowWidth = window;
    d->windowLevel = level;
    
#ifdef VTK_FOUND
    if (d->imageViewer) {
        d->imageViewer->SetColorWindow(window);
        d->imageViewer->SetColorLevel(level);
        updateDisplay();
    }
#endif
    
    updateWindowLevelInfo();
    emit windowLevelChanged(window, level);
}

void ViewportWidget::getWindowLevel(double& window, double& level) const {
    window = d->windowWidth;
    level = d->windowLevel;
}

void ViewportWidget::setZoom(double zoom) {
    d->currentZoom = zoom;
    
#ifdef VTK_FOUND
    if (d->renderer) {
        vtkCamera* camera = d->renderer->GetActiveCamera();
        if (camera) {
            camera->SetParallelScale(1.0 / zoom);
            updateDisplay();
        }
    }
#endif
    
    d->zoomLabel->setText(QString("缩放: %1%").arg(static_cast<int>(zoom * 100)));
    emit zoomChanged(zoom);
}

double ViewportWidget::getZoom() const {
    return d->currentZoom;
}

void ViewportWidget::resetZoom() {
    setZoom(1.0);
}

vtkRenderWindow* ViewportWidget::getRenderWindow() const {
    return d->renderWindow;
}

vtkRenderer* ViewportWidget::getRenderer() const {
    return d->renderer;
}

void ViewportWidget::onSliceChanged(int slice) {
    setSlicePosition(slice);
}

void ViewportWidget::onWindowLevelChanged(double window, double level) {
    setWindowLevel(window, level);
}

void ViewportWidget::onZoomChanged(double zoom) {
    setZoom(zoom);
}

void ViewportWidget::updateSliceInfo() {
    d->sliceLabel->setText(QString("%1/%2").arg(d->currentSlice + 1).arg(d->sliceCount));
}

void ViewportWidget::updateWindowLevelInfo() {
    d->windowLevelLabel->setText(QString("窗宽/窗位: %1/%2")
                                .arg(static_cast<int>(d->windowWidth))
                                .arg(static_cast<int>(d->windowLevel)));
}

} // namespace MedicalImaging

#include "ViewportWidget.moc"