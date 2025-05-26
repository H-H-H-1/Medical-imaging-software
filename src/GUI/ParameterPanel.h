/**
 * @file ParameterPanel.h
 * @brief 医学影像软件参数控制面板头文件
 * @details 定义了一个多功能参数控制面板，用于医学影像的显示、处理和配准操作
 * 
 * 文件路径: f:\xsz-VTK\code\Medical imaging software\src\GUI\ParameterPanel.h
 */

#ifndef PARAMETERPANEL_H
#define PARAMETERPANEL_H

#include <QWidget>
#include <QScopedPointer>
#include <QString>

// Qt类的前向声明
class QTabWidget;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSlider;
class QComboBox;
class QDoubleSpinBox;
class QSpinBox;
class QCheckBox;

namespace MedicalImaging {

class ParameterPanelPrivate;

/**
 * @class ParameterPanel
 * @brief 医学影像软件的主要参数控制面板
 * @details 这个类提供了一个多标签页的界面，包含以下功能模块：
 * 
 * 1. 图像控制标签页：
 *    - 窗宽窗位调整：控制图像显示的亮度和对比度
 *    - 阈值设置：设置图像分割的上下阈值
 *    - 透明度控制：调整3D渲染中的透明度
 * 
 * 2. 滤波处理标签页：
 *    - 高斯滤波：平滑去噪，可调整sigma值和核大小
 *    - 中值滤波：去除椒盐噪声，可调整核大小
 *    - 锐化滤波：增强图像边缘细节
 * 
 * 3. 配准标签页：
 *    - 固定图像和移动图像选择
 *    - 配准算法选择
 *    - 配准参数控制
 * 
 * 4. 渲染/测量标签页：
 *    - 测量工具控制
 *    - 渲染参数设置
 * 
 * 该类使用PImpl设计模式来隐藏实现细节，通过信号槽机制与其他组件通信。
 */
class ParameterPanel : public QWidget
{
    Q_OBJECT

public:
    // 添加缺失的枚举定义
    enum MeasurementTool { 
        Distance, 
        Angle, 
        Area 
    };
    
    /**
     * @brief 构造函数
     * @param parent 父组件指针，默认为nullptr
     */
    explicit ParameterPanel(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     * @details 自动清理PImpl指针和相关资源
     */
    ~ParameterPanel();

    // ========== 公共接口方法 ==========
    void setWindowLevel(double window, double level);
    void getWindowLevel(double& window, double& level) const;

    /**
     * @brief 设置窗宽窗位的取值范围
     * @param minVal 最小值
     * @param maxVal 最大值
     * @details 用于初始化或更新窗宽窗位滑动条的取值范围
     */
    void setWindowLevelRange(double minWindow, double maxWindow,
                                       double minLevel, double maxLevel) ;

    void setThreshold(double lower, double upper) ;
    void getThreshold(double& lower, double& upper) const;
    
    /**
     * @brief 设置阈值的取值范围
     * @param minVal 最小阈值
     * @param maxVal 最大阈值
     * @details 用于图像分割时设置阈值范围
     */
    void setThresholdRange(double minVal, double maxVal);
    void setOpacity(double opacity);
    double getOpacity() const;
    void setColormap(const QString& colormapName);
    QString getColormap() const;


    void setGaussianSigma(double sigma);
    void setGaussianKernelSize(int kernelSize);

    void setMedianKernelSize(int kernelSize);

    int getGaussianKernelSize() const;

    int getMedianKernelSize() const;

    void setFixedImageFile(const QString& filePath);
    QString getFixedImageFile() const;

    void setMovingImageFile(const QString& filePath);
    QString getMovingImageFile() const; 

    void setRegistrationAlgorithm(const QString& algorithm);
    QString getRegistrationAlgorithm() const;   

    void enableWindowLevelControls(bool enabled);

    void enableThresholdControls(bool enabled);
    void enableFilterControls(bool enabled);
    void enableRegistrationControls(bool enabled);

    void setSliceRange(int min, int max);
    void setSlicePosition(int slice);
    int getSlicePosition() const;

    void setZoom(double zoom);
    double getZoom() const;

    void setViewType(int viewType) ;
    int getViewType() const ;

    void updateMeasurementResult(const QString& result) ;

    /**
     * @brief 设置透明度的取值范围
     * @param minVal 最小透明度（0.0为完全透明）
     * @param maxVal 最大透明度（1.0为完全不透明）
     */
    void setOpacityRange(double minVal, double maxVal);

    /**
     * @brief 设置当前窗宽值
     * @param window 窗宽值
     * @details 更新UI显示并发出相应信号
     */
    void setCurrentWindow(double window);
    
    /**
     * @brief 设置当前窗位值
     * @param level 窗位值
     */
    void setCurrentLevel(double level);
    
    /**
     * @brief 设置当前阈值
     * @param threshold 阈值
     */
    void setCurrentThreshold(double threshold);
    
    /**
     * @brief 设置当前透明度
     * @param opacity 透明度值（0.0-1.0）
     */
    void setCurrentOpacity(double opacity);
    
    /**
     * @brief 更新显示
     * @details 刷新所有UI组件的显示状态
     */
    void updateDisplay();

    /**
     * @brief 更新当前切片信息
     * @details 更新切片标签显示，格式为 "当前切片 / 总切片数"
     */
    void updateSliceInfo();

    void onWindowLevelChanged(double window, double level);

    void onThresholdChanged(double lower, double upper) ;

    void onOpacityChanged(double opacity) ;

    /**
     * @brief 重置所有参数到默认值
     */
    void resetParameters();

public slots:
    // ========== 公共槽函数 ==========
    // 这些槽函数可以从外部调用或连接到其他组件
    
    /**
     * @brief 应用高斯滤波槽函数
     * @details 响应高斯滤波按钮点击，获取当前sigma和核大小参数
     * 发出gaussianFilterRequested信号
     */
    void onApplyGaussianFilter();
    
    /**
     * @brief 应用中值滤波槽函数
     * @details 响应中值滤波按钮点击，获取当前核大小参数
     * 发出medianFilterRequested信号
     */
    void onApplyMedianFilter();
    
    /**
     * @brief 应用锐化滤波槽函数
     * @details 响应锐化滤波按钮点击
     * 发出sharpenFilterRequested信号
     */
    void onApplySharpenFilter();
    
    /**
     * @brief 开始配准槽函数
     * @details 响应开始配准按钮点击，收集配准参数
     * 发出startRegistrationRequested信号
     */
    void onStartRegistration();
    
    /**
     * @brief 重置配准槽函数
     * @details 响应重置配准按钮点击
     * 发出resetRegistrationRequested信号
     */
    void onResetRegistration();
    
    /**
     * @brief 清除测量槽函数
     * @details 连接到清除测量按钮，清除所有测量标记
     * 发出clearMeasurementsRequested信号
     */
    void onClearMeasurements(); // 连接到clearMeasurementsButton

private slots:
    // ========== 私有槽函数 ==========
    // 用于处理UI元素的内部信号，不对外暴露
    
    /**
     * @brief 窗宽滑动条值改变槽函数
     * @param value 滑动条的整数值，需要转换为实际窗宽值
     * @details 响应窗宽滑动条变化，发出windowChanged信号
     */
    void onWindowChanged(int value);
    
    /**
     * @brief 窗位滑动条值改变槽函数
     * @param value 滑动条的整数值，需要转换为实际窗位值
     * @details 响应窗位滑动条变化，发出levelChanged信号
     */
    void onLevelChanged(int value);
    
    /**
     * @brief 阈值滑动条值改变槽函数
     * @param value 滑动条的整数值，需要转换为实际阈值
     * @details 响应阈值滑动条变化，发出thresholdChanged信号
     */
    void onLowerThresholdChanged(double value);
    void onUpperThresholdChanged(double value);
    
    /**
     * @brief 透明度滑动条值改变槽函数
     * @param value 滑动条的整数值，需要转换为0.0-1.0的透明度值
     * @details 响应透明度滑动条变化，发出opacityChanged信号
     */
    void onOpacitySliderChanged(int value); 

    void onColormapChanged(const QString& colormapName);

    /**
     * @brief 移动图像浏览按钮槽函数
     * @details 打开文件对话框选择移动图像文件
     * 发出movingImageChanged信号
     */
    void onMovingImageBrowse();

    // ========== 滤波控制相关槽函数 ==========
    
    /**
     * @brief 滤波类型下拉框改变槽函数
     * @param text 选中的滤波类型文本
     * @details 响应滤波类型选择变化，发出filterTypeChanged信号
     */
    void onFilterTypeComboChanged(const QString& text);
    
    /**
     * @brief 核大小数值框改变槽函数
     *
     */
    void onKernelSizeSpinBoxChanged(int value); // 用于QSpinBox
    
    /**
     * @brief Sigma值数值框改变槽函数
     * @param value Sigma双精度浮点值
     * @details 响应Sigma参数输入变化，发出sigmaChanged信号
     */
    void onSigmaDoubleSpinBoxChanged(double value); // 用于QDoubleSpinBox

    void onFixedImageBrowse() ;
    
    // ========== 配准控制相关槽函数 ==========
    
    /**
     * @brief 配准算法下拉框改变槽函数
     * @param text 选中的配准算法文本
     * @details 响应配准算法选择变化，发出registrationAlgorithmChanged信号
     */
    void onRegAlgorithmComboChanged(const QString& text);


signals:
    // ========== 信号声明 ==========
    // 用于通知应用程序其他部分参数变化和操作请求
    
    // ========== 图像显示参数信号 ==========
    
    /**
     * @brief 窗宽改变信号
     * @param window 新的窗宽值
     * @details 当用户调整窗宽滑动条时发出，用于更新图像显示
     */
    void windowChanged(double window);
    
    /**
     * @brief 窗位改变信号
     * @param level 新的窗位值
     * @details 当用户调整窗位滑动条时发出，用于更新图像显示
     */
    void levelChanged(double level);
    
    /**
     * @brief 阈值改变信号
     * @param lower 下阈值
     * @param upper 上阈值
     * @details 当用户调整阈值时发出，用于图像分割显示
     * 注意：确保此信号与.cpp文件中的实现匹配
     */
    void thresholdChanged(double lower, double upper); // 确保与.cpp匹配
    
    /**
     * @brief 透明度改变信号
     * @param opacity 新的透明度值（0.0-1.0）
     * @details 当用户调整透明度滑动条时发出，用于3D渲染
     */
    void opacityChanged(double opacity);
    
    // ========== 滤波处理请求信号 ==========
    
    /**
     * @brief 高斯滤波请求信号
     * @param sigma 高斯核的标准差
     * @param kernelSize 高斯核的大小
     * @details 当用户请求应用高斯滤波时发出
     */
    void gaussianFilterRequested(double sigma, int kernelSize);
    
    /**
     * @brief 中值滤波请求信号
     * @param kernelSize 中值滤波核的大小
     * @details 当用户请求应用中值滤波时发出
     */
    void medianFilterRequested(int kernelSize);
    
    /**
     * @brief 锐化滤波请求信号
     * @details 当用户请求应用锐化滤波时发出，使用默认锐化参数
     */
    void sharpenFilterRequested();
    
    // ========== 配准操作信号 ==========
    
    /**
     * @brief 配准参数改变信号
     * @param fixedImage 固定图像文件路径
     * @param movingImage 移动图像文件路径
     * @param algorithm 选择的配准算法
     * @details 当配准参数发生变化时发出
     */
    void registrationParametersChanged(const QString& fixedImage, const QString& movingImage, const QString& algorithm);
    
    /**
     * @brief 开始配准请求信号
     * @details 当用户点击开始配准按钮时发出
     */
    void startRegistrationRequested();
    
    /**
     * @brief 重置配准请求信号
     * @details 当用户点击重置配准按钮时发出
     */
    void resetRegistrationRequested();
    
    /**
     * @brief 清除测量请求信号
     * @details 当用户点击清除测量按钮时发出，清除所有测量标记
     * 注意：确保此信号正确声明
     */
    void clearMeasurementsRequested(); // 确保正确声明

    // ========== 新增UI控制/参数信号 ==========
    
    /**
     * @brief 滤波类型改变信号
     * @param filterType 选择的滤波类型
     * @details 当用户在滤波类型下拉框中选择不同选项时发出
     */
    void filterTypeChanged(const QString& filterType);
    
    /**
     * @brief 核大小改变信号
     * @param kernelSize 新的核大小值
     * @details 当用户调整滤波核大小时发出
     */
    void kernelSizeChanged(int kernelSize);
    
    /**
     * @brief Sigma值改变信号
     * @param sigma 新的sigma值
     * @details 当用户调整高斯滤波sigma参数时发出
     */
    void sigmaChanged(double sigma);
    
    /**
     * @brief 固定图像改变信号
     * @param filePath 新选择的固定图像文件路径
     * @details 当用户选择新的固定图像文件时发出
     */
    void fixedImageChanged(const QString& filePath);
    
    /**
     * @brief 移动图像改变信号
     * @param filePath 新选择的移动图像文件路径
     * @details 当用户选择新的移动图像文件时发出
     */
    void movingImageChanged(const QString& filePath);
    
    /**
     * @brief 配准算法改变信号
     * @param algorithm 新选择的配准算法
     * @details 当用户在配准算法下拉框中选择不同选项时发出
     */
    void registrationAlgorithmChanged(const QString& algorithm);

    /**
     * @brief 窗宽窗位改变信号
     * @param window 新的窗宽值
     * @param level 新的窗位值
     * @details 当用户调整窗宽窗位时发出，用于更新图像显示
     */
    void windowLevelChanged(double window, double level);

    /**
     * @brief 切片改变信号
     * @param slice 新的切片位置
     * @details 当用户调整切片滑动条时发出，用于更新当前切片
     */
    void sliceChanged(int slice);

    /**
     * @brief 缩放因子改变信号
     * @param zoom 新的缩放因子
     * @details 当用户调整缩放滑动条时发出，用于更新图像缩放
     */
    void zoomChanged(double zoom);

    /**
     * @brief 颜色映射表改变信号
     * @param colormap 新的颜色映射表名称
     * @details 当用户选择不同的颜色映射表时发出
     */
    void colormapChanged(const QString& colormap);

    /**
     * @brief 重置参数请求信号
     * @details 当用户点击重置按钮时发出，重置所有参数为默认值
     */
    void resetParametersRequested();

    /**
     * @brief 适应窗口请求信号
     * @details 当用户选择适应窗口时发出，调整图像以适应窗口大小
     */
    void fitToWindowRequested();

    /**
     * @brief 重置缩放请求信号
     * @details 当用户选择重置缩放时发出，恢复默认缩放级别
     */
    void resetZoomRequested();

    /**
     * @brief 视图类型改变信号
     * @param viewType 新的视图类型
     * @details 当用户在视图类型下拉框中选择不同选项时发出
     */
    void viewTypeChanged(int viewType);

    /**
     * @brief 显示坐标轴改变信号
     * @param enabled 是否显示坐标轴
     * @details 当用户勾选或取消勾选显示坐标轴时发出
     */
    void showAxisChanged(bool enabled);

    /**
     * @brief 显示刻度改变信号
     * @param enabled 是否显示刻度
     * @details 当用户勾选或取消勾选显示刻度时发出
     */
    void showScaleChanged(bool enabled);

    /**
     * @brief 亮度改变信号
     * @param brightness 新的亮度值
     * @details 当用户调整亮度滑动条时发出，用于更新图像亮度
     */
    void brightnessChanged(int brightness);

    /**
     * @brief 对比度改变信号
     * @param contrast 新的对比度值
     * @details 当用户调整对比度滑动条时发出，用于更新图像对比度
     */
    void contrastChanged(int contrast);

    /**
     * @brief 反转颜色改变信号
     * @param inverted 是否反转颜色
     * @details 当用户勾选或取消勾选反转颜色时发出
     */
    void invertColorsChanged(bool inverted);

    // ========== 测量工具信号 ==========
    
    /**
     * @brief 测量工具选择信号
     * @param tool 选择的测量工具类型
     * @details 当用户选择不同的测量工具时发出
     */
    void measurementToolSelected(MeasurementTool tool);

private:
    // ========== 私有方法 ==========
    
    // ========== UI初始化方法 ==========
    
    /**
     * @brief 设置用户界面
     * @details 创建并布局所有UI组件，包括标签页、分组框、控件等
     * 在构造函数中调用，负责整个界面的初始化
     */
    void setupUI();
    
    /**
     * @brief 连接信号槽
     * @details 建立UI控件与槽函数之间的连接关系
     * 在setupUI()之后调用，确保所有信号槽正确连接
     */
    void connectSignals();

    // ========== 标签页创建方法 ==========
    
    /**
     * @brief 创建图像控制标签页
     * @return QWidget* 图像控制标签页组件指针
     * @details 包含窗宽窗位、阈值、透明度等图像显示控制
     */
    QWidget* createImageControlTab();
    
    /**
     * @brief 创建滤波处理标签页
     * @return QWidget* 滤波处理标签页组件指针
     * @details 包含各种图像滤波算法的参数控制和应用按钮
     */
    QWidget* createFilterProcessingTab();
    
    /**
     * @brief 创建配准标签页
     * @return QWidget* 配准标签页组件指针
     * @details 包含图像配准的参数设置和控制按钮
     */
    QWidget* createRegistrationTab();
    
    /**
     * @brief 创建渲染标签页
     * @return QWidget* 渲染/测量标签页组件指针
     * @details 也可命名为createMeasurementAndSettingsTab，包含测量工具和渲染设置
     */
    QWidget* createRenderingTab(); // 或更具描述性的名称如createMeasurementAndSettingsTab

    QGroupBox* createWindowLevelGroup();

    QGroupBox* createSliceGroup();

    QGroupBox* createZoomGroup();

    QGroupBox* createViewGroup();

    /**
     * @brief 创建阈值分组框
     * @return QGroupBox* 阈值分组框指针
     * @details 包含阈值设置相关的滑动条和标签
     */
    QGroupBox* createThresholdGroup();

      /**
     * @brief 创建滤波选项分组框
     * @return QGroupBox* 滤波选项分组框指针
     * @details 为清晰起见重命名，包含滤波类型选择和参数设置
     */
    QGroupBox* createFilterOptionsGroup(); // 为清晰起见重命名

       /**
     * @brief 创建配准参数分组框
     * @return QGroupBox* 配准参数分组框指针
     * @details 为清晰起见重命名，包含配准算法选择和参数设置
     */
    QGroupBox* createRegistrationParamsGroup(); // 为清晰起见重命名
    
       /**
     * @brief 创建图像处理分组框
     * @return QGroupBox* 图像处理分组框指针
     * @details 包含窗宽窗位控制相关的UI组件
     */
    QGroupBox* createImageProcessingGroup();

    /**
     * @brief 创建测量分组框
     * @return QGroupBox* 测量分组框指针
     * @details 包含测量工具的控制按钮和显示结果
     */
    QGroupBox* createMeasurementGroup();



    // ========== PImpl设计模式 ==========

    QScopedPointer<ParameterPanelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ParameterPanel)  // 这里应该是ParameterPanel，不是其他类名
};

} // namespace MedicalImaging

/**
 * @brief ParameterPanel类定义结束
 * @details 这个类是医学影像软件中的核心UI组件，提供了完整的参数控制界面。
 * 
 * 使用说明：
 * 1. 实例化此类并将其添加到主窗口布局中
 * 2. 连接相关信号到对应的处理函数
 * 3. 调用公共接口方法设置初始参数范围
 * 4. 响应信号更新图像显示和处理参数
 * 
 * 注意事项：
 * - 确保在使用前正确初始化VTK和Qt环境
 * - 信号槽连接需要在setupUI()之后进行
 * - PImpl模式要求在.cpp文件中完整定义Impl结构体
 */

#endif // PARAMETERPANEL_H
