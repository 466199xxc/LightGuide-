#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QThread>
#include <QImage>

// 后台图像处理线程：把耗时的处理放到子线程，避免阻塞界面
class ImageProcessor : public QThread
{
    Q_OBJECT

public:
    enum TaskType {
        TaskNone = 0,   // 无任务
        TaskGray,       // 灰度化
        TaskEdge        // Sobel 边缘检测
    };

    explicit ImageProcessor(QObject *parent = nullptr);

    // 启动一个处理任务，type 为处理类型
    void startTask(const QImage &img, TaskType type);

signals:
    // 处理完成后发出结果图像
    void processed(const QImage &img);

protected:
    void run() override;

private:
    QImage toGray(const QImage &src);
    QImage sobelEdge(const QImage &src);

    QImage m_src;      // 待处理图像
    TaskType m_type;   // 当前任务类型
};

#endif // IMAGEPROCESSOR_H
