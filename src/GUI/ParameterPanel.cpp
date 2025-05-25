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

namespace MedicalImaging {

struct ParameterPanel::Impl {
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
    
    // 图像处理控件
    QSlider* brightnessSlider = nullptr;
    QSlider* contrastSlider = nullptr;
    QComboBox* colormapCombo = nullptr;
    QCheckBox* invertColorsCheckBox = nullptr;
    
    // 测量工具控件
    QPushButton* measureDistanceButton = nullptr;
    QPushButton* measureAngleButton = nullptr;
    QPushButton* measureAreaButton = nullptr;
    QLabel* measurementResultLabel = nullptr;
    
    // 当前参数值
    double currentWindow = 400.0;
    double currentLevel = 40.0;
    int currentSlice = 0;
    int maxSlice = 100;
    double currentZoom = 1.0;
    
    Impl() = default;
};

ParameterPanel::ParameterPanel(QWidget* parent)
    : QWidget(parent), d(std::make_unique<Impl>()) {
    setupUI();
    connectSignals();
    updateDisplay();
}

ParameterPanel::~ParameterPanel() = default;

void ParameterPanel::setupUI() {
    setFixedWidth(280);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // 创建各个控制组
    mainLayout->addWidget(createWindowLevelGroup());
    mainLayout->addWidget(createSliceGroup());
    mainLayout->addWidget(createZoomGroup());
    mainLayout->addWidget(createViewGroup());
    mainLayout->addWidget(createImageProcessingGroup());
    mainLayout->addWidget(createMeasurementGroup());
    
    // 添加弹性空间
    mainLayout->addStretch();
}

QGroupBox* ParameterPanel::createWindowLevelGroup() {
    QGroupBox* group = new QGroupBox("窗宽窗位", this);
    QGridLayout* layout = new QGridLayout(group);
    
    // 窗宽控件
    layout->addWidget(new QLabel("窗宽:"), 0, 0);
    d->windowSlider = new QSlider(Qt::Horizontal);
    d->windowSlider->setRange(1, 2000);
    d->windowSlider->setValue(static_cast<int>(d->currentWindow));
    layout->addWidget(d->windowSlider, 0, 1);
    
    d->windowSpinBox = new QDoubleSpinBox();
    d->windowSpinBox->setRange(1.0, 2000.0);
    d->windowSpinBox->setValue(d->currentWindow);
    d->windowSpinBox->setSuffix(" HU");
    layout->addWidget(d->windowSpinBox, 0, 2);
    
    // 窗位控件
    layout->addWidget(new QLabel("窗位:"), 1, 0);
    d->levelSlider = new QSlider(Qt::Horizontal);
    d->levelSlider->setRange(-1000, 1000);
    d->levelSlider->setValue(static_cast<int>(d->currentLevel));
    layout->addWidget(d->levelSlider, 1, 1);
    
    d->levelSpinBox = new QDoubleSpinBox();
    d->levelSpinBox->setRange(-1000.0, 1000.0);
    d->levelSpinBox->setValue(d->currentLevel);
    d->levelSpinBox->setSuffix(" HU");
    layout->addWidget(d->levelSpinBox, 1, 2);
    
    return group;
}

QGroupBox* ParameterPanel::createSliceGroup() {
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
    
    d->sliceInfoLabel = new QLabel(QString("%1 / %2").arg(d->currentSlice).arg(d->maxSlice));
    layout->addWidget(d->sliceInfoLabel, 1, 0, 1, 3);
    
    return group;
}

QGroupBox* ParameterPanel::createZoomGroup() {
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

QGroupBox* ParameterPanel::createImageProcessingGroup() {
    QGroupBox* group = new QGroupBox("图像处理", this);
    QGridLayout* layout = new QGridLayout(group);
    
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
    
    return group;
}

QGroupBox* ParameterPanel::createMeasurementGroup() {
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
    QPushButton* clearMeasurementsButton = new QPushButton("清除");
    buttonLayout2->addWidget(d->measureAreaButton);
    buttonLayout2->addWidget(clearMeasurementsButton);
    layout->addLayout(buttonLayout2);
    
    // 测量结果显示
    d->measurementResultLabel = new QLabel("测量结果将显示在此处");
    d->measurementResultLabel->setWordWrap(true);
    d->measurementResultLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 5px; border: 1px solid #ccc; }");
    layout->addWidget(d->measurementResultLabel);
    
    return group;
}

void ParameterPanel::connectSignals() {
    // 窗宽窗位信号连接
    connect(d->windowSlider, &QSlider::valueChanged, this, [this](int value) {
        d->currentWindow = static_cast<double>(value);
        d->windowSpinBox->setValue(d->currentWindow);
        emit windowLevelChanged(d->currentWindow, d->currentLevel);
    });
    
    connect(d->windowSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this](double value) {
        d->currentWindow = value;
        d->windowSlider->setValue(static_cast<int>(value));
        emit windowLevelChanged(d->currentWindow, d->currentLevel);
    });
    
    connect(d->levelSlider, &QSlider::valueChanged, this, [this](int value) {
        d->currentLevel = static_cast<double>(value);
        d->levelSpinBox->setValue(d->currentLevel);
        emit windowLevelChanged(d->currentWindow, d->currentLevel);
    });
    
    connect(d->levelSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this](double value) {
        d->currentLevel = value;
        d->levelSlider->setValue(static_cast<int>(value));
        emit windowLevelChanged(d->currentWindow, d->currentLevel);
    });
    
    // 切片信号连接
    connect(d->sliceSlider, &QSlider::valueChanged, this, [this](int value) {
        d->currentSlice = value;
        d->sliceSpinBox->setValue(value);
        updateSliceInfo();
        emit sliceChanged(value);
    });
    
    connect(d->sliceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, [this](int value) {
        d->currentSlice = value;
        d->sliceSlider->setValue(value);
        updateSliceInfo();
        emit sliceChanged(value);
    });
    
    // 缩放信号连接
    connect(d->zoomSlider, &QSlider::valueChanged, this, [this](int value) {
        d->currentZoom = value / 100.0;
        d->zoomSpinBox->setValue(d->currentZoom);
        emit zoomChanged(d->currentZoom);
    });
    
    connect(d->zoomSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            this, [this](double value) {
        d->currentZoom = value;
        d->zoomSlider->setValue(static_cast<int>(value * 100));
        emit zoomChanged(d->currentZoom);
    });
    
    connect(d->resetZoomButton, &QPushButton::clicked, this, [this]() {
        setZoom(1.0);
        emit resetZoomRequested();
    });
    
    connect(d->fitToWindowButton, &QPushButton::clicked, this, &ParameterPanel::fitToWindowRequested);
    
    // 视图控制信号连接
    connect(d->viewTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &ParameterPanel::viewTypeChanged);
    
    connect(d->showAxisCheckBox, &QCheckBox::toggled, this, &ParameterPanel::showAxisChanged);
    connect(d->showScaleCheckBox, &QCheckBox::toggled, this, &ParameterPanel::showScaleChanged);
    
    // 图像处理信号连接
    connect(d->brightnessSlider, &QSlider::valueChanged, this, &ParameterPanel::brightnessChanged);
    connect(d->contrastSlider, &QSlider::valueChanged, this, &ParameterPanel::contrastChanged);
    connect(d->colormapCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &ParameterPanel::colormapChanged);
    connect(d->invertColorsCheckBox, &QCheckBox::toggled, this, &ParameterPanel::invertColorsChanged);
    
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
}

void ParameterPanel::setWindowLevel(double window, double level) {
    d->currentWindow = window;
    d->currentLevel = level;
    
    d->windowSlider->setValue(static_cast<int>(window));
    d->windowSpinBox->setValue(window);
    d->levelSlider->setValue(static_cast<int>(level));
    d->levelSpinBox->setValue(level);
}

void ParameterPanel::getWindowLevel(double& window, double& level) const {
    window = d->currentWindow;
    level = d->currentLevel;
}

void ParameterPanel::setSliceRange(int min, int max) {
    d->maxSlice = max;
    d->sliceSlider->setRange(min, max);
    d->sliceSpinBox->setRange(min, max);
    updateSliceInfo();
}

void ParameterPanel::setSlicePosition(int slice) {
    d->currentSlice = slice;
    d->sliceSlider->setValue(slice);
    d->sliceSpinBox->setValue(slice);
    updateSliceInfo();
}

int ParameterPanel::getSlicePosition() const {
    return d->currentSlice;
}

void ParameterPanel::setZoom(double zoom) {
    d->currentZoom = zoom;
    d->zoomSlider->setValue(static_cast<int>(zoom * 100));
    d->zoomSpinBox->setValue(zoom);
}

double ParameterPanel::getZoom() const {
    return d->currentZoom;
}

void ParameterPanel::setViewType(int viewType) {
    d->viewTypeCombo->setCurrentIndex(viewType);
}

int ParameterPanel::getViewType() const {
    return d->viewTypeCombo->currentIndex();
}

void ParameterPanel::updateMeasurementResult(const QString& result) {
    d->measurementResultLabel->setText(result);
}

void ParameterPanel::updateDisplay() {
    updateSliceInfo();
}

void ParameterPanel::updateSliceInfo() {
    d->sliceInfoLabel->setText(QString("%1 / %2").arg(d->currentSlice).arg(d->maxSlice));
}

} // namespace MedicalImaging
