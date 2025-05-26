#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <memory>

class vtkImageData;

namespace MedicalImaging {

/**
 * @brief 鎻掍欢鎺ュ彛鍩虹被
 * 
 * 瀹氫箟鍖诲鎴愬儚杞欢鎻掍欢鐨勬爣鍑嗘帴鍙?
 */
class PluginInterface : public QObject {
    Q_OBJECT

public:
    virtual ~PluginInterface() = default;

    // 鎻掍欢鍩烘湰淇℃伅
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
 * @brief 鍥惧儚澶勭悊鎻掍欢鎺ュ彛
 */
class ImageProcessingPlugin : public PluginInterface {
    Q_OBJECT

public:
    virtual ~ImageProcessingPlugin() = default;
    
    // 鍥惧儚澶勭悊鐗瑰畾鏂规硶
    virtual QStringList getSupportedFormats() const = 0;
    virtual bool hasPreview() const = 0;
    virtual vtkImageData* generatePreview(vtkImageData* input) = 0;
    
signals:
    void processingStarted();
    void processingFinished();
    void processingProgress(int percentage);
};

/**
 * @brief 鍙鍖栨彃浠舵帴鍙?
 */
class VisualizationPlugin : public PluginInterface {
    Q_OBJECT

public:
    virtual ~VisualizationPlugin() = default;
    
    // 鍙鍖栫壒瀹氭柟娉?
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
 * @brief 娴嬮噺鎻掍欢鎺ュ彛
 */
class MeasurementPlugin : public PluginInterface {
    Q_OBJECT

public:
    virtual ~MeasurementPlugin() = default;
    
    // 娴嬮噺鐗瑰畾鏂规硶
    virtual QStringList getMeasurementTypes() const = 0;
    virtual QString getUnit() const = 0;
    virtual double performMeasurement(const QString& type, vtkImageData* data) = 0;
    virtual QString getResultText() const = 0;
    
signals:
    void measurementCompleted(const QString& result);
};

} // namespace MedicalImaging

// 娉ㄥ唽鎺ュ彛锛岃Qt鐨勫厓瀵硅薄绯荤粺鑳借瘑鍒?
Q_DECLARE_INTERFACE(MedicalImaging::PluginInterface, "MedicalImaging.PluginInterface/1.0")

#endif // PLUGININTERFACE_H