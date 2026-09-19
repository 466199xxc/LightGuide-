#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

class QLabel;
class QScrollArea;
class ImageProcessor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openImage();
    void showOriginal();
    void showGray();
    void showEdge();
    void onProcessed(const QImage &img);

private:
    void updateDisplay(const QImage &img);

    QImage m_srcImage;          // 原始图像
    QLabel *m_imageLabel;       // 显示图像的标签
    QScrollArea *m_scrollArea;  // 支持滚动的区域
    ImageProcessor *m_processor; // 后台处理线程
};

#endif // MAINWINDOW_H
