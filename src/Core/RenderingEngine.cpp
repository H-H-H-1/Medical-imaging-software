#include "RenderingEngine.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>

class RenderingEngine::RenderingEnginePrivate : public QObject {
    Q_OBJECT // 添加 Q_OBJECT 宏以启用信号和槽机制

    // 私有成员变量可以在这里声明
public:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkImageActor> imageActor;
    bool initialized = false;
};

RenderingEngine::RenderingEngine(QObject *parent)
    : QObject(parent)
    , d_ptr(std::make_unique<RenderingEnginePrivate>())
{
    Q_D(RenderingEngine);
    d->renderer = vtkSmartPointer<vtkRenderer>::New();
    d->imageActor = vtkSmartPointer<vtkImageActor>::New();
}

RenderingEngine::~RenderingEngine() {
    cleanup();
}

void RenderingEngine::initialize() {
    Q_D(RenderingEngine);
    
    if (d->initialized) {
        return;
    }
    
    // 设置默认背景色为黑色
    d->renderer->SetBackground(0.0, 0.0, 0.0);
    
    // 添加图像演员到渲染器
    d->renderer->AddActor(d->imageActor);
    
    d->initialized = true;
    emit renderingStarted();
}

void RenderingEngine::cleanup() {
    Q_D(RenderingEngine);
    
    if (!d->initialized) {
        return;
    }
    
    if (d->renderer) {
        d->renderer->RemoveAllViewProps();
    }
    
    d->initialized = false;
}

bool RenderingEngine::isInitialized() const {
    Q_D(const RenderingEngine);
    return d->initialized;
}

void RenderingEngine::render() {
    Q_D(RenderingEngine);
    
    if (!d->initialized || !d->renderWindow) {
        return;
    }
    
    emit renderingStarted();
    d->renderWindow->Render();
    emit renderingFinished();
}

void RenderingEngine::resetCamera() {
    Q_D(RenderingEngine);
    
    if (d->renderer) {
        d->renderer->ResetCamera();
        emit cameraChanged();
    }
}

void RenderingEngine::setBackgroundColor(double r, double g, double b) {
    Q_D(RenderingEngine);
    
    if (d->renderer) {
        d->renderer->SetBackground(r, g, b);
    }
}

void RenderingEngine::setImageData(vtkImageData* imageData) {
    Q_D(RenderingEngine);
    
    if (imageData && d->imageActor) {
        d->imageActor->SetInputData(imageData);
        resetCamera();
    }
}

void RenderingEngine::removeImageData() {
    Q_D(RenderingEngine);
    
    if (d->imageActor) {
        d->imageActor->SetInputData(nullptr);
    }
}

void RenderingEngine::setViewport(int x, int y, int width, int height) {
    Q_D(RenderingEngine);
    
    if (d->renderer) {
        d->renderer->SetViewport(
            static_cast<double>(x) / width,
            static_cast<double>(y) / height,
            static_cast<double>(x + width) / width,
            static_cast<double>(y + height) / height
        );
    }
}

vtkRenderer* RenderingEngine::getRenderer() const {
    Q_D(const RenderingEngine);
    return d->renderer;
}

vtkRenderWindow* RenderingEngine::getRenderWindow() const {
    Q_D(const RenderingEngine);
    return d->renderWindow;
}

vtkRenderWindowInteractor* RenderingEngine::getInteractor() const {
    Q_D(const RenderingEngine);
    return d->interactor;
}

void RenderingEngine::onImageDataChanged() {
    // 响应图像数据变化
    render();
}

#include "RenderingEngine.moc"
