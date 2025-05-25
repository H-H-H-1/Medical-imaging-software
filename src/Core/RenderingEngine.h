#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include <QObject>
#include <memory>

// VTK前向声明
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkImageData;

/**
 * @brief 渲染引擎类，负责VTK渲染管理
 */
class RenderingEngine : public QObject {
    Q_OBJECT

public:
    explicit RenderingEngine(QObject *parent = nullptr);
    ~RenderingEngine();

    // 渲染器管理
    void initialize();
    void cleanup();
    bool isInitialized() const;

    // 渲染控制
    void render();
    void resetCamera();
    void setBackgroundColor(double r, double g, double b);

    // 数据设置
    void setImageData(vtkImageData* imageData);
    void removeImageData();

    // 视口操作
    void setViewport(int x, int y, int width, int height);
    
    // 获取VTK组件
    vtkRenderer* getRenderer() const;
    vtkRenderWindow* getRenderWindow() const;
    vtkRenderWindowInteractor* getInteractor() const;

signals:
    void renderingStarted();
    void renderingFinished();
    void cameraChanged();

public slots:
    void onImageDataChanged();

private:
    class RenderingEnginePrivate;
    std::unique_ptr<RenderingEnginePrivate> d_ptr;
    Q_DECLARE_PRIVATE(RenderingEngine)
};

#endif // RENDERINGENGINE_H
