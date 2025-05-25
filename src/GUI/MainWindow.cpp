#include "MainWindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QSplitter>

class MainWindow::MainWindowPrivate {
public:
    QWidget* centralWidget = nullptr;
    QSplitter* mainSplitter = nullptr;
    QLabel* imageDisplayLabel = nullptr;
    QWidget* parameterPanel = nullptr;
    QPushButton* openButton = nullptr;
    QPushButton* processButton = nullptr;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d_ptr(std::make_unique<MainWindowPrivate>())
{
    setupUI();
    setupMenuBar();
    setupStatusBar();
    setupConnections();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    Q_D(MainWindow);
    
    d->centralWidget = new QWidget(this);
    setCentralWidget(d->centralWidget);

    // 创建主分割器
    d->mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // 左侧：图像显示区域
    d->imageDisplayLabel = new QLabel("医学图像显示区域", this);
    d->imageDisplayLabel->setAlignment(Qt::AlignCenter);
    d->imageDisplayLabel->setStyleSheet(
        "QLabel { "
        "background-color: #f0f0f0; "
        "border: 2px dashed #cccccc; "
        "font-size: 16px; "
        "color: #666666; "
        "}"
    );
    d->imageDisplayLabel->setMinimumSize(400, 300);
    
    // 右侧：参数面板
    d->parameterPanel = new QWidget(this);
    d->parameterPanel->setMaximumWidth(300);
    d->parameterPanel->setStyleSheet("QWidget { background-color: #fafafa; }");
    
    auto paramLayout = new QVBoxLayout(d->parameterPanel);
    
    auto titleLabel = new QLabel("参数控制面板", this);
    titleLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; margin: 10px; }");
    
    d->openButton = new QPushButton("打开医学图像", this);
    d->openButton->setStyleSheet(
        "QPushButton { "
        "background-color: #4CAF50; "
        "color: white; "
        "border: none; "
        "padding: 8px 16px; "
        "border-radius: 4px; "
        "font-size: 12px; "
        "}"
        "QPushButton:hover { background-color: #45a049; }"
    );
    
    d->processButton = new QPushButton("图像处理", this);
    d->processButton->setStyleSheet(
        "QPushButton { "
        "background-color: #2196F3; "
        "color: white; "
        "border: none; "
        "padding: 8px 16px; "
        "border-radius: 4px; "
        "font-size: 12px; "
        "}"
        "QPushButton:hover { background-color: #1976D2; }"
    );
    d->processButton->setEnabled(false);
    
    paramLayout->addWidget(titleLabel);
    paramLayout->addWidget(d->openButton);
    paramLayout->addWidget(d->processButton);
    paramLayout->addStretch();
    
    // 添加到分割器
    d->mainSplitter->addWidget(d->imageDisplayLabel);
    d->mainSplitter->addWidget(d->parameterPanel);
    d->mainSplitter->setStretchFactor(0, 1);
    d->mainSplitter->setStretchFactor(1, 0);
    
    // 设置主布局
    auto mainLayout = new QVBoxLayout(d->centralWidget);
    mainLayout->addWidget(d->mainSplitter);
    
    setWindowTitle("医学成像软件");
    resize(800, 600);
}

void MainWindow::setupMenuBar() {
    // 文件菜单
    auto fileMenu = menuBar()->addMenu("文件(&F)");
    fileMenu->addAction("打开(&O)", this, &MainWindow::openFile, QKeySequence::Open);
    fileMenu->addAction("保存(&S)", this, &MainWindow::saveFile, QKeySequence::Save);
    fileMenu->addAction("导出图像(&E)", this, &MainWindow::exportImage);
    fileMenu->addSeparator();
    fileMenu->addAction("退出(&X)", this, &MainWindow::exit, QKeySequence::Quit);

    // 编辑菜单
    auto editMenu = menuBar()->addMenu("编辑(&E)");
    editMenu->addAction("重置视图(&R)", this, &MainWindow::resetView);
    editMenu->addAction("重置参数(&P)", this, &MainWindow::resetParameters);

    // 帮助菜单
    auto helpMenu = menuBar()->addMenu("帮助(&H)");
    helpMenu->addAction("帮助文档(&H)", this, &MainWindow::showHelp);
    helpMenu->addAction("关于(&A)", this, &MainWindow::showAbout);
}

void MainWindow::setupStatusBar() {
    statusBar()->showMessage("就绪 - 请打开医学图像文件开始工作");
}

void MainWindow::setupConnections() {
    Q_D(MainWindow);
    connect(d->openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(d->processButton, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "图像处理", "图像处理功能将在后续版本中实现");
    });
}

void MainWindow::closeEvent(QCloseEvent *event) {
    auto reply = QMessageBox::question(this, "确认退出",
                                     "确定要退出医学成像软件吗？",
                                     QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::openFile() {
    Q_D(MainWindow);
    
    QString fileName = QFileDialog::getOpenFileName(this,
        "打开医学图像文件",
        "",
        "医学图像文件 (*.dcm *.nii *.nii.gz *.mhd *.mha);;所有文件 (*.*)");
    
    if (!fileName.isEmpty()) {
        d->imageDisplayLabel->setText(QString("已选择文件：\n%1\n\n图像加载功能将在后续版本中实现").arg(fileName));
        d->processButton->setEnabled(true);
        statusBar()->showMessage("已选择文件：" + fileName);
        
        emit onImageDataChanged();
    }
}

void MainWindow::saveFile() {
    QMessageBox::information(this, "保存文件", "文件保存功能将在后续版本中实现");
}

void MainWindow::exportImage() {
    QMessageBox::information(this, "导出图像", "图像导出功能将在后续版本中实现");
}

void MainWindow::exit() {
    close();
}

void MainWindow::resetView() {
    QMessageBox::information(this, "重置视图", "视图重置功能将在后续版本中实现");
}

void MainWindow::resetParameters() {
    QMessageBox::information(this, "重置参数", "参数重置功能将在后续版本中实现");
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "关于医学成像软件",
        "<h3>医学成像软件 v1.0</h3>"
        "<p>基于Qt和VTK的医学图像处理软件架构演示版本</p>"
        "<br>"
        "<p><b>软件架构：</b></p>"
        "<p>• <b>表示层 (Presentation Layer):</b> Qt GUI组件</p>"
        "<p>• <b>业务逻辑层 (Business Logic Layer):</b> 核心算法</p>"
        "<p>• <b>数据层 (Data Layer):</b> 数据模型和管理</p>"
        "<p>• <b>插件系统 (Plugin System):</b> 可扩展架构</p>"
        "<br>"
        "<p><b>主要功能模块：</b></p>"
        "<p>✓ 数据模型 (DataModel)</p>"
        "<p>✓ 渲染引擎 (RenderingEngine)</p>"
        "<p>✓ 图像处理器 (ImageProcessor)</p>"
        "<p>✓ 配准管理器 (RegistrationManager)</p>"
        "<p>✓ 用户界面 (GUI)</p>"
        "<p>✓ 插件系统 (Plugins)</p>"
        "<br>"
        "<p>© 2025 Medical Imaging Solutions</p>");
}

void MainWindow::showHelp() {
    QMessageBox::information(this, "帮助",
        "<h3>使用说明</h3>"
        "<p><b>1. 打开医学图像：</b></p>"
        "<p>使用 文件 → 打开 或点击 '打开医学图像' 按钮</p>"
        "<p>支持的格式：DICOM (.dcm), NIfTI (.nii), MetaImage (.mhd)</p>"
        "<br>"
        "<p><b>2. 图像处理：</b></p>"
        "<p>打开图像后，可以使用右侧参数面板进行图像处理</p>"
        "<br>"
        "<p><b>3. 软件架构：</b></p>"
        "<p>本软件采用分层架构设计，具有良好的可扩展性</p>");
}

void MainWindow::onImageDataChanged() {
    statusBar()->showMessage("图像数据已更新");
    updateUI();
}

void MainWindow::onMetaDataChanged() {
    statusBar()->showMessage("元数据已更新");
    updateUI();
}

void MainWindow::updateUI() {
    // 更新界面状态
}

#include "MainWindow.moc"
