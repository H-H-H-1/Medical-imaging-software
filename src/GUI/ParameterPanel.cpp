#include "ParameterPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSplitter>
#include <QTabWidget>
#include <QFileDialog>
#include <QDebug>
#include <QLineEdit>
#include <memory>

namespace MedicalImaging{


struct ParameterPanelPrivate {
    // 修正构造函数
    explicit ParameterPanelPrivate(ParameterPanel* parent) : q_ptr(parent) {}
    
    ParameterPanel* q_ptr;
    Q_DECLARE_PUBLIC(ParameterPanel) ;

    // 窗宽窗位控件
    QSlider* windowSlider = nullptr;
    QSlider* levelSlider = nullptr;
    QDoubleSpinBox* windowSpinBox = nullptr;
    QDoubleSpinBox* levelSpinBox = nullptr;
    
    // 切片控件
    QSlider* sliceSlider = nullptr;
    QSpinBox* sliceSpinBox = nullptr;
    QLabel* sliceInfoLabel = nullptr;
    
    // 缩放控件
    QSlider* zoomSlider = nullptr;
    QDoubleSpinBox* zoomSpinBox = nullptr;
    QPushButton* resetZoomButton = nullptr;
    QPushButton* fitToWindowButton = nullptr;
    
    // 视图控件
    QComboBox* viewTypeCombo = nullptr;
    QCheckBox* showAxisCheckBox = nullptr;
    QCheckBox* showScaleCheckBox = nullptr;
    
    // 图像处理控件 (渲染标签页)
    QSlider* brightnessSlider = nullptr;
    QSlider* contrastSlider = nullptr;
    QComboBox* colormapCombo = nullptr; // 用于渲染的颜色图
    QCheckBox* invertColorsCheckBox = nullptr;
    
    // 测量工具控件 (渲染标签页)
    QPushButton* measureDistanceButton = nullptr;
    QPushButton* measureAngleButton = nullptr;
    QPushButton* measureAreaButton = nullptr;
    QLabel* measurementResultLabel = nullptr;

    // 阈值控件 (滤波处理标签页)
    QSlider* lowerThresholdSlider = nullptr;
    QSlider* upperThresholdSlider = nullptr;
    QDoubleSpinBox* lowerThresholdSpinBox = nullptr;
    QDoubleSpinBox* upperThresholdSpinBox = nullptr;

    // 透明度控件 (渲染标签页)
    QSlider* opacitySlider = nullptr;
    QDoubleSpinBox* opacitySpinBox = nullptr;

    // 滤波控件 (滤波处理标签页)
    QComboBox* filterTypeCombo = nullptr; // 比如高斯、中值等
    QDoubleSpinBox* gaussianSigmaSpinBox = nullptr;
    QSpinBox* gaussianKernelSizeSpinBox = nullptr; // Added for Gaussian Kernel
    QSpinBox* medianKernelSizeSpinBox = nullptr; // Renamed from medianRadiusSpinBox for clarity
    QPushButton* applyFilterButton = nullptr; // Generic apply button
    // QPushButton* applyEdgeDetectionButton = nullptr; // Specific buttons might be better
    // QPushButton* applyMorphologyButton = nullptr;
    // QComboBox* morphologyOperationCombo = nullptr;
    // QSpinBox* morphologyRadiusSpinBox = nullptr;

    // 配准控件 (配准标签页)
    QLineEdit* fixedImageLineEdit = nullptr;
    QPushButton* fixedImageBrowseButton = nullptr;
    QLineEdit* movingImageLineEdit = nullptr;
    QPushButton* movingImageBrowseButton = nullptr;
    QComboBox* registrationAlgorithmCombo = nullptr; // Renamed from registrationMethodCombo
    // QDoubleSpinBox* registrationToleranceSpinBox = nullptr; // May not be needed if part of algorithm params
    QPushButton* startRegistrationButton = nullptr; // Renamed from applyRegistrationButton
    QPushButton* resetRegistrationButton = nullptr; // New button

    // 当前参数值
    double currentWindow = 400.0;
    double currentLevel = 40.0;
    double minWindowRange = 1.0, maxWindowRange = 2000.0;
    double minLevelRange = -1000.0, maxLevelRange = 1000.0;

    int currentSlice = 0;
    int maxSlice = 100; // 默认值，应由外部设置
    int minSlice = 0;   // 默认值，应由外部设置
    double currentZoom = 1.0;

    double currentLowerThreshold = 0.0;
    double currentUpperThreshold = 255.0;
    double minThresholdRange = 0.0, maxThresholdRange = 1000.0; // 示例范围

    double currentOpacity = 1.0; // 0.0 (透明) to 1.0 (不透明)
    
    QString currentColormap = "灰度";

    double currentGaussianSigma = 1.0;
    int currentGaussianKernelSize = 3; // Default Gaussian kernel
    int currentMedianKernelSize = 3;   // Default Median kernel    // QString currentRegistrationMethod = "ICP"; // Renamed to currentRegAlgorithm
    // double currentRegistrationTolerance = 0.01;
    QString currentFixedImageFile;
    QString currentMovingImageFile;
    QString currentRegAlgorithm = "ICP";



};

ParameterPanel::ParameterPanel(QWidget* parent)
    : QWidget(parent), d_ptr(new ParameterPanelPrivate(this)) {

    setupUI();
    connectSignals();
    updateDisplay();
}

ParameterPanel::~ParameterPanel() = default;

void ParameterPanel::setupUI() {
    Q_D(ParameterPanel);
    
    setFixedWidth(300); // 稍微加宽以容纳标签页和控件
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(5, 5, 5, 5);
    
    QTabWidget* tabWidget = new QTabWidget(this);

    // 创建各个标签页
    tabWidget->addTab(createImageControlTab(), "图像控制");
    tabWidget->addTab(createFilterProcessingTab(), "滤波处理");
    tabWidget->addTab(createRegistrationTab(), "配准");
    tabWidget->addTab(createRenderingTab(), "渲染与测量");
    
    mainLayout->addWidget(tabWidget);
}

// --- 实现 setup...Tab 和 create...Group 方法 ---

// 图像控制标签页
QWidget* ParameterPanel::createImageControlTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);

    layout->addWidget(createWindowLevelGroup());
    layout->addWidget(createSliceGroup());
    layout->addWidget(createZoomGroup());
    layout->addWidget(createViewGroup());
    layout->addStretch();
    return tab;
}

// 滤波处理标签页
QWidget* ParameterPanel::createFilterProcessingTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);

    layout->addWidget(createThresholdGroup());
    // layout->addWidget(createFilterGroup()); // Original general filter group
    layout->addWidget(createFilterOptionsGroup()); // New more detailed filter group
    layout->addStretch();
    return tab;
}

// 配准标签页
QWidget* ParameterPanel::createRegistrationTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);

    // layout->addWidget(createRegistrationGroup()); // Original general registration group
    layout->addWidget(createRegistrationParamsGroup()); // New more detailed registration group
    layout->addStretch();
    return tab;
}

// 渲染与测量标签页 (合并原图像处理和测量)
QWidget* ParameterPanel::createRenderingTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 10, 10, 10);

    layout->addWidget(createImageProcessingGroup()); // 包含亮度、对比度、颜色图、反色、透明度
    layout->addWidget(createMeasurementGroup());
    layout->addStretch();
    return tab;
}

QGroupBox* ParameterPanel::createWindowLevelGroup() {
    Q_D(ParameterPanel);
    
    QGroupBox* group = new QGroupBox("窗宽窗位", this);
    QGridLayout* layout = new QGridLayout(group);
    
    // 窗宽控件
    layout->addWidget(new QLabel("窗宽:"), 0, 0);
    d->windowSlider = new QSlider(Qt::Horizontal);
    d->windowSlider->setRange(static_cast<int>(d->minWindowRange), static_cast<int>(d->maxWindowRange));
    d->windowSlider->setValue(static_cast<int>(d->currentWindow));
    layout->addWidget(d->windowSlider, 0, 1);
    
    d->windowSpinBox = new QDoubleSpinBox();
    d->windowSpinBox->setRange(d->minWindowRange, d->maxWindowRange);
    d->windowSpinBox->setValue(d->currentWindow);
    d->windowSpinBox->setSuffix(" HU");
    layout->addWidget(d->windowSpinBox, 0, 2);
    
    // 窗位控件
    layout->addWidget(new QLabel("窗位:"), 1, 0);
    d->levelSlider = new QSlider(Qt::Horizontal);
    d->levelSlider->setRange(static_cast<int>(d->minLevelRange), static_cast<int>(d->maxLevelRange));
    d->levelSlider->setValue(static_cast<int>(d->currentLevel));
    layout->addWidget(d->levelSlider, 1, 1);
    
    d->levelSpinBox = new QDoubleSpinBox();
    d->levelSpinBox->setRange(d->minLevelRange, d->maxLevelRange);
    d->levelSpinBox->setValue(d->currentLevel);
    d->levelSpinBox->setSuffix(" HU");
    layout->addWidget(d->levelSpinBox, 1, 2);
      return group;
}

QGroupBox* ParameterPanel::createSliceGroup() {
    Q_D(ParameterPanel);
    
    QGroupBox* group = new QGroupBox("切片控制", this);
    QGridLayout* layout = new QGridLayout(group);
    
    layout->addWidget(new QLabel("切片:"), 0, 0);
    
    d->sliceSlider = new QSlider(Qt::Horizontal);
    d->sliceSlider->setRange(0, d->maxSlice);
    d->sliceSlider->setValue(d->currentSlice);
    layout->addWidget(d->sliceSlider, 0, 1);
    
    d->sliceSpinBox = new QSpinBox();
    d->sliceSpinBox->setRange(0, d->maxSlice);
    d->sliceSpinBox->setValue(d->currentSlice);
    layout->addWidget(d->sliceSpinBox, 0, 2);
    
    // 切片信息标签
    d->sliceInfoLabel = new QLabel(QString("切片 %1/%2").arg(d->currentSlice).arg(d->maxSlice));
    layout->addWidget(d->sliceInfoLabel, 1, 0, 1, 3);
    
    return group;
}

QGroupBox* ParameterPanel::createZoomGroup() {
    Q_D(ParameterPanel);
    
    QGroupBox* group = new QGroupBox("缩放控制", this);
    QGridLayout* layout = new QGridLayout(group);
    
    layout->addWidget(new QLabel("缩放:"), 0, 0);
    
    d->zoomSlider = new QSlider(Qt::Horizontal);
    d->zoomSlider->setRange(10, 500); // 0.1x to 5.0x
    d->zoomSlider->setValue(static_cast<int>(d->currentZoom * 100));
    layout->addWidget(d->zoomSlider, 0, 1);
    
    d->zoomSpinBox = new QDoubleSpinBox();
    d->zoomSpinBox->setRange(0.1, 5.0);
    d->zoomSpinBox->setSingleStep(0.1);
    d->zoomSpinBox->setValue(d->currentZoom);
    d->zoomSpinBox->setSuffix("x");
    layout->addWidget(d->zoomSpinBox, 0, 2);
    
    // 缩放按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    d->resetZoomButton = new QPushButton("重置");
    d->fitToWindowButton = new QPushButton("适应窗口");
    buttonLayout->addWidget(d->resetZoomButton);
    buttonLayout->addWidget(d->fitToWindowButton);
    layout->addLayout(buttonLayout, 1, 0, 1, 3);
    
    return group;
}

QGroupBox* ParameterPanel::createViewGroup() {
    Q_D(ParameterPanel);
    
    QGroupBox* group = new QGroupBox("视图控制", this);
    QVBoxLayout* layout = new QVBoxLayout(group);
    
    // 视图类型选择
    QHBoxLayout* viewTypeLayout = new QHBoxLayout();
    viewTypeLayout->addWidget(new QLabel("视图类型:"));
    d->viewTypeCombo = new QComboBox();
    d->viewTypeCombo->addItems(QStringList() << "轴位" << "冠状位" << "矢状位" << "3D体积");
    viewTypeLayout->addWidget(d->viewTypeCombo);
    layout->addLayout(viewTypeLayout);
    
    // 显示选项
    d->showAxisCheckBox = new QCheckBox("显示坐标轴");
    d->showAxisCheckBox->setChecked(true);
    layout->addWidget(d->showAxisCheckBox);
    
    d->showScaleCheckBox = new QCheckBox("显示比例尺");
    d->showScaleCheckBox->setChecked(false);
    layout->addWidget(d->showScaleCheckBox);
    
    return group;
}

QGroupBox* ParameterPanel::createThresholdGroup() {
    Q_D(ParameterPanel);
    
    QGroupBox* group = new QGroupBox("阈值分割", this);
    QGridLayout* layout = new QGridLayout(group);

    // 低阈值
    layout->addWidget(new QLabel("低阈值:"), 0, 0);
    d->lowerThresholdSlider = new QSlider(Qt::Horizontal);
    d->lowerThresholdSlider->setRange(static_cast<int>(d->minThresholdRange), static_cast<int>(d->maxThresholdRange));
    d->lowerThresholdSlider->setValue(static_cast<int>(d->currentLowerThreshold));
    layout->addWidget(d->lowerThresholdSlider, 0, 1);
    d->lowerThresholdSpinBox = new QDoubleSpinBox();
    d->lowerThresholdSpinBox->setRange(d->minThresholdRange, d->maxThresholdRange);
    d->lowerThresholdSpinBox->setValue(d->currentLowerThreshold);
    layout->addWidget(d->lowerThresholdSpinBox, 0, 2);

    // 高阈值
    layout->addWidget(new QLabel("高阈值:"), 1, 0);
    d->upperThresholdSlider = new QSlider(Qt::Horizontal);
    d->upperThresholdSlider->setRange(static_cast<int>(d->minThresholdRange), static_cast<int>(d->maxThresholdRange));
    d->upperThresholdSlider->setValue(static_cast<int>(d->currentUpperThreshold));
    layout->addWidget(d->upperThresholdSlider, 1, 1);
    d->upperThresholdSpinBox = new QDoubleSpinBox();
    d->upperThresholdSpinBox->setRange(d->minThresholdRange, d->maxThresholdRange);
    d->upperThresholdSpinBox->setValue(d->currentUpperThreshold);
    layout->addWidget(d->upperThresholdSpinBox, 1, 2);
    
    return group;
}

QGroupBox* ParameterPanel::createFilterOptionsGroup() {
    Q_D(ParameterPanel);
    
    QGroupBox* group = new QGroupBox("滤波选项", this);
    QGridLayout* layout = new QGridLayout(group);
    layout->setColumnStretch(1, 1); // Allow spinboxes/lineedits to expand

    // Filter Type
    layout->addWidget(new QLabel("滤波类型:"), 0, 0);
    d->filterTypeCombo = new QComboBox();
    d->filterTypeCombo->addItems(QStringList() << "高斯滤波" << "中值滤波" << "锐化滤波");
    layout->addWidget(d->filterTypeCombo, 0, 1, 1, 2);

    // Gaussian Sigma
    layout->addWidget(new QLabel("高斯 Sigma:"), 1, 0);
    d->gaussianSigmaSpinBox = new QDoubleSpinBox();
    d->gaussianSigmaSpinBox->setRange(0.1, 10.0);
    d->gaussianSigmaSpinBox->setSingleStep(0.1);
    d->gaussianSigmaSpinBox->setValue(d->currentGaussianSigma);
    layout->addWidget(d->gaussianSigmaSpinBox, 1, 1);

    // Gaussian Kernel Size
    layout->addWidget(new QLabel("高斯核大小:"), 2, 0);
    d->gaussianKernelSizeSpinBox = new QSpinBox();
    d->gaussianKernelSizeSpinBox->setRange(1, 21); // Odd numbers usually
    d->gaussianKernelSizeSpinBox->setSingleStep(2);
    d->gaussianKernelSizeSpinBox->setValue(d->currentGaussianKernelSize);
    layout->addWidget(d->gaussianKernelSizeSpinBox, 2, 1);

    // Median Kernel Size
    layout->addWidget(new QLabel("中值核大小:"), 3, 0);
    d->medianKernelSizeSpinBox = new QSpinBox();
    d->medianKernelSizeSpinBox->setRange(1, 21); // Odd numbers usually
    d->medianKernelSizeSpinBox->setSingleStep(2);
    d->medianKernelSizeSpinBox->setValue(d->currentMedianKernelSize);
    layout->addWidget(d->medianKernelSizeSpinBox, 3, 1);
    
    // Initially hide/disable kernel/sigma based on filter type
    // This will be handled by onFilterTypeComboChanged slot

    // Apply Filter Button
    d->applyFilterButton = new QPushButton("应用当前滤波");
    layout->addWidget(d->applyFilterButton, 4, 0, 1, 3); // Span across columns

    // Enable/disable controls based on initial filter type
    QString currentFilter = d->filterTypeCombo->currentText();
    d->gaussianSigmaSpinBox->setVisible(currentFilter == "高斯滤波");
    d->gaussianKernelSizeSpinBox->setVisible(currentFilter == "高斯滤波");
    d->medianKernelSizeSpinBox->setVisible(currentFilter == "中值滤波");
    // Sharpen might not have parameters here, or they could be added

    return group;
}

QGroupBox* ParameterPanel::createRegistrationParamsGroup() {
    Q_D(ParameterPanel);
    
    QGroupBox* group = new QGroupBox("配准参数", this);
    QGridLayout* layout = new QGridLayout(group);
    layout->setColumnStretch(1, 1);

    // Fixed Image
    layout->addWidget(new QLabel("固定图像:"), 0, 0);
    d->fixedImageLineEdit = new QLineEdit();
    d->fixedImageLineEdit->setReadOnly(true);
    layout->addWidget(d->fixedImageLineEdit, 0, 1);
    d->fixedImageBrowseButton = new QPushButton("浏览...");
    layout->addWidget(d->fixedImageBrowseButton, 0, 2);

    // Moving Image
    layout->addWidget(new QLabel("移动图像:"), 1, 0);
    d->movingImageLineEdit = new QLineEdit();
    d->movingImageLineEdit->setReadOnly(true);
    layout->addWidget(d->movingImageLineEdit, 1, 1);
    d->movingImageBrowseButton = new QPushButton("浏览...");
    layout->addWidget(d->movingImageBrowseButton, 1, 2);

    // Registration Algorithm
    layout->addWidget(new QLabel("配准算法:"), 2, 0);
    d->registrationAlgorithmCombo = new QComboBox();
    d->registrationAlgorithmCombo->addItems(QStringList() << "ICP" << "仿射" << "B样条"); // Example algorithms
    layout->addWidget(d->registrationAlgorithmCombo, 2, 1, 1, 2);

    // Action Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    d->startRegistrationButton = new QPushButton("开始配准");
    d->resetRegistrationButton = new QPushButton("重置配准");
    buttonLayout->addWidget(d->startRegistrationButton);
    buttonLayout->addWidget(d->resetRegistrationButton);
    layout->addLayout(buttonLayout, 3, 0, 1, 3);

    return group;
}

QGroupBox* ParameterPanel::createImageProcessingGroup() {
    Q_D(ParameterPanel);
    
    QGroupBox* group = new QGroupBox("图像效果", this);
    QGridLayout* layout = new QGridLayout(group);
    layout->setColumnStretch(1, 1); // Allow sliders to expand more
    
    // 亮度调节
    layout->addWidget(new QLabel("亮度:"), 0, 0);
    d->brightnessSlider = new QSlider(Qt::Horizontal);
    d->brightnessSlider->setRange(-100, 100);
    d->brightnessSlider->setValue(0);
    layout->addWidget(d->brightnessSlider, 0, 1);
    
    // 对比度调节
    layout->addWidget(new QLabel("对比度:"), 1, 0);
    d->contrastSlider = new QSlider(Qt::Horizontal);
    d->contrastSlider->setRange(-100, 100);
    d->contrastSlider->setValue(0);
    layout->addWidget(d->contrastSlider, 1, 1);
    
    // 颜色映射
    layout->addWidget(new QLabel("颜色映射:"), 2, 0);
    d->colormapCombo = new QComboBox();
    d->colormapCombo->addItems(QStringList() << "灰度" << "彩虹" << "热图" << "冷暖");
    layout->addWidget(d->colormapCombo, 2, 1);
    
    // 反色选项
    d->invertColorsCheckBox = new QCheckBox("反色显示");
    layout->addWidget(d->invertColorsCheckBox, 3, 0, 1, 2);

    // 透明度控制
    layout->addWidget(new QLabel("透明度:"), 4, 0);
    d->opacitySlider = new QSlider(Qt::Horizontal);
    d->opacitySlider->setRange(0, 100); 
    d->opacitySlider->setValue(static_cast<int>(d->currentOpacity * 100));
    layout->addWidget(d->opacitySlider, 4, 1); // Occupy 1 column
    d->opacitySpinBox = new QDoubleSpinBox();
    d->opacitySpinBox->setRange(0.0, 1.0);
    d->opacitySpinBox->setSingleStep(0.01);
    d->opacitySpinBox->setValue(d->currentOpacity);
    layout->addWidget(d->opacitySpinBox, 4, 2); // Occupy 1 column
    
    return group;
}

QGroupBox* ParameterPanel::createMeasurementGroup() {
    Q_D(ParameterPanel);
    
    QGroupBox* group = new QGroupBox("测量工具", this);
    QVBoxLayout* layout = new QVBoxLayout(group);
    
    // 测量按钮
    QHBoxLayout* buttonLayout1 = new QHBoxLayout();
    d->measureDistanceButton = new QPushButton("距离");
    d->measureAngleButton = new QPushButton("角度");
    buttonLayout1->addWidget(d->measureDistanceButton);
    buttonLayout1->addWidget(d->measureAngleButton);
    layout->addLayout(buttonLayout1);
    
    QHBoxLayout* buttonLayout2 = new QHBoxLayout();
    d->measureAreaButton = new QPushButton("面积");
    QPushButton* clearMeasurementsButton = new QPushButton("清除测量"); 
    // Connect the clearMeasurementsButton
    connect(clearMeasurementsButton, &QPushButton::clicked, this, &ParameterPanel::onClearMeasurements); // Corrected slot name
    buttonLayout2->addWidget(d->measureAreaButton);
    buttonLayout2->addWidget(clearMeasurementsButton); // Add the button to the layout
    layout->addLayout(buttonLayout2);
    
    // 测量结果显示
    d->measurementResultLabel = new QLabel("测量结果将显示在此处");
    d->measurementResultLabel->setWordWrap(true);
    d->measurementResultLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 5px; border: 1px solid #ccc; }");
    layout->addWidget(d->measurementResultLabel);
    
    return group;
}

void ParameterPanel::connectSignals() {
    Q_D(ParameterPanel);
    
    // 窗宽窗位信号连接
    connect(d->windowSlider, &QSlider::valueChanged, this, &ParameterPanel::onWindowChanged);
    connect(d->windowSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this, d](double value) {
        d->currentWindow = value;
        d->windowSlider->setValue(static_cast<int>(value)); // 同步Slider
        emit windowLevelChanged(d->currentWindow, d->currentLevel);
    });
    
    connect(d->levelSlider, &QSlider::valueChanged, this, &ParameterPanel::onLevelChanged);    connect(d->levelSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this, d](double value) {
        d->currentLevel = value;
        d->levelSlider->setValue(static_cast<int>(value)); // 同步Slider
        emit windowLevelChanged(d->currentWindow, d->currentLevel);
    });
      // 切片信号连接
    connect(d->sliceSlider, &QSlider::valueChanged, this, [this, d](int value) {
        d->currentSlice = value;
        d->sliceSpinBox->setValue(value);
        updateSliceInfo();
        emit sliceChanged(value);
    });
      connect(d->sliceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, [this, d](int value) {
        d->currentSlice = value;
        d->sliceSlider->setValue(value);
        updateSliceInfo();
        emit sliceChanged(value);
    });
      // 缩放信号连接
    connect(d->zoomSlider, &QSlider::valueChanged, this, [this, d](int value) {
        d->currentZoom = value / 100.0;
        d->zoomSpinBox->setValue(d->currentZoom);
        emit zoomChanged(d->currentZoom);
    });
      connect(d->zoomSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this, d](double value) {
        d->currentZoom = value;
        d->zoomSlider->setValue(static_cast<int>(value * 100));
        emit zoomChanged(d->currentZoom);
    });
    
    connect(d->resetZoomButton, &QPushButton::clicked, this, [this]() {
        setZoom(1.0); // 使用public方法
        emit resetZoomRequested();
    });
    
    connect(d->fitToWindowButton, &QPushButton::clicked, this, &ParameterPanel::fitToWindowRequested);
    
    // 视图控制信号连接
    connect(d->viewTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &ParameterPanel::viewTypeChanged);
    
    connect(d->showAxisCheckBox, &QCheckBox::toggled, this, &ParameterPanel::showAxisChanged);
    connect(d->showScaleCheckBox, &QCheckBox::toggled, this, &ParameterPanel::showScaleChanged);
    
    // 图像效果 (渲染标签页) 信号连接
    connect(d->brightnessSlider, &QSlider::valueChanged, this, &ParameterPanel::brightnessChanged);
    connect(d->contrastSlider, &QSlider::valueChanged, this, &ParameterPanel::contrastChanged);
    // connect(d->colormapCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ParameterPanel::colormapChanged); // 这个信号参数是int, 我们需要QString
    connect(d->colormapCombo, &QComboBox::currentTextChanged, this, &ParameterPanel::onColormapChanged); // 使用currentTextChanged获取QString
    connect(d->invertColorsCheckBox, &QCheckBox::toggled, this, &ParameterPanel::invertColorsChanged);

    // 透明度信号连接
    connect(d->opacitySlider, &QSlider::valueChanged, this, &ParameterPanel::onOpacitySliderChanged);
    connect(d->opacitySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, d](double value){
        d->currentOpacity = value;
        d->opacitySlider->setValue(static_cast<int>(value * 100));
        emit opacityChanged(d->currentOpacity);
    });

    // 阈值信号连接
    connect(d->lowerThresholdSlider, &QSlider::valueChanged, this, &ParameterPanel::onLowerThresholdChanged);
    connect(d->lowerThresholdSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, d](double value){
        d->currentLowerThreshold = value;
        d->lowerThresholdSlider->setValue(static_cast<int>(value));
        if (d->currentLowerThreshold > d->currentUpperThreshold) { // 确保下阈值不超过上阈值
            d->currentUpperThreshold = d->currentLowerThreshold;
            d->upperThresholdSlider->setValue(static_cast<int>(d->currentUpperThreshold));
            d->upperThresholdSpinBox->setValue(d->currentUpperThreshold);
        }
        emit thresholdChanged(d->currentLowerThreshold, d->currentUpperThreshold);
    });

    connect(d->upperThresholdSlider, &QSlider::valueChanged, this, &ParameterPanel::onUpperThresholdChanged);
    connect(d->upperThresholdSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this, d](double value){
        d->currentUpperThreshold = value;
        d->upperThresholdSlider->setValue(static_cast<int>(value));
         if (d->currentUpperThreshold < d->currentLowerThreshold) { // 确保上阈值不低于下阈值
            d->currentLowerThreshold = d->currentUpperThreshold;
            d->lowerThresholdSlider->setValue(static_cast<int>(d->currentLowerThreshold));
            d->lowerThresholdSpinBox->setValue(d->currentLowerThreshold);
        }
        emit thresholdChanged(d->currentLowerThreshold, d->currentUpperThreshold);
    });
    
    // 滤波参数信号连接 (部分已在createFilterGroup中连接按钮)
    connect(d->gaussianSigmaSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ParameterPanel::onSigmaDoubleSpinBoxChanged);
    connect(d->gaussianKernelSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ParameterPanel::onKernelSizeSpinBoxChanged);    connect(d->medianKernelSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this, d](int value){
            d->currentMedianKernelSize = value;
            emit kernelSizeChanged(value); // Or a more specific signal if needed
         });    // 配准参数信号连接
    if(d->registrationAlgorithmCombo) { // 确保控件已创建
        connect(d->registrationAlgorithmCombo, &QComboBox::currentTextChanged, this, [this, d](const QString& algorithm){
            d->currentRegAlgorithm = algorithm;
            emit registrationAlgorithmChanged(algorithm);
        });
    }
    
    // 测量工具信号连接
    connect(d->measureDistanceButton, &QPushButton::clicked, this, [this]() {
        emit measurementToolSelected(Distance);
    });
    connect(d->measureAngleButton, &QPushButton::clicked, this, [this]() {
        emit measurementToolSelected(Angle);
    });
    connect(d->measureAreaButton, &QPushButton::clicked, this, [this]() {
        emit measurementToolSelected(Area);
    });
    
    // 修复滤波参数信号连接，添加缺失的信号连接
    connect(d->filterTypeCombo, &QComboBox::currentTextChanged, this, &ParameterPanel::onFilterTypeComboChanged);
    connect(d->applyFilterButton, &QPushButton::clicked, this, [this, d]() {
        QString filterType = d->filterTypeCombo ? d->filterTypeCombo->currentText() : "";
        if (filterType == "高斯滤波") {
            onApplyGaussianFilter();
        } else if (filterType == "中值滤波") {
            onApplyMedianFilter();
        } else if (filterType == "锐化滤波") {
            onApplySharpenFilter();
        }
    });
    
    // 添加配准相关信号连接
    connect(d->fixedImageBrowseButton, &QPushButton::clicked, this, &ParameterPanel::onFixedImageBrowse);
    connect(d->movingImageBrowseButton, &QPushButton::clicked, this, &ParameterPanel::onMovingImageBrowse);
    connect(d->startRegistrationButton, &QPushButton::clicked, this, &ParameterPanel::onStartRegistration);
    connect(d->resetRegistrationButton, &QPushButton::clicked, this, &ParameterPanel::onResetRegistration);
}


// --- 实现 Public 方法 ---
void ParameterPanel::setWindowLevel(double window, double level) {
    Q_D(ParameterPanel);
    
    d->currentWindow = window;
    d->currentLevel = level;
    
    if (d->windowSlider) d->windowSlider->setValue(static_cast<int>(window));
    if (d->windowSpinBox) d->windowSpinBox->setValue(window);
    if (d->levelSlider) d->levelSlider->setValue(static_cast<int>(level));
    if (d->levelSpinBox) d->levelSpinBox->setValue(level);
    // 不在此处 emit windowLevelChanged，通常由用户操作触发或特定逻辑控制
}

void ParameterPanel::getWindowLevel(double& window, double& level) const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    window = d->currentWindow;
    level = d->currentLevel;
}

void ParameterPanel::setWindowLevelRange(double minWindow, double maxWindow,
                                       double minLevel, double maxLevel) {
    Q_D(ParameterPanel);    
    d->minWindowRange = minWindow;
    d->maxWindowRange = maxWindow;
    d->minLevelRange = minLevel;
    d->maxLevelRange = maxLevel;

    if (d->windowSlider) d->windowSlider->setRange(static_cast<int>(minWindow), static_cast<int>(maxWindow));
    if (d->windowSpinBox) d->windowSpinBox->setRange(minWindow, maxWindow);
    if (d->levelSlider) d->levelSlider->setRange(static_cast<int>(minLevel), static_cast<int>(maxLevel));
    if (d->levelSpinBox) d->levelSpinBox->setRange(minLevel, maxLevel);
}

void ParameterPanel::setThreshold(double lower, double upper) {
    Q_D(ParameterPanel);
    d->currentLowerThreshold = lower;
    d->currentUpperThreshold = upper;
    if(d->lowerThresholdSlider) d->lowerThresholdSlider->setValue(static_cast<int>(lower));
    if(d->lowerThresholdSpinBox) d->lowerThresholdSpinBox->setValue(lower);
    if(d->upperThresholdSlider) d->upperThresholdSlider->setValue(static_cast<int>(upper));
    if(d->upperThresholdSpinBox) d->upperThresholdSpinBox->setValue(upper);
}

void ParameterPanel::getThreshold(double& lower, double& upper) const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    lower = d->currentLowerThreshold;
    upper = d->currentUpperThreshold;
}

void ParameterPanel::setThresholdRange(double minVal, double maxVal) {
    Q_D(ParameterPanel);
    d->minThresholdRange = minVal;
    d->maxThresholdRange = maxVal;
    if(d->lowerThresholdSlider) d->lowerThresholdSlider->setRange(static_cast<int>(minVal), static_cast<int>(maxVal));
    if(d->lowerThresholdSpinBox) d->lowerThresholdSpinBox->setRange(minVal, maxVal);
    if(d->upperThresholdSlider) d->upperThresholdSlider->setRange(static_cast<int>(minVal), static_cast<int>(maxVal));
    if(d->upperThresholdSpinBox) d->upperThresholdSpinBox->setRange(minVal, maxVal);
}

void ParameterPanel::setOpacity(double opacity) {
    Q_D(ParameterPanel);
    d->currentOpacity = opacity;
    if(d->opacitySlider) d->opacitySlider->setValue(static_cast<int>(opacity * 100));
    if(d->opacitySpinBox) d->opacitySpinBox->setValue(opacity);
}

double ParameterPanel::getOpacity() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->currentOpacity;
}

void ParameterPanel::setColormap(const QString& colormapName) {
    Q_D(ParameterPanel);    
    d->currentColormap = colormapName;
    if(d->colormapCombo) d->colormapCombo->setCurrentText(colormapName);
}

QString ParameterPanel::getColormap() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->currentColormap;
}

void ParameterPanel::setGaussianSigma(double sigma) {
    Q_D(ParameterPanel);
    d->currentGaussianSigma = sigma;
    if(d->gaussianSigmaSpinBox) d->gaussianSigmaSpinBox->setValue(sigma);
    // emit sigmaChanged(sigma); // Emit when set programmatically
}

// Add setter for Gaussian Kernel Size
void ParameterPanel::setGaussianKernelSize(int kernelSize) {
    Q_D(ParameterPanel);
    d->currentGaussianKernelSize = kernelSize;
    if(d->gaussianKernelSizeSpinBox) d->gaussianKernelSizeSpinBox->setValue(kernelSize);
    // emit kernelSizeChanged(kernelSize); // Emit when set programmatically
}

// Modify setter for Median Radius to Median Kernel Size
void ParameterPanel::setMedianKernelSize(int kernelSize) {
    Q_D(ParameterPanel);
    d->currentMedianKernelSize = kernelSize;
    if(d->medianKernelSizeSpinBox) d->medianKernelSizeSpinBox->setValue(kernelSize);
    // emit kernelSizeChanged(kernelSize); // Emit when set programmatically
}

// Getters
// ... (getGaussianSigma already exists) ...
int ParameterPanel::getGaussianKernelSize() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->currentGaussianKernelSize;
}

int ParameterPanel::getMedianKernelSize() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->currentMedianKernelSize;
}

void ParameterPanel::setFixedImageFile(const QString& filePath) {
    Q_D(ParameterPanel);
    d->currentFixedImageFile = filePath;
    if(d->fixedImageLineEdit) d->fixedImageLineEdit->setText(filePath);
    // emit fixedImageChanged(filePath); // Emit when set programmatically
}

QString ParameterPanel::getFixedImageFile() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->currentFixedImageFile;
}

void ParameterPanel::setMovingImageFile(const QString& filePath) {
    Q_D(ParameterPanel);
    d->currentMovingImageFile = filePath;
    if(d->movingImageLineEdit) d->movingImageLineEdit->setText(filePath);
    // emit movingImageChanged(filePath); // Emit when set programmatically
}

QString ParameterPanel::getMovingImageFile() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->currentMovingImageFile;
}

void ParameterPanel::setRegistrationAlgorithm(const QString& algorithm) {
    Q_D(ParameterPanel);
    d->currentRegAlgorithm = algorithm;
    if(d->registrationAlgorithmCombo) d->registrationAlgorithmCombo->setCurrentText(algorithm);
    // emit registrationAlgorithmChanged(algorithm); // Emit when set programmatically
}

QString ParameterPanel::getRegistrationAlgorithm() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->currentRegAlgorithm;
}

void ParameterPanel::enableWindowLevelControls(bool enabled) {
    Q_D(ParameterPanel);
    if(d->windowSlider) d->windowSlider->setEnabled(enabled);
    if(d->windowSpinBox) d->windowSpinBox->setEnabled(enabled);
    if(d->levelSlider) d->levelSlider->setEnabled(enabled);
    if(d->levelSpinBox) d->levelSpinBox->setEnabled(enabled);
}

void ParameterPanel::enableThresholdControls(bool enabled) {
    Q_D(ParameterPanel);
    if(d->lowerThresholdSlider) d->lowerThresholdSlider->setEnabled(enabled);
    if(d->lowerThresholdSpinBox) d->lowerThresholdSpinBox->setEnabled(enabled);
    if(d->upperThresholdSlider) d->upperThresholdSlider->setEnabled(enabled);
    if(d->upperThresholdSpinBox) d->upperThresholdSpinBox->setEnabled(enabled);
}

void ParameterPanel::enableFilterControls(bool enabled) {
    Q_D(ParameterPanel);
    if(d->filterTypeCombo) d->filterTypeCombo->setEnabled(enabled);
    if(d->gaussianSigmaSpinBox) d->gaussianSigmaSpinBox->setEnabled(enabled && d->filterTypeCombo && d->filterTypeCombo->currentText() == "高斯滤波");
    if(d->gaussianKernelSizeSpinBox) d->gaussianKernelSizeSpinBox->setEnabled(enabled && d->filterTypeCombo && d->filterTypeCombo->currentText() == "高斯滤波");
    if(d->medianKernelSizeSpinBox) d->medianKernelSizeSpinBox->setEnabled(enabled && d->filterTypeCombo && d->filterTypeCombo->currentText() == "中值滤波");
    if(d->applyFilterButton) d->applyFilterButton->setEnabled(enabled);
    // ... any other specific filter controls ...
}

void ParameterPanel::enableRegistrationControls(bool enabled) {
    Q_D(ParameterPanel);
    if(d->fixedImageLineEdit) d->fixedImageLineEdit->setEnabled(enabled);
    if(d->fixedImageBrowseButton) d->fixedImageBrowseButton->setEnabled(enabled);
    if(d->movingImageLineEdit) d->movingImageLineEdit->setEnabled(enabled);
    if(d->movingImageBrowseButton) d->movingImageBrowseButton->setEnabled(enabled);
    if(d->registrationAlgorithmCombo) d->registrationAlgorithmCombo->setEnabled(enabled);
    if(d->startRegistrationButton) d->startRegistrationButton->setEnabled(enabled);
    if(d->resetRegistrationButton) d->resetRegistrationButton->setEnabled(enabled);
}


void ParameterPanel::setSliceRange(int min, int max) {
    Q_D(ParameterPanel);
    d->minSlice = min;
    d->maxSlice = max; // Assuming min is always 0 or handled by image data
    if (d->sliceSlider) d->sliceSlider->setRange(min, max);
    if (d->sliceSpinBox) d->sliceSpinBox->setRange(min, max);
    updateSliceInfo();
}

void ParameterPanel::setSlicePosition(int slice) {
    Q_D(ParameterPanel);
    d->currentSlice = slice;
    if (d->sliceSlider) d->sliceSlider->setValue(slice);
    if (d->sliceSpinBox) d->sliceSpinBox->setValue(slice);
    // updateSliceInfo(); // Already called by slider/spinbox valueChanged
}

int ParameterPanel::getSlicePosition() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->currentSlice;
}

void ParameterPanel::setZoom(double zoom) {
    Q_D(ParameterPanel);
    d->currentZoom = zoom;
    if (d->zoomSlider) d->zoomSlider->setValue(static_cast<int>(zoom * 100));
    if (d->zoomSpinBox) d->zoomSpinBox->setValue(zoom);
}

double ParameterPanel::getZoom() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->currentZoom;
}

void ParameterPanel::setViewType(int viewType) {
    Q_D(ParameterPanel);
    if (d->viewTypeCombo) d->viewTypeCombo->setCurrentIndex(viewType);
}

int ParameterPanel::getViewType() const {
    Q_D(const ParameterPanel);  // 修复：添加 const 关键字
    return d->viewTypeCombo ? d->viewTypeCombo->currentIndex() : 0;
}

void ParameterPanel::updateMeasurementResult(const QString& result) {
    Q_D(ParameterPanel);
    if (d->measurementResultLabel) d->measurementResultLabel->setText(result);
}

void ParameterPanel::updateDisplay() {
    Q_D(ParameterPanel);
    // This method updates all UI elements based on the current state of d values.
    // Update all relevant controls based on current d values
    setWindowLevel(d->currentWindow, d->currentLevel);
    setSlicePosition(d->currentSlice); // This will also call updateSliceInfo
    setZoom(d->currentZoom);
    setViewType(getViewType()); // Assuming getViewType returns current index
    setThreshold(d->currentLowerThreshold, d->currentUpperThreshold);
    setOpacity(d->currentOpacity);
    setColormap(d->currentColormap);
    setGaussianSigma(d->currentGaussianSigma);
    setGaussianKernelSize(d->currentGaussianKernelSize);
    setMedianKernelSize(d->currentMedianKernelSize);
    
    setFixedImageFile(d->currentFixedImageFile);
    setMovingImageFile(d->currentMovingImageFile);
    setRegistrationAlgorithm(d->currentRegAlgorithm);

    // Update visibility of filter params based on current selection
    if(d->filterTypeCombo) {
      onFilterTypeComboChanged(d->filterTypeCombo->currentText());
    }

    if(d->showAxisCheckBox) d->showAxisCheckBox->setChecked(d->showAxisCheckBox->isChecked()); // Or some internal state
    if(d->showScaleCheckBox) d->showScaleCheckBox->setChecked(d->showScaleCheckBox->isChecked());
    if(d->brightnessSlider) d->brightnessSlider->setValue(d->brightnessSlider->value());
    if(d->contrastSlider) d->contrastSlider->setValue(d->contrastSlider->value());
    if(d->invertColorsCheckBox) d->invertColorsCheckBox->setChecked(d->invertColorsCheckBox->isChecked());

    updateSliceInfo();
}

void ParameterPanel::updateSliceInfo() {
    Q_D(ParameterPanel);    
    if (d->sliceInfoLabel) {
        d->sliceInfoLabel->setText(QString("%1 / %2").arg(d->currentSlice).arg(d->maxSlice));
    }
}

// --- 实现 Public Slots ---
void ParameterPanel::onWindowLevelChanged(double window, double level) {
    // This slot is intended to be called EXTERNALLY if the window/level changes
    // (e.g., from another part of the application or a preset)
    // It then updates the UI.
    setWindowLevel(window, level);
    // It should NOT emit windowLevelChanged again, to avoid loops.
}

void ParameterPanel::onThresholdChanged(double lower, double upper) {
    setThreshold(lower, upper);
}

void ParameterPanel::onOpacityChanged(double opacity) {
    setOpacity(opacity);
}

void ParameterPanel::resetParameters() {
    Q_D(ParameterPanel);
    // This slot resets all parameters to their default values
    // Reset all parameters to default values and update UI
    // Example defaults:
    d->currentWindow = 400.0;
    d->currentLevel = 40.0;
    d->currentSlice = 0;
    // d->maxSlice remains as set by data
    d->currentZoom = 1.0;
    d->currentLowerThreshold = d->minThresholdRange; // Or a sensible default
    d->currentUpperThreshold = d->maxThresholdRange; // Or a sensible default
    d->currentOpacity = 1.0;
    d->currentColormap = "灰度";
    d->currentGaussianSigma = 1.0;
    d->currentGaussianKernelSize = 3;
    d->currentMedianKernelSize = 3;
    d->currentFixedImageFile = "";
    d->currentMovingImageFile = "";
    d->currentRegAlgorithm = "ICP"; // Default algorithm

    if(d->filterTypeCombo) d->filterTypeCombo->setCurrentIndex(0); // Default to Gaussian or first item
    if(d->viewTypeCombo) d->viewTypeCombo->setCurrentIndex(0); // Default to Axial
    if(d->showAxisCheckBox) d->showAxisCheckBox->setChecked(true);
    if(d->showScaleCheckBox) d->showScaleCheckBox->setChecked(false);
    if(d->brightnessSlider) d->brightnessSlider->setValue(0);
    if(d->contrastSlider) d->contrastSlider->setValue(0);
    if(d->invertColorsCheckBox) d->invertColorsCheckBox->setChecked(false);
    
    updateDisplay(); // Update all UI elements

    // Emit signals for all changed parameters if necessary, or a general reset signal
    emit windowLevelChanged(d->currentWindow, d->currentLevel);
    emit sliceChanged(d->currentSlice);
    emit zoomChanged(d->currentZoom);
    emit thresholdChanged(d->currentLowerThreshold, d->currentUpperThreshold);
    emit opacityChanged(d->currentOpacity);
    emit colormapChanged(d->currentColormap); // Make sure this signal exists with QString
    emit filterTypeChanged(d->filterTypeCombo ? d->filterTypeCombo->currentText() : "高斯滤波");
    emit kernelSizeChanged(d->currentGaussianKernelSize); // Or a general kernel size if applicable
    emit sigmaChanged(d->currentGaussianSigma);
    emit fixedImageChanged(d->currentFixedImageFile);
    emit movingImageChanged(d->currentMovingImageFile);
    emit registrationAlgorithmChanged(d->currentRegAlgorithm);
    // emit registrationParametersChanged(...); // This should be emitted when parameters are confirmed, not on general reset.
    
    emit resetParametersRequested(); // General signal
}


// --- 实现 Private Slots ---
void ParameterPanel::onWindowChanged(int value) {
    Q_D(ParameterPanel);    
    d->currentWindow = static_cast<double>(value);
    if(d->windowSpinBox) d->windowSpinBox->setValue(d->currentWindow);
    emit windowChanged(d->currentWindow); // Emit only window
}

void ParameterPanel::onLevelChanged(int value) {
    Q_D(ParameterPanel);    
    d->currentLevel = static_cast<double>(value);
    if(d->levelSpinBox) d->levelSpinBox->setValue(d->currentLevel);
    emit levelChanged(d->currentLevel); // Emit only level
}

void ParameterPanel::onLowerThresholdChanged(double value) {
    Q_D(ParameterPanel);    
    d->currentLowerThreshold = value;
    // d->lowerThresholdSlider->setValue(static_cast<int>(value)); // Already connected if slider changes spinbox
    if (d->currentLowerThreshold > d->currentUpperThreshold) {
        d->currentUpperThreshold = d->currentLowerThreshold;
        if(d->upperThresholdSlider) d->upperThresholdSlider->setValue(static_cast<int>(d->currentUpperThreshold));
        if(d->upperThresholdSpinBox) d->upperThresholdSpinBox->setValue(d->currentUpperThreshold);
    }
    emit thresholdChanged(d->currentLowerThreshold, d->currentUpperThreshold);
}

void ParameterPanel::onUpperThresholdChanged(double value) {
    Q_D(ParameterPanel);    
    d->currentUpperThreshold = value;
    // d->upperThresholdSlider->setValue(static_cast<int>(value));
     if (d->currentUpperThreshold < d->currentLowerThreshold) {
        d->currentLowerThreshold = d->currentUpperThreshold;
        if(d->lowerThresholdSlider) d->lowerThresholdSlider->setValue(static_cast<int>(d->currentLowerThreshold));
        if(d->lowerThresholdSpinBox) d->lowerThresholdSpinBox->setValue(d->currentLowerThreshold);
    }
    emit thresholdChanged(d->currentLowerThreshold, d->currentUpperThreshold);
}

void ParameterPanel::onOpacitySliderChanged(int value) {
    Q_D(ParameterPanel);    
    d->currentOpacity = value / 100.0;
    if(d->opacitySpinBox) d->opacitySpinBox->setValue(d->currentOpacity);
    emit opacityChanged(d->currentOpacity);
}

void ParameterPanel::onColormapChanged(const QString& colormapName) {
    Q_D(ParameterPanel);    
    d->currentColormap = colormapName;
    emit colormapChanged(d->currentColormap); // Ensure this signal takes QString
}

// Slots for new filter controls
void ParameterPanel::onFilterTypeComboChanged(const QString& filterType) {
    Q_D(ParameterPanel);    
    bool isGaussian = (filterType == "高斯滤波");
    bool isMedian = (filterType == "中值滤波");
    // bool isSharpen = (filterType == "锐化滤波"); // Sharpen might not have params

    if(d->gaussianSigmaSpinBox) d->gaussianSigmaSpinBox->setVisible(isGaussian);
    if(d->gaussianKernelSizeSpinBox) d->gaussianKernelSizeSpinBox->setVisible(isGaussian);
    if(d->medianKernelSizeSpinBox) d->medianKernelSizeSpinBox->setVisible(isMedian);
    
    // Adjust label visibility if they are separate QLabels not part of the SpinBox itself
    // (Assuming labels are created next to spinboxes and their visibility is tied)
    // e.g. find child label and setVisible. For now, assuming spinbox visibility is enough.

    emit filterTypeChanged(filterType);
}

void ParameterPanel::onKernelSizeSpinBoxChanged(int value) {
    Q_D(ParameterPanel);
    // This could be generic if only one kernel size is active at a time,
    // or specific if Gaussian and Median kernels are different.
    // Based on current setup, d->filterTypeCombo determines which kernel is relevant.
    QString currentFilter = d->filterTypeCombo ? d->filterTypeCombo->currentText() : "";
    if (currentFilter == "高斯滤波") {
        d->currentGaussianKernelSize = value;
    } else if (currentFilter == "中值滤波") {
        d->currentMedianKernelSize = value;
    }
    // For simplicity, emitting a generic kernelSizeChanged.
    // If specific signals are needed, they can be added.
    emit kernelSizeChanged(value);
}

void ParameterPanel::onSigmaDoubleSpinBoxChanged(double value) {
    Q_D(ParameterPanel);  
    d->currentGaussianSigma = value;
    emit sigmaChanged(value);
}

// Slots for new registration controls
void ParameterPanel::onFixedImageBrowse() {
    Q_D(ParameterPanel);
    
    QString filePath = QFileDialog::getOpenFileName(this, "选择固定图像文件", "", "图像文件 (*.png *.jpg *.bmp *.dcm *.nii *.nii.gz);;所有文件 (*)");
    if (!filePath.isEmpty()) {
        d->currentFixedImageFile = filePath;
        if(d->fixedImageLineEdit) d->fixedImageLineEdit->setText(filePath);
        emit fixedImageChanged(filePath);
    }
}

void ParameterPanel::onMovingImageBrowse() {
    Q_D(ParameterPanel);
    
    QString filePath = QFileDialog::getOpenFileName(this, "选择移动图像文件", "", "图像文件 (*.png *.jpg *.bmp *.dcm *.nii *.nii.gz);;所有文件 (*)");
    if (!filePath.isEmpty()) {
        d->currentMovingImageFile = filePath;
        if(d->movingImageLineEdit) d->movingImageLineEdit->setText(filePath);
        emit movingImageChanged(filePath);
    }
}

void ParameterPanel::onRegAlgorithmComboChanged(const QString& algorithm) {
    Q_D(ParameterPanel);
    
    d->currentRegAlgorithm = algorithm;
    emit registrationAlgorithmChanged(algorithm);
}


// --- 实现 Public Slots ---

void ParameterPanel::onApplyGaussianFilter() {
    Q_D(ParameterPanel);
    
    qDebug() << "ParameterPanel: Apply Gaussian Filter requested. Sigma: " << d->currentGaussianSigma << " Kernel: " << d->currentGaussianKernelSize;
    emit gaussianFilterRequested(d->currentGaussianSigma, d->currentGaussianKernelSize);
}

void ParameterPanel::onApplyMedianFilter() {
    Q_D(ParameterPanel);
    
     qDebug() << "ParameterPanel: Apply Median Filter requested. Kernel: " << d->currentMedianKernelSize;
    emit medianFilterRequested(d->currentMedianKernelSize);
}

void ParameterPanel::onApplySharpenFilter() {
    Q_D(ParameterPanel);

    qDebug() << "ParameterPanel: Apply Sharpen Filter requested.";
    emit sharpenFilterRequested();
}

void ParameterPanel::onStartRegistration() {
    Q_D(ParameterPanel);
    qDebug() << "ParameterPanel: Start Registration requested. Fixed: " << d->currentFixedImageFile << " Moving: " << d->currentMovingImageFile << " Algo: " << d->currentRegAlgorithm;
    if (d->currentFixedImageFile.isEmpty() || d->currentMovingImageFile.isEmpty()) {
        qDebug() << "Fixed or Moving image not set.";
        // Optionally show a message to the user
        return;
    }
    
    // Then emit the combined signal for parameters
    emit registrationParametersChanged(d->currentFixedImageFile, d->currentMovingImageFile, d->currentRegAlgorithm);
    // And the request to start
    emit startRegistrationRequested();
}

void ParameterPanel::onResetRegistration() {

    Q_D(ParameterPanel);
    qDebug() << "ParameterPanel: Reset Registration requested.";
    // Clear file paths and reset algorithm in UI and internal state
    setFixedImageFile("");
    setMovingImageFile("");
    if(d->registrationAlgorithmCombo) d->registrationAlgorithmCombo->setCurrentIndex(0); // Reset to first algorithm
    d->currentRegAlgorithm = d->registrationAlgorithmCombo ? d->registrationAlgorithmCombo->itemText(0) : "ICP";

    // Emit signals for changes
    emit fixedImageChanged("");
    emit movingImageChanged("");
    emit registrationAlgorithmChanged(d->currentRegAlgorithm);
    emit resetRegistrationRequested();
}

void ParameterPanel::onClearMeasurements() {
    Q_D(ParameterPanel);
    
    qDebug() << "ParameterPanel: Clear Measurements requested.";
    if (d->measurementResultLabel) d->measurementResultLabel->setText("测量结果已清除");
    emit clearMeasurementsRequested();
}

}// namespace MedicalImaging