#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <memory>

// VTK前向声明
class vtkImageData;
class vtkMatrix4x4;

/**
 * @brief 数据模型类，管理医学图像数据和元数据
 */
class DataModel : public QObject {
    Q_OBJECT

public:
    explicit DataModel(QObject *parent = nullptr);
    ~DataModel();

    // 图像数据操作
    void setImageData(vtkImageData* imageData);
    vtkImageData* getImageData() const;
    bool hasImageData() const;

    // 元数据操作
    void setMetaData(const QVariantMap& metaData);
    QVariantMap getMetaData() const;
    void setMetaDataValue(const QString& key, const QVariant& value);
    QVariant getMetaDataValue(const QString& key, const QVariant& defaultValue = QVariant()) const;

    // 变换矩阵操作
    void setTransformMatrix(vtkMatrix4x4* matrix);
    vtkMatrix4x4* getTransformMatrix() const;

    // 数据清理
    void clear();

    // 数据验证
    bool isValid() const;

signals:
    void imageDataChanged();
    void metaDataChanged();
    void transformMatrixChanged();

private:
    class DataModelPrivate;
    std::unique_ptr<DataModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(DataModel)
};

#endif // DATAMODEL_H
