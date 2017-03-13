#include "videosubtitlewidget.h"

#include <QLabel>
#include <QPainter>
#include <QEasingCurve>
#include <QDebug>
#include <QGraphicsTextItem>
#include <QGraphicsVideoItem>
#include <QGraphicsDropShadowEffect>

VideoSubtitleWidget::VideoSubtitleWidget(QWidget *parent) : QGraphicsView(parent)
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setStyleSheet( "VideoSubtitleWidget { border-style: none; background-color:black}" );

    setupUi();
}

SubtitleItem VideoSubtitleWidget::currentSubtitleItem() const
{
    return m_currentSubtitleItem;
}

void VideoSubtitleWidget::setCurrentSubtitleItem(const SubtitleItem& currentSubtitleItem)
{
    if (m_currentSubtitleItem != currentSubtitleItem) {
        m_currentSubtitleItem = currentSubtitleItem;
        m_subtitleBottomOutput->setHtml("<center>" + m_currentSubtitleItem.text().replace("\n", "<br/>") + "</center>");
        qDebug() << "showing" << m_currentSubtitleItem.start().value() << m_currentSubtitleItem.end().value() << m_currentSubtitleItem.text();
        adjustSubtitleItem();
    }
}

QGraphicsVideoItem* VideoSubtitleWidget::videoOutput()
{
    return m_videoOutput;
}

void VideoSubtitleWidget::setupUi()
{
    m_scene = new QGraphicsScene(this);
    m_videoOutput = new QGraphicsVideoItem;
    m_videoOutput->setAspectRatioMode(Qt::KeepAspectRatio);
    m_scene->addItem(m_videoOutput);
    m_subtitleBottomOutput = new QGraphicsTextItem(m_videoOutput);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setBlurRadius(3);
    shadowEffect->setColor(QColor(Qt::black));

    m_subtitleBottomOutput->setGraphicsEffect(shadowEffect);
    m_subtitleBottomOutput->setDefaultTextColor(QColor(230, 230, 230));
    m_subtitleBottomOutput->setFont(calculateBestFontSize());

    setScene(m_scene);
}

QFont VideoSubtitleWidget::calculateBestFontSize()
{
    QFont l_font = font();
    l_font.setPointSizeF(20);
    return l_font;
}

void VideoSubtitleWidget::adjustSubtitleItem()
{
    m_subtitleBottomOutput->setTextWidth(width() - 10);
    m_subtitleBottomOutput->adjustSize();

    qint32 l_x = (width() - m_subtitleBottomOutput->boundingRect().width()) / 2;
    qint32 l_y = height() - m_subtitleBottomOutput->boundingRect().height() - 5;

    m_subtitleBottomOutput->setPos(l_x, l_y);
}

void VideoSubtitleWidget::resizeEvent(QResizeEvent* /*event*/)
{
    if (!size().isValid()) return;

    setSceneRect(QRect(QPoint(0, 0), size()));
    centerOn(m_videoOutput);
    m_videoOutput->setSize(size());

    m_subtitleBottomOutput->setFont(calculateBestFontSize());
    adjustSubtitleItem();
}
