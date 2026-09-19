#include "ImageProcessor.h"

#include <QColor>
#include <QtGlobal>
#include <cmath>

ImageProcessor::ImageProcessor(QObject *parent)
    : QThread(parent), m_type(TaskNone)
{
}

void ImageProcessor::startTask(const QImage &img, TaskType type)
{
    // 上一次任务还没跑完就先忽略，防止重复 start 导致报错
    if (isRunning()) {
        return;
    }

    m_src = img;
    m_type = type;
    start();
}

void ImageProcessor::run()
{
    if (m_src.isNull() || m_type == TaskNone) {
        return;
    }

    QImage result;
    switch (m_type) {
    case TaskGray:
        result = toGray(m_src);
        break;
    case TaskEdge:
        result = sobelEdge(m_src);
        break;
    default:
        break;
    }

    emit processed(result);
}

// 灰度化：对每个像素取 R/G/B 的加权平均值
QImage ImageProcessor::toGray(const QImage &src)
{
    QImage gray(src.size(), QImage::Format_RGB32);

    for (int y = 0; y < src.height(); ++y) {
        for (int x = 0; x < src.width(); ++x) {
            int g = qGray(src.pixel(x, y));
            gray.setPixel(x, y, qRgb(g, g, g));
        }
    }
    return gray;
}

// Sobel 边缘检测：先用灰度图，再分别算 X/Y 方向的梯度，合成幅值
QImage ImageProcessor::sobelEdge(const QImage &src)
{
    QImage gray = toGray(src);
    QImage edge(gray.size(), QImage::Format_RGB32);

    // 3x3 卷积核
    static const int gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    static const int gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int y = 1; y < gray.height() - 1; ++y) {
        for (int x = 1; x < gray.width() - 1; ++x) {
            int sx = 0;
            int sy = 0;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int g = qGray(gray.pixel(x + dx, y + dy));
                    sx += g * gx[dy + 1][dx + 1];
                    sy += g * gy[dy + 1][dx + 1];
                }
            }

            int mag = static_cast<int>(std::sqrt(sx * sx + sy * sy));
            mag = qBound(0, mag, 255);
            edge.setPixel(x, y, qRgb(mag, mag, mag));
        }
    }
    return edge;
}
