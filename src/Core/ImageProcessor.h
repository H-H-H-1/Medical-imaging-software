#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <memory>

// VTK前向声明
class vtkImageData;

/**
 * @brief 图像处理器，提供各种图像处理算法
 */
class ImageProcessor : public QObject {
    Q_OBJECT

public:
    explicit ImageProcessor(QObject *parent = nullptr);
    ~ImageProcessor();

    // 滤波操作
    vtkImageData* applyGaussianSmoothing(vtkImageData* input, double sigma);
    vtkImageData* applyMedianFilter(vtkImageData* input, int kernelSize);
    vtkImageData* applyAnisotropicDiffusion(vtkImageData* input, int iterations, double timeStep);

    // 形态学操作
    vtkImageData* applyErosion(vtkImageData* input, int radius);
    vtkImageData* applyDilation(vtkImageData* input, int radius);
    vtkImageData* applyOpening(vtkImageData* input, int radius);
    vtkImageData* applyClosing(vtkImageData* input, int radius);

    // 边缘检测
    vtkImageData* applySobelFilter(vtkImageData* input);
    vtkImageData* applyCannyEdgeDetector(vtkImageData* input, double threshold1, double threshold2);

    // 阈值处理
    vtkImageData* applyThreshold(vtkImageData* input, double lowerThreshold, double upperThreshold);
    vtkImageData* applyOtsuThreshold(vtkImageData* input);

signals:
    void processingStarted();
    void processingFinished();
    void processingProgress(int percentage);

private:
    class ImageProcessorPrivate;
    std::unique_ptr<ImageProcessorPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ImageProcessor)
};

#endif // IMAGEPROCESSOR_H
