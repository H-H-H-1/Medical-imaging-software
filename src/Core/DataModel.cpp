#include "DataModel.h"
#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>

class DataModel::DataModelPrivate: public QObject {
    Q_OBJECT // 添加 Q_OBJECT{
public:
    vtkSmartPointer<vtkImageData> imageData;
    vtkSmartPointer<vtkMatrix4x4> transformMatrix;
    QVariantMap metaData;
};

DataModel::DataModel(QObject *parent)
    : QObject(parent)
    , d_ptr(std::make_unique<DataModelPrivate>())
{
    Q_D(DataModel);
    d->transformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    d->transformMatrix->Identity();
}

DataModel::~DataModel() = default;

void DataModel::setImageData(vtkImageData* imageData) {
    Q_D(DataModel);
    if (d->imageData != imageData) {
        d->imageData = imageData;
        emit imageDataChanged();
    }
}

vtkImageData* DataModel::getImageData() const {
    Q_D(const DataModel);
    return d->imageData;
}

bool DataModel::hasImageData() const {
    Q_D(const DataModel);
    return d->imageData != nullptr;
}

void DataModel::setMetaData(const QVariantMap& metaData) {
    Q_D(DataModel);
    if (d->metaData != metaData) {
        d->metaData = metaData;
        emit metaDataChanged();
    }
}

QVariantMap DataModel::getMetaData() const {
    Q_D(const DataModel);
    return d->metaData;
}

void DataModel::setMetaDataValue(const QString& key, const QVariant& value) {
    Q_D(DataModel);
    if (d->metaData.value(key) != value) {
        d->metaData[key] = value;
        emit metaDataChanged();
    }
}

QVariant DataModel::getMetaDataValue(const QString& key, const QVariant& defaultValue) const {
    Q_D(const DataModel);
    return d->metaData.value(key, defaultValue);
}

void DataModel::setTransformMatrix(vtkMatrix4x4* matrix) {
    Q_D(DataModel);
    if (matrix && d->transformMatrix != matrix) {
        d->transformMatrix->DeepCopy(matrix);
        emit transformMatrixChanged();
    }
}

vtkMatrix4x4* DataModel::getTransformMatrix() const {
    Q_D(const DataModel);
    return d->transformMatrix;
}

void DataModel::clear() {
    Q_D(DataModel);
    bool changed = false;
    
    if (d->imageData) {
        d->imageData = nullptr;
        emit imageDataChanged();
        changed = true;
    }
    
    if (!d->metaData.isEmpty()) {
        d->metaData.clear();
        emit metaDataChanged();
        changed = true;
    }
    
    d->transformMatrix->Identity();
    if (changed) {
        emit transformMatrixChanged();
    }
}

bool DataModel::isValid() const {
    Q_D(const DataModel);
    return d->imageData != nullptr;
}

#include "DataModel.moc"
