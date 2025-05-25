#include "RegistrationManager.h"
#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>

class RegistrationManager::RegistrationManagerPrivate {
public:
    vtkSmartPointer<vtkImageData> fixedImage;
    vtkSmartPointer<vtkImageData> movingImage;
    vtkSmartPointer<vtkImageData> registeredImage;
    vtkSmartPointer<vtkMatrix4x4> transformMatrix;
    int maxIterations = 100;
    double tolerance = 1e-6;
};

RegistrationManager::RegistrationManager(QObject *parent)
    : QObject(parent)
    , d_ptr(std::make_unique<RegistrationManagerPrivate>())
{
    Q_D(RegistrationManager);
    d->transformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    d->transformMatrix->Identity();
}

RegistrationManager::~RegistrationManager() = default;

bool RegistrationManager::performRigidRegistration(vtkImageData* fixedImage, vtkImageData* movingImage) {
    Q_D(RegistrationManager);
    
    if (!fixedImage || !movingImage) {
        return false;
    }
    
    emit registrationStarted();
    
    // 这里应该实现实际的刚性配准算法
    // 暂时返回true表示成功
    d->fixedImage = fixedImage;
    d->movingImage = movingImage;
    d->registeredImage = movingImage; // 临时解决方案
    
    emit registrationFinished();
    return true;
}

bool RegistrationManager::performAffineRegistration(vtkImageData* fixedImage, vtkImageData* movingImage) {
    Q_D(RegistrationManager);
    
    if (!fixedImage || !movingImage) {
        return false;
    }
    
    emit registrationStarted();
    
    // 这里应该实现实际的仿射配准算法
    d->fixedImage = fixedImage;
    d->movingImage = movingImage;
    d->registeredImage = movingImage; // 临时解决方案
    
    emit registrationFinished();
    return true;
}

bool RegistrationManager::performDeformableRegistration(vtkImageData* fixedImage, vtkImageData* movingImage) {
    Q_D(RegistrationManager);
    
    if (!fixedImage || !movingImage) {
        return false;
    }
    
    emit registrationStarted();
    
    // 这里应该实现实际的可变形配准算法
    d->fixedImage = fixedImage;
    d->movingImage = movingImage;
    d->registeredImage = movingImage; // 临时解决方案
    
    emit registrationFinished();
    return true;
}

vtkMatrix4x4* RegistrationManager::getTransformMatrix() const {
    Q_D(const RegistrationManager);
    return d->transformMatrix;
}

vtkImageData* RegistrationManager::getRegisteredImage() const {
    Q_D(const RegistrationManager);
    return d->registeredImage;
}

void RegistrationManager::setMaxIterations(int iterations) {
    Q_D(RegistrationManager);
    d->maxIterations = iterations;
}

void RegistrationManager::setTolerance(double tolerance) {
    Q_D(RegistrationManager);
    d->tolerance = tolerance;
}

#include "RegistrationManager.moc"
