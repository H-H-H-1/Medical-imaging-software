#ifndef REGISTRATIONMANAGER_H
#define REGISTRATIONMANAGER_H

#include <QObject>
#include <memory>

// VTK前向声明
class vtkImageData;
class vtkMatrix4x4;

/**
 * @brief 配准管理器，处理图像配准相关功能
 */
class RegistrationManager : public QObject {
    Q_OBJECT

public:
    explicit RegistrationManager(QObject *parent = nullptr);
    ~RegistrationManager();

    // 配准操作
    bool performRigidRegistration(vtkImageData* fixedImage, vtkImageData* movingImage);
    bool performAffineRegistration(vtkImageData* fixedImage, vtkImageData* movingImage);
    bool performDeformableRegistration(vtkImageData* fixedImage, vtkImageData* movingImage);

    // 获取配准结果
    vtkMatrix4x4* getTransformMatrix() const;
    vtkImageData* getRegisteredImage() const;

    // 配准参数设置
    void setMaxIterations(int iterations);
    void setTolerance(double tolerance);

signals:
    void registrationStarted();
    void registrationFinished();
    void registrationProgress(int percentage);

private:
    class RegistrationManagerPrivate;
    std::unique_ptr<RegistrationManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(RegistrationManager)
};

#endif // REGISTRATIONMANAGER_H
