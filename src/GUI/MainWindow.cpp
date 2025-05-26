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
#include <QCloseEvent>

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

    // 主分割器
    d->mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // 图像显示区域
    d->imageDisplayLabel = new QLabel("请打开医学影像文件", this);
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
    
    // 参数控制面板
    d->parameterPanel = new QWidget(this);
    d->parameterPanel->setMaximumWidth(300);
    d->parameterPanel->setStyleSheet("QWidget { background-color: #fafafa; }");
    
    QVBoxLayout* paramLayout = new QVBoxLayout(d->parameterPanel);
    
    QLabel* titleLabel = new QLabel("控制面板", this);
    titleLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; margin: 10px; }");
    
    d->openButton = new QPushButton("打开文件", this);
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
    
    d->processButton = new QPushButton("处理图像", this);
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
    QVBoxLayout* mainLayout = new QVBoxLayout(d->centralWidget);
    mainLayout->addWidget(d->mainSplitter);
    
    setWindowTitle("医学影像软件");
    resize(800, 600);
}

void MainWindow::setupMenuBar() {
    // 文件菜单
    QMenu* fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(tr("打开(&O)"), this, &MainWindow::openFile, 
                       QKeySequence(Qt::CTRL | Qt::Key_O));
    fileMenu->addAction(tr("保存(&S)"), this, &MainWindow::saveFile, 
                       QKeySequence(Qt::CTRL | Qt::Key_S));
    fileMenu->addAction(tr("导出图像(&E)"), this, &MainWindow::exportImage, 
                       QKeySequence(Qt::CTRL | Qt::Key_E));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("退出(&X)"), this, &MainWindow::exit, 
                       QKeySequence(Qt::CTRL | Qt::Key_Q));

    // 编辑菜单
    QMenu* editMenu = menuBar()->addMenu(tr("编辑(&E)"));
    editMenu->addAction(tr("重置视图(&R)"), this, &MainWindow::resetView, 
                       QKeySequence(Qt::CTRL | Qt::Key_R));
    editMenu->addAction(tr("重置参数(&P)"), this, &MainWindow::resetParameters, 
                       QKeySequence(Qt::CTRL | Qt::Key_P));

    // 帮助菜单
    QMenu* helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(tr("使用说明(&H)"), this, &MainWindow::showHelp, 
                       QKeySequence(Qt::Key_F1));
    helpMenu->addAction(tr("关于(&A)"), this, &MainWindow::showAbout);
}

void MainWindow::setupStatusBar() {
    statusBar()->showMessage("准备就绪，请打开医学影像文件");
    statusBar()->setStyleSheet("QStatusBar { font-size: 12px; }");
}

void MainWindow::setupConnections() {
    Q_D(MainWindow);
    connect(d->openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(d->processButton, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "处理图像", "图像处理功能开发中...");
    });
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "yes?","you want to exit?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);  // 设置默认按钮为"否"

    if (reply == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::openFile() {
    Q_D(MainWindow);
    
    static QString lastDir = QDir::homePath(); // 记住上次打开的目录
    
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("打开医学影像文件"),
        lastDir,
        tr("医学影像文件 (*.dcm *.nii *.nii.gz *.mhd *.mha);;"
           "DICOM文件 (*.dcm);;"
           "NIfTI文件 (*.nii *.nii.gz);;"
           "MetaImage文件 (*.mhd *.mha);;"
           "所有文件 (*.*)")
    );
    
    if (!fileName.isEmpty()) {
        lastDir = QFileInfo(fileName).absolutePath(); // 更新上次打开的目录
        d->imageDisplayLabel->setText(tr("loaded files:\n%1\n\nnow can to config image").arg(fileName));
        d->processButton->setEnabled(true);
        statusBar()->showMessage(tr("now:%1").arg(fileName));
        
        emit onImageDataChanged();
    }
}

void MainWindow::saveFile() {
    QMessageBox::information(this, tr("保存文件"), tr("保存功能开发中..."));
}

void MainWindow::exportImage() {
    QMessageBox::information(this, tr("导出图像"), tr("导出功能开发中..."));
}

void MainWindow::exit() {
    close();
}

void MainWindow::resetView() {
    QMessageBox::information(this, tr("重置视图"), tr("视图重置功能开发中..."));
}

void MainWindow::resetParameters() {
    QMessageBox::information(this, tr("重置参数"), tr("参数重置功能开发中..."));
}

void MainWindow::showAbout() {
    QMessageBox::about(this, tr("关于医学影像软件"),
        "<h3>医学影像软件 v1.0</h3>"
        "<p>基于Qt和VTK开发的医学影像处理软件</p>"
        "<br>"
        "<p><b>软件架构</b></p>"
        "<p>• <b>表示层(Presentation Layer):</b> Qt GUI界面</p>"
        "<p>• <b>业务逻辑层(Business Logic Layer):</b> 核心算法</p>"
        "<p>• <b>数据层(Data Layer):</b> 数据模型和存储</p>"
        "<p>• <b>插件系统 (Plugin System):</b> 扩展功能</p>"
        "<br>"
        "<p><b>主要功能模块</b></p>"
        "<p>• 数据模型 (DataModel)</p>"
        "<p>• 渲染引擎 (RenderingEngine)</p>"
        "<p>• 图像处理器(ImageProcessor)</p>"
        "<p>• 配准管理器(RegistrationManager)</p>"
        "<p>• 用户界面 (GUI)</p>"
        "<p>• 插件系统 (Plugins)</p>"
        "<br>"
        "<p>© 2025 Medical Imaging Solutions</p>");
}

void MainWindow::showHelp() {
    QMessageBox::information(this, tr("帮助"),
        "<h3>使用说明</h3>"
        "<p><b>1. 打开医学影像文件</b></p>"
        "<p>点击 文件 → 打开 或点击'打开文件' 按钮</p>"
        "<p>支持格式：DICOM (.dcm), NIfTI (.nii), MetaImage (.mhd)</p>"
        "<br>"
        "<p><b>2. 图像处理</b></p>"
        "<p>使用右侧参数面板调整窗宽窗位、滤波等参数</p>"
        "<br>"
        "<p><b>3. 图像配准</b></p>"
        "<p>在配准标签页中选择固定图像和移动图像进行配准</p>");
}

void MainWindow::onImageDataChanged() {
    statusBar()->showMessage(tr("image data has been updated"));
    updateUI();
}

void MainWindow::onMetaDataChanged() {
    statusBar()->showMessage(tr("meta data has been updated"));
    updateUI();
}

void MainWindow::updateUI() {
    // 更新界面逻辑
}


