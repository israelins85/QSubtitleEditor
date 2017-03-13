#ifndef SUBTITLE_H
#define SUBTITLE_H

#include "subtitleitem.h"

#include <QObject>

class Subtitle : public QObject
{
    Q_OBJECT
private:
    QList<SubtitleItem> m_itens;
    bool m_changed = false;

public:
    Subtitle();

    bool clear();

    bool add(const SubtitleItem& a_item);
    bool remove(const qint32& a_idx);
    bool replace(qint32 a_idx, const SubtitleItem& a_item);

    const SubtitleItem& at(qint32 a_idx) const;

    SubtitleItem byTime(TimeStamp a_msec) const;

    const QList<SubtitleItem>& itens() const;

    bool changed() const;

    enum class IdxType {
        Exact, Near
    };
    qint32 idxByTime(TimeStamp a_msec, IdxType a_idxType = IdxType::Exact) const;

signals:
    void itemAdded(qint32 a_idx, const SubtitleItem& a_item);
    void itemRemoved(qint32 a_idx);
    void itensRemoved();
};

#endif // SUBTITLE_H
