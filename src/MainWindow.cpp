#include "MainWindow.h"
#include "ImageProcessor.h"

#include <QLabel>
#include <QScrollArea>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QKeySequence>
#include <QStatusBar>
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setMinimumSize(400, 300);
    m_imageLabel->setStyleSheet("background-color: #2b2b2b;");

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidget(m_imageLabel);
    m_scrollArea->setWidgetResizable(true);
    setCentralWidget(m_scrollArea);

    m_processor = new ImageProcessor(this);
    connect(m_processor, &ImageProcessor::processed,
            this, &MainWindow::onProcessed);

    // 菜单栏
    QMenu *fileMenu = menuBar()->addMenu("文件(&F)");
    QAction *openAct = fileMenu->addAction("打开图片(&O)...");
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::openImage);

    fileMenu->addSeparator();
    QAction *exitAct = fileMenu->addAction("退出(&Q)");
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    // 工具栏
    QToolBar *toolBar = addToolBar("处理");
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction *resetAct = toolBar->addAction("还原");
    connect(resetAct, &QAction::triggered, this, &MainWindow::showOriginal);

    QAction *grayAct = toolBar->addAction("灰度");
    connect(grayAct, &QAction::triggered, this, &MainWindow::showGray);

    QAction *edgeAct = toolBar->addAction("边缘检测");
    connect(edgeAct, &QAction::triggered, this, &MainWindow::showEdge);

    statusBar()->showMessage("就绪：请通过 文件 -> 打开图片 载入图像");
}

MainWindow::~MainWindow()
{
    // 析构前确保后台线程已结束，防止线程还在运行时对象被销毁
    if (m_processor->isRunning()) {
        m_processor->wait();
    }
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "打开图片", QString(),
        "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)");

    if (fileName.isEmpty()) {
        return;
    }

    QImage img(fileName);
    if (img.isNull()) {
        QMessageBox::warning(this, "提示", "无法读取该图片文件！");
        return;
    }

    m_srcImage = img;
    showOriginal();
}

void MainWindow::showOriginal()
{
    if (m_srcImage.isNull()) {
        return;
    }
    updateDisplay(m_srcImage);
}

void MainWindow::showGray()
{
    if (m_srcImage.isNull()) {
        return;
    }
    m_processor->startTask(m_srcImage, ImageProcessor::TaskGray);
}

void MainWindow::showEdge()
{
    if (m_srcImage.isNull()) {
        return;
    }
    m_processor->startTask(m_srcImage, ImageProcessor::TaskEdge);
}

void MainWindow::onProcessed(const QImage &img)
{
    updateDisplay(img);
}

void MainWindow::updateDisplay(const QImage &img)
{
    m_imageLabel->setPixmap(QPixmap::fromImage(img));
    m_imageLabel->resize(img.size());
    statusBar()->showMessage(
        QString("尺寸: %1 x %2").arg(img.width()).arg(img.height()));
}
