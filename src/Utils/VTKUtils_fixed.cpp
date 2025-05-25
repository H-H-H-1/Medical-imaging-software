#include "VTKUtils_fixed.h"
#include <QtCore/QDebug>

namespace MedicalImaging {

// 条件编译 - 仅在VTK可用时编译
#ifdef VTK_AVAILABLE
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkDataSet.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkLookupTable.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageTestSource.h>
#include <vtkSphereSource.h>
#include <vtkMath.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>

bool VTKUtils::isVTKAvailable() {
    return true;
}

QString VTKUtils::getVTKVersion() {
    return QString("VTK 9.x");
}

bool VTKUtils::isValidImageData(vtkImageData* imageData) {
    return validateImageData(imageData);
}

bool VTKUtils::validateImageData(vtkImageData* imageData) {
    if (!imageData) {
        return false;
    }
    
    int* dims = imageData->GetDimensions();
    return (dims[0] > 0 && dims[1] > 0 && dims[2] > 0);
}

void VTKUtils::getImageInfo(vtkImageData* imageData, QString& info) {
    if (!imageData) {
        info = "Invalid image data";
        return;
    }
    
    int* dims = imageData->GetDimensions();
    double* spacing = imageData->GetSpacing();
    double* origin = imageData->GetOrigin();
    
    info = QString("Dimensions: %1 x %2 x %3\n"
                  "Spacing: %4 x %5 x %6\n"
                  "Origin: %7 x %8 x %9")
           .arg(dims[0]).arg(dims[1]).arg(dims[2])
           .arg(spacing[0]).arg(spacing[1]).arg(spacing[2])
           .arg(origin[0]).arg(origin[1]).arg(origin[2]);
}

vtkImageData* VTKUtils::createTestImageData(int dimensions[3]) {
    return createTestImageData(dimensions[0], dimensions[1], dimensions[2]);
}

vtkImageData* VTKUtils::createTestImageData(int width, int height, int depth) {
    auto testSource = vtkImageTestSource::New();
    testSource->SetWholeExtent(0, width-1, 0, height-1, 0, depth-1);
    testSource->Update();
    
    vtkImageData* result = vtkImageData::New();
    result->DeepCopy(testSource->GetOutput());
    testSource->Delete();
    
    return result;
}

void VTKUtils::copyImageData(vtkImageData* source, vtkImageData* target) {
    if (source && target) {
        target->DeepCopy(source);
    }
}

bool VTKUtils::saveImageData(vtkImageData* imageData, const QString& filename) {
    Q_UNUSED(imageData)
    Q_UNUSED(filename)
    // 实现保存逻辑
    return false;
}

vtkImageData* VTKUtils::loadImageData(const QString& filename) {
    Q_UNUSED(filename)
    // 实现加载逻辑
    return nullptr;
}

bool VTKUtils::isValidPolyData(vtkPolyData* polyData) {
    return polyData != nullptr && polyData->GetNumberOfPoints() > 0;
}

void VTKUtils::getPolyDataInfo(vtkPolyData* polyData, QString& info) {
    if (!polyData) {
        info = "Invalid poly data";
        return;
    }
    
    info = QString("Points: %1\nCells: %2")
           .arg(polyData->GetNumberOfPoints())
           .arg(polyData->GetNumberOfCells());
}

vtkPolyData* VTKUtils::createTestPolyData() {
    auto sphereSource = vtkSphereSource::New();
    sphereSource->SetRadius(1.0);
    sphereSource->SetThetaResolution(20);
    sphereSource->SetPhiResolution(20);
    sphereSource->Update();
    
    vtkPolyData* result = vtkPolyData::New();
    result->DeepCopy(sphereSource->GetOutput());
    sphereSource->Delete();
    
    return result;
}

vtkMatrix4x4* VTKUtils::createIdentityMatrix() {
    auto matrix = vtkMatrix4x4::New();
    matrix->Identity();
    return matrix;
}

vtkMatrix4x4* VTKUtils::createTranslationMatrix(double x, double y, double z) {
    auto matrix = vtkMatrix4x4::New();
    matrix->Identity();
    matrix->SetElement(0, 3, x);
    matrix->SetElement(1, 3, y);
    matrix->SetElement(2, 3, z);
    return matrix;
}

vtkMatrix4x4* VTKUtils::createRotationMatrix(double angleX, double angleY, double angleZ) {
    Q_UNUSED(angleX)
    Q_UNUSED(angleY)
    Q_UNUSED(angleZ)
    // 简化实现，返回单位矩阵
    return createIdentityMatrix();
}

vtkMatrix4x4* VTKUtils::createScaleMatrix(double sx, double sy, double sz) {
    auto matrix = vtkMatrix4x4::New();
    matrix->Identity();
    matrix->SetElement(0, 0, sx);
    matrix->SetElement(1, 1, sy);
    matrix->SetElement(2, 2, sz);
    return matrix;
}

void VTKUtils::multiplyMatrices(vtkMatrix4x4* a, vtkMatrix4x4* b, vtkMatrix4x4* result) {
    if (a && b && result) {
        vtkMatrix4x4::Multiply4x4(a, b, result);
    }
}

vtkLookupTable* VTKUtils::createGrayScaleLookupTable(int numberOfColors) {
    auto lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(numberOfColors);
    lut->SetRange(0.0, 1.0);
    lut->SetValueRange(0.0, 1.0);
    lut->SetSaturationRange(0.0, 0.0);
    lut->SetRampToLinear();
    lut->Build();
    return lut;
}

vtkLookupTable* VTKUtils::createJetLookupTable(int numberOfColors) {
    auto lut = vtkLookupTable::New();
    lut->SetNumberOfTableValues(numberOfColors);
    lut->SetRange(0.0, 1.0);
    lut->SetHueRange(0.67, 0.0);
    lut->Build();
    return lut;
}

vtkLookupTable* VTKUtils::createLookupTable(const QString& colormap) {
    if (colormap.toLower() == "jet") {
        return createJetLookupTable();
    } else {
        return createGrayScaleLookupTable();
    }
}

vtkColorTransferFunction* VTKUtils::createGrayScaleColorTransferFunction() {
    auto ctf = vtkColorTransferFunction::New();
    ctf->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
    ctf->AddRGBPoint(1.0, 1.0, 1.0, 1.0);
    return ctf;
}

vtkColorTransferFunction* VTKUtils::createJetColorTransferFunction() {
    auto ctf = vtkColorTransferFunction::New();
    ctf->AddRGBPoint(0.0, 0.0, 0.0, 0.5);
    ctf->AddRGBPoint(0.25, 0.0, 0.0, 1.0);
    ctf->AddRGBPoint(0.5, 0.0, 1.0, 1.0);
    ctf->AddRGBPoint(0.75, 1.0, 1.0, 0.0);
    ctf->AddRGBPoint(1.0, 1.0, 0.0, 0.0);
    return ctf;
}

vtkMatrix4x4* VTKUtils::createTransformMatrix(double tx, double ty, double tz,
                                             double rx, double ry, double rz,
                                             double sx, double sy, double sz) {
    Q_UNUSED(rx)
    Q_UNUSED(ry)
    Q_UNUSED(rz)
    
    // 简化实现：创建平移和缩放矩阵
    auto matrix = vtkMatrix4x4::New();
    matrix->Identity();
    
    // 设置平移
    matrix->SetElement(0, 3, tx);
    matrix->SetElement(1, 3, ty);
    matrix->SetElement(2, 3, tz);
    
    // 设置缩放
    matrix->SetElement(0, 0, sx);
    matrix->SetElement(1, 1, sy);
    matrix->SetElement(2, 2, sz);
    
    return matrix;
}

QString VTKUtils::vtkDataTypeToString(int dataType) {
    switch(dataType) {
        case VTK_CHAR: return "char";
        case VTK_UNSIGNED_CHAR: return "unsigned char";
        case VTK_SHORT: return "short";
        case VTK_UNSIGNED_SHORT: return "unsigned short";
        case VTK_INT: return "int";
        case VTK_UNSIGNED_INT: return "unsigned int";
        case VTK_FLOAT: return "float";
        case VTK_DOUBLE: return "double";
        default: return "unknown";
    }
}

int VTKUtils::stringToVtkDataType(const QString& dataTypeString) {
    QString lower = dataTypeString.toLower();
    if (lower == "char") return VTK_CHAR;
    if (lower == "unsigned char") return VTK_UNSIGNED_CHAR;
    if (lower == "short") return VTK_SHORT;
    if (lower == "unsigned short") return VTK_UNSIGNED_SHORT;
    if (lower == "int") return VTK_INT;
    if (lower == "unsigned int") return VTK_UNSIGNED_INT;
    if (lower == "float") return VTK_FLOAT;
    if (lower == "double") return VTK_DOUBLE;
    return VTK_VOID;
}

void VTKUtils::getDataRange(vtkDataSet* dataSet, double range[2]) {
    if (dataSet && dataSet->GetPointData() && dataSet->GetPointData()->GetScalars()) {
        dataSet->GetPointData()->GetScalars()->GetRange(range);
    } else {
        range[0] = 0.0;
        range[1] = 1.0;
    }
}

void VTKUtils::getScalarRange(vtkDataSet* dataSet, double range[2]) {
    getDataRange(dataSet, range);
}

void VTKUtils::normalizeData(vtkDataSet* dataSet) {
    Q_UNUSED(dataSet)
    // 实现数据标准化逻辑
}

QString VTKUtils::getVTKFileExtension(const QString& filename) {
    int lastDot = filename.lastIndexOf('.');
    if (lastDot >= 0) {
        return filename.mid(lastDot + 1).toLower();
    }
    return QString();
}

bool VTKUtils::isVTKFile(const QString& filename) {
    QString ext = getVTKFileExtension(filename);
    QStringList vtkExts = getSupportedVTKExtensions();
    return vtkExts.contains(ext);
}

QStringList VTKUtils::getSupportedVTKExtensions() {
    return QStringList() << "vtk" << "vti" << "vtu" << "vtp" << "vts" << "vtr";
}

void VTKUtils::printImageDataInfo(vtkImageData* imageData) {
    if (!imageData) {
        qDebug() << "Invalid image data";
        return;
    }
    
    int* dims = imageData->GetDimensions();
    double* spacing = imageData->GetSpacing();
    double* origin = imageData->GetOrigin();
    
    qDebug() << QString("ImageData Info:")
             << QString("Dimensions: %1 x %2 x %3").arg(dims[0]).arg(dims[1]).arg(dims[2])
             << QString("Spacing: %1 x %2 x %3").arg(spacing[0]).arg(spacing[1]).arg(spacing[2])
             << QString("Origin: %1 x %2 x %3").arg(origin[0]).arg(origin[1]).arg(origin[2]);
}

void VTKUtils::printPolyDataInfo(vtkPolyData* polyData) {
    if (!polyData) {
        qDebug() << "Invalid poly data";
        return;
    }
    
    qDebug() << QString("PolyData Info:")
             << QString("Points: %1").arg(polyData->GetNumberOfPoints())
             << QString("Cells: %1").arg(polyData->GetNumberOfCells());
}

void VTKUtils::printMatrixInfo(vtkMatrix4x4* matrix) {
    if (!matrix) {
        qDebug() << "Invalid matrix";
        return;
    }
    
    qDebug() << "Matrix Info:";
    for (int i = 0; i < 4; ++i) {
        QString row;
        for (int j = 0; j < 4; ++j) {
            row += QString("%1 ").arg(matrix->GetElement(i, j), 8, 'f', 3);
        }
        qDebug() << row;
    }
}

void VTKUtils::calculateOptimalWindowLevel(vtkImageData* imageData, double& window, double& level) {
    if (!imageData) {
        window = 1.0;
        level = 0.5;
        return;
    }
    
    double range[2];
    getDataRange(imageData, range);
    window = range[1] - range[0];
    level = (range[1] + range[0]) / 2.0;
}

void VTKUtils::applyWindowLevel(vtkImageData* imageData, double window, double level) {
    Q_UNUSED(imageData)
    Q_UNUSED(window)
    Q_UNUSED(level)
    // 实现窗宽窗位应用逻辑
}

double VTKUtils::calculateDistance(const double point1[3], const double point2[3]) {
    return sqrt(vtkMath::Distance2BetweenPoints(point1, point2));
}

void VTKUtils::calculateCenter(vtkDataSet* dataSet, double center[3]) {
    if (!dataSet) {
        center[0] = center[1] = center[2] = 0.0;
        return;
    }
    
    double bounds[6];
    dataSet->GetBounds(bounds);
    center[0] = (bounds[0] + bounds[1]) / 2.0;
    center[1] = (bounds[2] + bounds[3]) / 2.0;
    center[2] = (bounds[4] + bounds[5]) / 2.0;
}

double VTKUtils::calculateVolume(vtkPolyData* polyData) {
    Q_UNUSED(polyData)
    // 实现体积计算逻辑
    return 0.0;
}

double VTKUtils::calculateSurfaceArea(vtkPolyData* polyData) {
    Q_UNUSED(polyData)
    // 实现表面积计算逻辑
    return 0.0;
}

#else // VTK_AVAILABLE not defined

// VTK不可用时的替代实现
bool VTKUtils::isVTKAvailable() {
    return false;
}

QString VTKUtils::getVTKVersion() {
    return QString("VTK not available");
}

bool VTKUtils::isValidImageData(vtkImageData* imageData) {
    Q_UNUSED(imageData)
    return false;
}

bool VTKUtils::validateImageData(vtkImageData* imageData) {
    Q_UNUSED(imageData)
    return false;
}

void VTKUtils::getImageInfo(vtkImageData* imageData, QString& info) {
    Q_UNUSED(imageData)
    info = "VTK not available";
}

vtkImageData* VTKUtils::createTestImageData(int dimensions[3]) {
    Q_UNUSED(dimensions)
    return nullptr;
}

vtkImageData* VTKUtils::createTestImageData(int width, int height, int depth) {
    Q_UNUSED(width)
    Q_UNUSED(height)
    Q_UNUSED(depth)
    return nullptr;
}

void VTKUtils::copyImageData(vtkImageData* source, vtkImageData* target) {
    Q_UNUSED(source)
    Q_UNUSED(target)
}

bool VTKUtils::saveImageData(vtkImageData* imageData, const QString& filename) {
    Q_UNUSED(imageData)
    Q_UNUSED(filename)
    return false;
}

vtkImageData* VTKUtils::loadImageData(const QString& filename) {
    Q_UNUSED(filename)
    return nullptr;
}

bool VTKUtils::isValidPolyData(vtkPolyData* polyData) {
    Q_UNUSED(polyData)
    return false;
}

void VTKUtils::getPolyDataInfo(vtkPolyData* polyData, QString& info) {
    Q_UNUSED(polyData)
    info = "VTK not available";
}

vtkPolyData* VTKUtils::createTestPolyData() {
    return nullptr;
}

vtkMatrix4x4* VTKUtils::createIdentityMatrix() {
    return nullptr;
}

vtkMatrix4x4* VTKUtils::createTranslationMatrix(double x, double y, double z) {
    Q_UNUSED(x)
    Q_UNUSED(y)
    Q_UNUSED(z)
    return nullptr;
}

vtkMatrix4x4* VTKUtils::createRotationMatrix(double angleX, double angleY, double angleZ) {
    Q_UNUSED(angleX)
    Q_UNUSED(angleY)
    Q_UNUSED(angleZ)
    return nullptr;
}

vtkMatrix4x4* VTKUtils::createScaleMatrix(double sx, double sy, double sz) {
    Q_UNUSED(sx)
    Q_UNUSED(sy)
    Q_UNUSED(sz)
    return nullptr;
}

void VTKUtils::multiplyMatrices(vtkMatrix4x4* a, vtkMatrix4x4* b, vtkMatrix4x4* result) {
    Q_UNUSED(a)
    Q_UNUSED(b)
    Q_UNUSED(result)
}

vtkLookupTable* VTKUtils::createGrayScaleLookupTable(int numberOfColors) {
    Q_UNUSED(numberOfColors)
    return nullptr;
}

vtkLookupTable* VTKUtils::createJetLookupTable(int numberOfColors) {
    Q_UNUSED(numberOfColors)
    return nullptr;
}

vtkLookupTable* VTKUtils::createLookupTable(const QString& colormap) {
    Q_UNUSED(colormap)
    return nullptr;
}

vtkColorTransferFunction* VTKUtils::createGrayScaleColorTransferFunction() {
    return nullptr;
}

vtkColorTransferFunction* VTKUtils::createJetColorTransferFunction() {
    return nullptr;
}

vtkMatrix4x4* VTKUtils::createTransformMatrix(double tx, double ty, double tz,
                                             double rx, double ry, double rz,
                                             double sx, double sy, double sz) {
    Q_UNUSED(tx)
    Q_UNUSED(ty)
    Q_UNUSED(tz)
    Q_UNUSED(rx)
    Q_UNUSED(ry)
    Q_UNUSED(rz)
    Q_UNUSED(sx)
    Q_UNUSED(sy)
    Q_UNUSED(sz)
    return nullptr;
}

QString VTKUtils::vtkDataTypeToString(int dataType) {
    Q_UNUSED(dataType)
    return "unknown";
}

int VTKUtils::stringToVtkDataType(const QString& dataTypeString) {
    Q_UNUSED(dataTypeString)
    return 0;
}

void VTKUtils::getDataRange(vtkDataSet* dataSet, double range[2]) {
    Q_UNUSED(dataSet)
    range[0] = 0.0;
    range[1] = 1.0;
}

void VTKUtils::getScalarRange(vtkDataSet* dataSet, double range[2]) {
    Q_UNUSED(dataSet)
    range[0] = 0.0;
    range[1] = 1.0;
}

void VTKUtils::normalizeData(vtkDataSet* dataSet) {
    Q_UNUSED(dataSet)
}

QString VTKUtils::getVTKFileExtension(const QString& filename) {
    int lastDot = filename.lastIndexOf('.');
    if (lastDot >= 0) {
        return filename.mid(lastDot + 1).toLower();
    }
    return QString();
}

bool VTKUtils::isVTKFile(const QString& filename) {
    QString ext = getVTKFileExtension(filename);
    QStringList vtkExts = getSupportedVTKExtensions();
    return vtkExts.contains(ext);
}

QStringList VTKUtils::getSupportedVTKExtensions() {
    return QStringList() << "vtk" << "vti" << "vtu" << "vtp" << "vts" << "vtr";
}

void VTKUtils::printImageDataInfo(vtkImageData* imageData) {
    Q_UNUSED(imageData)
    qDebug() << "VTK not available";
}

void VTKUtils::printPolyDataInfo(vtkPolyData* polyData) {
    Q_UNUSED(polyData)
    qDebug() << "VTK not available";
}

void VTKUtils::printMatrixInfo(vtkMatrix4x4* matrix) {
    Q_UNUSED(matrix)
    qDebug() << "VTK not available";
}

void VTKUtils::calculateOptimalWindowLevel(vtkImageData* imageData, double& window, double& level) {
    Q_UNUSED(imageData)
    window = 1.0;
    level = 0.5;
}

void VTKUtils::applyWindowLevel(vtkImageData* imageData, double window, double level) {
    Q_UNUSED(imageData)
    Q_UNUSED(window)
    Q_UNUSED(level)
}

double VTKUtils::calculateDistance(const double point1[3], const double point2[3]) {
    Q_UNUSED(point1)
    Q_UNUSED(point2)
    return 0.0;
}

void VTKUtils::calculateCenter(vtkDataSet* dataSet, double center[3]) {
    Q_UNUSED(dataSet)
    center[0] = center[1] = center[2] = 0.0;
}

double VTKUtils::calculateVolume(vtkPolyData* polyData) {
    Q_UNUSED(polyData)
    return 0.0;
}

double VTKUtils::calculateSurfaceArea(vtkPolyData* polyData) {
    Q_UNUSED(polyData)
    return 0.0;
}

#endif // VTK_AVAILABLE

} // namespace MedicalImaging
