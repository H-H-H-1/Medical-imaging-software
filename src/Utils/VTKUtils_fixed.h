#ifndef VTKUTILS_H
#define VTKUTILS_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <memory>

// VTK前向声明
class vtkImageData;
class vtkPolyData;
class vtkDataSet;
class vtkMatrix4x4;
class vtkTransform;
class vtkLookupTable;
class vtkColorTransferFunction;

namespace MedicalImaging {

/**
 * @brief VTK工具类
 * 
 * 提供VTK相关的实用功能和辅助方法
 */
class VTKUtils {
public:
    // VTK可用性检查
    static bool isVTKAvailable();
    static QString getVTKVersion();
    
    // 图像数据工具
    static bool isValidImageData(vtkImageData* imageData);
    static bool validateImageData(vtkImageData* imageData);
    static void getImageInfo(vtkImageData* imageData, QString& info);
    static vtkImageData* createTestImageData(int dimensions[3]);
    static vtkImageData* createTestImageData(int width, int height, int depth);
    static void copyImageData(vtkImageData* source, vtkImageData* target);
    
    // 文件I/O
    static bool saveImageData(vtkImageData* imageData, const QString& filename);
    static vtkImageData* loadImageData(const QString& filename);
    
    // 几何数据工具
    static bool isValidPolyData(vtkPolyData* polyData);
    static void getPolyDataInfo(vtkPolyData* polyData, QString& info);
    static vtkPolyData* createTestPolyData();
    
    // 变换工具
    static vtkMatrix4x4* createIdentityMatrix();
    static vtkMatrix4x4* createTranslationMatrix(double x, double y, double z);
    static vtkMatrix4x4* createRotationMatrix(double angleX, double angleY, double angleZ);
    static vtkMatrix4x4* createScaleMatrix(double sx, double sy, double sz);
    static void multiplyMatrices(vtkMatrix4x4* a, vtkMatrix4x4* b, vtkMatrix4x4* result);
    
    // 颜色映射工具
    static vtkLookupTable* createGrayScaleLookupTable(int numberOfColors = 256);
    static vtkLookupTable* createJetLookupTable(int numberOfColors = 256);
    static vtkLookupTable* createLookupTable(const QString& colormap);
    static vtkColorTransferFunction* createGrayScaleColorTransferFunction();
    static vtkColorTransferFunction* createJetColorTransferFunction();
    
    // 变换矩阵创建
    static vtkMatrix4x4* createTransformMatrix(double tx, double ty, double tz,
                                               double rx, double ry, double rz,
                                               double sx, double sy, double sz);
    
    // 数据类型转换
    static QString vtkDataTypeToString(int dataType);
    static int stringToVtkDataType(const QString& dataTypeString);
    
    // 数据范围工具
    static void getDataRange(vtkDataSet* dataSet, double range[2]);
    static void getScalarRange(vtkDataSet* dataSet, double range[2]);
    static void normalizeData(vtkDataSet* dataSet);
    
    // 文件I/O辅助
    static QString getVTKFileExtension(const QString& filename);
    static bool isVTKFile(const QString& filename);
    static QStringList getSupportedVTKExtensions();
    
    // 内存管理辅助
    template<typename T>
    static void safeDelete(T*& pointer) {
        if (pointer) {
            pointer->Delete();
            pointer = nullptr;
        }
    }
    
    // 调试辅助
    static void printImageDataInfo(vtkImageData* imageData);
    static void printPolyDataInfo(vtkPolyData* polyData);
    static void printMatrixInfo(vtkMatrix4x4* matrix);
    
    // 窗宽窗位计算
    static void calculateOptimalWindowLevel(vtkImageData* imageData, double& window, double& level);
    static void applyWindowLevel(vtkImageData* imageData, double window, double level);
    
    // 几何计算
    static double calculateDistance(const double point1[3], const double point2[3]);
    static void calculateCenter(vtkDataSet* dataSet, double center[3]);
    static double calculateVolume(vtkPolyData* polyData);
    static double calculateSurfaceArea(vtkPolyData* polyData);

private:
    VTKUtils() = delete;  // 静态工具类，不允许实例化
    ~VTKUtils() = delete;
    VTKUtils(const VTKUtils&) = delete;
    VTKUtils& operator=(const VTKUtils&) = delete;
};

/**
 * @brief VTK智能指针辅助类
 * 
 * 简化VTK智能指针的使用
 */
template<typename T>
class VTKSmartPointer {
public:
    VTKSmartPointer() : ptr(nullptr) {}
    
    explicit VTKSmartPointer(T* p) : ptr(p) {
        if (ptr) ptr->Register(nullptr);
    }
    
    VTKSmartPointer(const VTKSmartPointer& other) : ptr(other.ptr) {
        if (ptr) ptr->Register(nullptr);
    }
    
    ~VTKSmartPointer() {
        if (ptr) ptr->UnRegister(nullptr);
    }
    
    VTKSmartPointer& operator=(const VTKSmartPointer& other) {
        if (this != &other) {
            if (ptr) ptr->UnRegister(nullptr);
            ptr = other.ptr;
            if (ptr) ptr->Register(nullptr);
        }
        return *this;
    }
    
    T* Get() const { return ptr; }
    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }
    operator T*() const { return ptr; }
    
    bool IsNull() const { return ptr == nullptr; }
    
    void Reset(T* p = nullptr) {
        if (ptr) ptr->UnRegister(nullptr);
        ptr = p;
        if (ptr) ptr->Register(nullptr);
    }

private:
    T* ptr;
};

} // namespace MedicalImaging

#endif // VTKUTILS_H
