#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

/**
 * @brief 简化的主窗口类用于演示
 */
class SimpleMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SimpleMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setupUI();
        setupMenuBar();
        setupStatusBar();
    }

private slots:
    void openFile() {
        QMessageBox::information(this, "打开文件", "文件打开功能将在后续版本中实现");
    }

    void showAbout() {
        QMessageBox::about(this, "关于",
            "医学成像软件 v1.0\n\n"
            "这是一个基于Qt和VTK的医学图像处理软件演示版本。\n\n"
            "主要功能：\n"
            "• 医学图像可视化\n"
            "• 体渲染和多平面重建\n"
            "• 图像处理和分析\n"
            "• 插件架构支持");
    }

private:
    void setupUI() {
        auto centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        auto layout = new QVBoxLayout(centralWidget);
        
        auto titleLabel = new QLabel("医学成像软件", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; margin: 20px; }");
        
        auto descLabel = new QLabel(
            "欢迎使用医学成像软件！\n\n"
            "当前版本包含以下功能模块：\n"
            "✓ 数据模型 (DataModel)\n"
            "✓ 渲染引擎 (RenderingEngine)\n"
            "✓ 图像处理器 (ImageProcessor)\n"
            "✓ 用户界面 (GUI)\n"
            "✓ 插件系统 (Plugins)\n\n"
            "请使用菜单栏中的功能开始您的工作。",
            this);
        descLabel->setAlignment(Qt::AlignCenter);
        descLabel->setWordWrap(true);
        
        auto openButton = new QPushButton("打开医学图像", this);
        connect(openButton, &QPushButton::clicked, this, &SimpleMainWindow::openFile);
        
        layout->addWidget(titleLabel);
        layout->addWidget(descLabel);
        layout->addWidget(openButton);
        layout->addStretch();

        setWindowTitle("医学成像软件 - Demo");
        resize(600, 400);
    }

    void setupMenuBar() {
        auto fileMenu = menuBar()->addMenu("文件(&F)");
        fileMenu->addAction("打开(&O)", this, &SimpleMainWindow::openFile, QKeySequence::Open);
        fileMenu->addSeparator();
        fileMenu->addAction("退出(&X)", this, &QWidget::close, QKeySequence::Quit);

        auto helpMenu = menuBar()->addMenu("帮助(&H)");
        helpMenu->addAction("关于(&A)", this, &SimpleMainWindow::showAbout);
    }

    void setupStatusBar() {
        statusBar()->showMessage("就绪");
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("医学成像软件");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Medical Imaging Solutions");

    try {
        SimpleMainWindow window;
        window.show();

        return app.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "错误", 
            QString("应用程序启动失败：%1").arg(e.what()));
        return -1;
    }
}

#include "main.moc"
