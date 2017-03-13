#ifndef VIDEOSUBTITLEWIDGET_H
#define VIDEOSUBTITLEWIDGET_H

#include <QGraphicsView>
#include "lib/subtitleitem.h"

class QLabel;
class QGraphicsVideoItem;
class VideoSubtitleWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit VideoSubtitleWidget(QWidget *parent = 0);

    SubtitleItem currentSubtitleItem() const;
    void setCurrentSubtitleItem(const SubtitleItem& currentSubtitleItem);

    QGraphicsVideoItem* videoOutput();

private:
    void setupUi();
    QFont calculateBestFontSize();
    void adjustSubtitleItem();

private:
    SubtitleItem m_currentSubtitleItem;
    QGraphicsScene* m_scene = nullptr;
    QGraphicsVideoItem* m_videoOutput = nullptr;
    QGraphicsTextItem* m_subtitleBottomOutput = nullptr;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // VIDEOSUBTITLEWIDGET_H
