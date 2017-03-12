#include "subtitle.h"

bool Subtitle::changed() const
{
    return m_changed;
}

Subtitle::Subtitle()
{}

bool Subtitle::clear()
{
    m_itens.clear();
    emit itensRemoved();
    return true;
}

bool Subtitle::add(const SubtitleItem& a_item)
{
    qint32 l_insertPoint = m_itens.size();

    for (qint32 l_idx = 0; l_idx < m_itens.size(); ++l_idx) {
        const SubtitleItem& l_s = m_itens[l_idx];
        if (l_s.start() > a_item.start()) break;
        l_insertPoint = l_idx + 1;
    }

    m_itens.insert(l_insertPoint, a_item);
    if (l_insertPoint < 0) l_insertPoint = 0;
    if (l_insertPoint >= m_itens.size()) l_insertPoint = m_itens.size() - 1;
    emit itemAdded(l_insertPoint, a_item);
    m_changed = true;

    return true;
}

bool Subtitle::remove(const qint32& a_idx)
{
    m_itens.removeAt(a_idx);
    emit itemRemoved(a_idx);
    m_changed = true;
    return true;
}

bool Subtitle::replace(qint32 a_idx, const SubtitleItem& a_item)
{
    if (a_item == m_itens[a_idx]) return true;
    if (!remove(a_idx)) return false;
    return add(a_item);
}

SubtitleItem Subtitle::byTime(qint64 a_msec) const
{
    qint32 l_idx = idxByTime(a_msec);

    if (l_idx >= 0) {
        return m_itens[l_idx];
    }

    return SubtitleItem();
}

const QList<SubtitleItem>& Subtitle::itens() const
{
    return m_itens;
}

qint32 Subtitle::idxByTime(qint64 a_msec) const
{
    qint32 l_idx = 0;
    for (; l_idx < m_itens.size(); ++l_idx) {
        const SubtitleItem& l_s = m_itens[l_idx];
        if (l_s.start() > a_msec) break;
        if (l_s.end() < a_msec) continue;
        return l_idx;
    }
    return -1;
}
