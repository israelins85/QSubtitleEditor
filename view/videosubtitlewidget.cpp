#include "videosubtitlewidget.h"

#include <QLabel>
#include <QPainter>
#include <QEasingCurve>
#include <QDebug>

VideoSubtitleWidget::VideoSubtitleWidget(QWidget *parent) : QVideoWidget(parent)
{
    m_lblSubTitle = new QLabel(this);
    m_lblSubTitle->setStyleSheet("background-color: rgba(255, 150, 150, 150);");
}

//void VideoSubtitleWidget::paintEvent(QPaintEvent* event)
//{
//    QVideoWidget::paintEvent(event);

//    QImage l_image = generateSubtitleImage();
//    QRect l_rectSource = QRect(QPoint(0, 0), l_image.size());
//    QRect l_rectTarget = l_rectSource;

//    l_rectTarget.setTop((height() - l_rectTarget.height()) / 2);
//    l_rectTarget.setLeft((width() - l_rectTarget.width()) / 2);

//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);
//    painter.drawImage(l_rectTarget, l_image, l_rectSource);
//}

SubtitleItem VideoSubtitleWidget::currentSubtitleItem() const
{
    return m_currentSubtitleItem;
}

void VideoSubtitleWidget::setCurrentSubtitleItem(const SubtitleItem& currentSubtitleItem)
{
    if (m_currentSubtitleItem != currentSubtitleItem) {
        m_currentSubtitleItem = currentSubtitleItem;
        m_lblSubTitle->setPixmap(QPixmap::fromImage(generateSubtitleImage()));
    }
//    update();
}

QImage makeSmoothTransparent(const QImage& a_image, QRgb a_backgroundColor, qint32 a_outline)
{
    QImage l_txtPixmapWOutLine = a_image;
    QEasingCurve l_easingCurve(QEasingCurve::OutQuint);

    for (int x = 0; x < a_image.width(); x++) {
        for (int y = 0; y < a_image.height(); y++) {
            QRgb l_rgb = a_image.pixel(x, y);
            if (l_rgb != a_backgroundColor) continue;

            qint32 l_minx2 = qMax(0, x - a_outline);
            qint32 l_maxx2 = qMin(a_image.width(), x + a_outline + 1);
            qint32 l_miny2 = qMax(0, y - a_outline);
            qint32 l_maxy2 = qMin(a_image.height(), y + a_outline + 1);

            qint32 l_pixels = 0;
            qint32 l_eqPixels = 0;

            for (int x2 = l_minx2; x2 < l_maxx2; x2++) {
                for (int y2 = l_miny2; y2 < l_maxy2; y2++) {
                    if ((x2 == x) && (y2 == y)) continue;
                    ++l_pixels;
                    QRgb l_rgb = a_image.pixel(x2, y2);
                    if (l_rgb != a_backgroundColor) continue;
                    ++l_eqPixels;
                }
            }

            qreal l_p = l_easingCurve.valueForProgress(1.0 - (qreal(l_eqPixels) / qreal(l_pixels)));
            l_rgb = qPremultiply(qRgba(qRed(l_rgb), qGreen(l_rgb), qBlue(l_rgb), 255 * l_p));
            l_txtPixmapWOutLine.setPixel(x, y, l_rgb);
        }
    }

    return l_txtPixmapWOutLine;
}

QImage VideoSubtitleWidget::generateSubtitleImage()
{
    static QImage sl_txtPixmap;
    static SubtitleItem sl_cachedItem;
    static QFont sl_cachedFont;

    bool l_needRegenerate = ((m_currentSubtitleItem != sl_cachedItem) || (sl_txtPixmap.size() != size()));
    if (l_needRegenerate) {
        sl_cachedItem = m_currentSubtitleItem;
        QRect l_rect = QRect(QPoint(0, 0), m_lblSubTitle->size());

        if (sl_txtPixmap.size() != l_rect.size())
            sl_txtPixmap = QImage(l_rect.size(), QImage::Format_ARGB32_Premultiplied);

        QPainter l_painter;

        sl_cachedFont = font();
        if (sl_cachedFont.pointSizeF() < 0)
            sl_cachedFont.setPointSize(sl_cachedFont.pixelSize() * 2);
        else
            sl_cachedFont.setPointSizeF(sl_cachedFont.pointSizeF() * 2);

        l_painter.begin(&sl_txtPixmap);
        if (!m_currentSubtitleItem.text().isEmpty()) {
            l_painter.fillRect(l_rect, Qt::white);
            l_painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            sl_cachedFont.setBold(m_currentSubtitleItem.isBold());
            sl_cachedFont.setItalic(m_currentSubtitleItem.isItalic());
            sl_cachedFont.setUnderline(m_currentSubtitleItem.isUnderline());
            l_painter.setFont(sl_cachedFont);
            l_painter.setPen(QColor(0, 0, 0));
            l_painter.drawText(l_rect, Qt::TextWordWrap | Qt::AlignHCenter | Qt::AlignBottom, m_currentSubtitleItem.text(false));
        } else {
            l_painter.fillRect(l_rect, Qt::transparent);
        }
        l_painter.end();

        if (!m_currentSubtitleItem.text().isEmpty()) {
            sl_txtPixmap = makeSmoothTransparent(sl_txtPixmap, qRgb(255, 255, 255), 2);
        }
    }

    return sl_txtPixmap;
}

void VideoSubtitleWidget::resizeEvent(QResizeEvent* /*event*/)
{
    if (!size().isValid()) return;

    QRect l_rect = QRect(QPoint(0, 0), size() * 0.9);
    l_rect.moveCenter(mapFromParent(geometry().center()));
    qDebug() << l_rect;
    m_lblSubTitle->setGeometry(l_rect);
    m_lblSubTitle->setPixmap(QPixmap::fromImage(generateSubtitleImage()));
    m_lblSubTitle->raise();
}
