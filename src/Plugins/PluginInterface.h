#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include <QObject>
#include <memory>

class vtkImageData;

namespace MedicalImaging {

/**
 * @brief 插件接口基类
 * 
 * 定义医学成像软件插件的标准接口
 */
class PluginInterface {
public:
    virtual ~PluginInterface() = default;

    // 插件基本信息
    virtual QString getName() const = 0;
    virtual QString getVersion() const = 0;
    virtual QString getDescription() const = 0;
    virtual QString getAuthor() const = 0;

    // 插件生命周期
    virtual bool initialize() = 0;
    virtual void finalize() = 0;
    virtual bool isInitialized() const = 0;

    // 插件功能
    virtual bool canProcess(vtkImageData* imageData) const = 0;
    virtual vtkImageData* process(vtkImageData* imageData) = 0;
    virtual QString getLastError() const = 0;
};

/**
 * @brief 图像处理插件接口
 */
class ImageProcessingPlugin : public PluginInterface {
public:
    enum ProcessingType {
        FILTER,         ///< 滤波处理
        ENHANCEMENT,    ///< 图像增强
        SEGMENTATION,   ///< 图像分割
        REGISTRATION,   ///< 图像配准
        RECONSTRUCTION  ///< 图像重建
    };

    virtual ProcessingType getProcessingType() const = 0;
    virtual QStringList getSupportedFormats() const = 0;
    virtual bool hasPreview() const = 0;
    virtual vtkImageData* preview(vtkImageData* imageData) = 0;
};

/**
 * @brief 可视化插件接口
 */
class VisualizationPlugin : public PluginInterface {
public:
    enum VisualizationType {
        VOLUME_RENDERING,   ///< 体绘制
        SURFACE_RENDERING,  ///< 面绘制
        MIP,               ///< 最大密度投影
        MPR                ///< 多平面重建
    };

    virtual VisualizationType getVisualizationType() const = 0;
    virtual QWidget* createControlWidget() = 0;
    virtual void updateVisualization(vtkImageData* imageData) = 0;
};

/**
 * @brief 测量工具插件接口
 */
class MeasurementPlugin : public PluginInterface {
public:
    enum MeasurementType {
        DISTANCE,   ///< 距离测量
        ANGLE,      ///< 角度测量
        AREA,       ///< 面积测量
        VOLUME      ///< 体积测量
    };

    virtual MeasurementType getMeasurementType() const = 0;
    virtual QString getMeasurementUnit() const = 0;
    virtual double measure(vtkImageData* imageData, const QVariantMap& parameters) = 0;
    virtual QWidget* createMeasurementWidget() = 0;
};

} // namespace MedicalImaging

// Qt插件宏定义
Q_DECLARE_INTERFACE(MedicalImaging::PluginInterface, "MedicalImaging.PluginInterface/1.0")
Q_DECLARE_INTERFACE(MedicalImaging::ImageProcessingPlugin, "MedicalImaging.ImageProcessingPlugin/1.0")
Q_DECLARE_INTERFACE(MedicalImaging::VisualizationPlugin, "MedicalImaging.VisualizationPlugin/1.0")
Q_DECLARE_INTERFACE(MedicalImaging::MeasurementPlugin, "MedicalImaging.MeasurementPlugin/1.0")

#endif // PLUGININTERFACE_H