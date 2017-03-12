#ifndef VIDEOSUBTITLEWIDGET_H
#define VIDEOSUBTITLEWIDGET_H


#include <QVideoWidget>
#include "lib/subtitleitem.h"

class QLabel;
class VideoSubtitleWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit VideoSubtitleWidget(QWidget *parent = 0);

    SubtitleItem currentSubtitleItem() const;
    void setCurrentSubtitleItem(const SubtitleItem& currentSubtitleItem);

private:
    QImage generateSubtitleImage();

private:
    SubtitleItem m_currentSubtitleItem;
    QLabel* m_lblSubTitle = nullptr;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // VIDEOSUBTITLEWIDGET_H
