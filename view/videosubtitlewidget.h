#ifndef VIDEOSUBTITLEWIDGET_H
#define VIDEOSUBTITLEWIDGET_H

#include "lib/subtitleitem.h"
#include <QWidget>

class QLabel;
class QResizeEvent;
class QAbstractVideoSurface;
class VideoSubtitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoSubtitleWidget(QWidget *parent = 0);

    SubtitleItem currentSubtitleItem() const;
    void setCurrentSubtitleItem(const SubtitleItem& currentSubtitleItem);

    QAbstractVideoSurface* surface();

private:
    QImage generateSubtitleImage();
    void updatePixmaps();

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent* event);

private:
    SubtitleItem m_currentSubtitleItem;
    QLabel* m_lblVideo = nullptr;
    QLabel* m_lblSubTitle = nullptr;
    QImage m_imgFrame;
};

#endif // VIDEOSUBTITLEWIDGET_H
