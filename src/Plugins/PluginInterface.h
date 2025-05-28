#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <memory>

class vtkImageData;

namespace MedicalImaging {

/**
 * @brief 插件接口基类
 * 
 * 定义医学成像软件插件的标准接口
 */
class PluginInterface : public QObject {
    Q_OBJECT

public:
    virtual ~PluginInterface() = default;

    // 插件基本信息
    virtual QString getName() const = 0;
    virtual QString getVersion() const = 0;
    virtual QString getDescription() const = 0;
    virtual QString getAuthor() const = 0;
    virtual bool initialize() = 0;
    virtual void finalize() = 0;
    virtual bool isInitialized() const = 0;
    virtual bool canProcess(vtkImageData* imageData) const = 0;
    virtual vtkImageData* process(vtkImageData* imageData) = 0;
    virtual QString getLastError() const = 0;
};

/**
 * @brief 图像处理插件接口
 */
class ImageProcessingPlugin : public PluginInterface {
    Q_OBJECT

public:
    virtual ~ImageProcessingPlugin() = default;
    
    // 图像处理特定方法
    virtual QStringList getSupportedFormats() const = 0;
    virtual bool hasPreview() const = 0;
    virtual vtkImageData* generatePreview(vtkImageData* input) = 0;
    
signals:
    void processingStarted();
    void processingFinished();
    void processingProgress(int percentage);
};

/**
 * @brief 可视化插件接口
 */
class VisualizationPlugin : public PluginInterface {
    Q_OBJECT

public:
    virtual ~VisualizationPlugin() = default;
    
    // 可视化特定方法
    virtual QString getRenderingType() const = 0;
    virtual bool supportsInteraction() const = 0;
    virtual void setRenderWindow(void* renderWindow) = 0;
    virtual void updateVisualization() = 0;
    
signals:
    void visualizationUpdated();
    void interactionStarted();
    void interactionFinished();
};

/**
 * @brief 测量插件接口
 */
class MeasurementPlugin : public PluginInterface {
    Q_OBJECT

public:
    virtual ~MeasurementPlugin() = default;
    
    // 测量特定方法
    virtual QStringList getMeasurementTypes() const = 0;
    virtual QString getUnit() const = 0;
    virtual double performMeasurement(const QString& type, vtkImageData* data) = 0;
    virtual QString getResultText() const = 0;
    
signals:
    void measurementCompleted(const QString& result);
};

} // namespace MedicalImaging

// 注册接口，让Qt的元对象系统能识别
Q_DECLARE_INTERFACE(MedicalImaging::PluginInterface, "MedicalImaging.PluginInterface/1.0")

#endif // PLUGININTERFACE_H