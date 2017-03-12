#include "subtitleitem.h"

#include <QRegularExpression>

TimeStamp::TimeStamp(qint64 a_value):
    m_value(a_value)
{}

qint64 TimeStamp::value() const
{
    return m_value;
}

void TimeStamp::setValue(const qint64& value)
{
    m_value = value;
}

bool TimeStamp::operator ==(const TimeStamp& a_other) const {
    return (m_value == a_other.m_value);
}

bool TimeStamp::operator ==(qint64 a_value) const {
    return (m_value == a_value);
}

bool TimeStamp::operator >=(const TimeStamp& a_other) const {
    return (m_value >= a_other.m_value);
}

bool TimeStamp::operator >=(qint64 a_value) const {
    return (m_value >= a_value);
}

bool TimeStamp::operator <=(const TimeStamp& a_other) const {
    return (m_value <= a_other.m_value);
}

bool TimeStamp::operator <=(qint64 a_value) const {
    return (m_value <= a_value);
}

bool TimeStamp::operator >(const TimeStamp& a_other) const {
    return (m_value > a_other.m_value);
}

bool TimeStamp::operator >(qint64 a_value) const {
    return (m_value > a_value);
}

bool TimeStamp::operator <(const TimeStamp& a_other) const {
    return (m_value < a_other.m_value);
}

bool TimeStamp::operator <(qint64 a_value) const {
    return (m_value < a_value);
}

bool TimeStamp::operator !=(const TimeStamp& a_other) const {
    return (m_value != a_other.m_value);
}

TimeStamp TimeStamp::operator -(qint64 a_value) const
{
    return (m_value - a_value);
}

TimeStamp TimeStamp::operator -(const TimeStamp& a_other) const
{
    return (m_value - a_other.m_value);
}

TimeStamp TimeStamp::operator +(qint64 a_value) const
{
    return (m_value + a_value);
}

TimeStamp TimeStamp::operator +(const TimeStamp& a_other) const
{
    return (m_value + a_other.m_value);
}

bool TimeStamp::operator !=(qint64 a_value) const {
    return (m_value != a_value);
}

QString TimeStamp::toString(TimeStamp::Format a_format)
{
    QString l_ret;
    qint64 l_ms = m_value;
    qint64 l_sec = l_ms / 1000; l_ms = l_ms % 1000;
    qint64 l_min = l_sec / 60; l_sec = l_sec % 60;
    qint64 l_hour = l_min / 60; l_hour = l_hour % 60;

    switch (a_format) {
        case Format::Human:
            l_ret = QString("%1:%2:%3.%4")
                .arg(l_hour, 2, 10, QChar('0'))
                .arg(l_min, 2, 10, QChar('0'))
                .arg(l_sec, 2, 10, QChar('0'))
                .arg(l_ms, 3, 10, QChar('0'));
        case Format::Str:
            l_ret = QString("%1:%2:%3,%4")
                .arg(l_hour, 2, 10, QChar('0'))
                .arg(l_min, 2, 10, QChar('0'))
                .arg(l_sec, 2, 10, QChar('0'))
                .arg(l_ms, 3, 10, QChar('0'));
    }

    return l_ret;
}

TimeStamp TimeStamp::fromString(TimeStamp::Format a_format, QString a_string)
{
    QRegularExpression l_regex;
    qint64 l_ms = 0;
    qint64 l_sec = 0;
    qint64 l_min = 0;
    qint64 l_hour = 0;

    switch (a_format) {
        case Format::Human:
            l_regex.setPattern("^(?<hour>\\d\\d):(?<min>\\d\\d):(?<sec>\\d\\d)\\.(?<msec>\\d\\d\\d)$");
        case Format::Str:
            l_regex.setPattern("^(?<hour>\\d\\d):(?<min>\\d\\d):(?<sec>\\d\\d),(?<msec>\\d\\d\\d)$");
    }

    QRegularExpressionMatch l_match = l_regex.match(a_string);
    if (l_match.hasMatch()) {
        l_hour = l_match.captured("hour").toLongLong();
        l_min = l_match.captured("min").toLongLong();
        l_sec = l_match.captured("sec").toLongLong();
        l_ms = l_match.captured("msec").toLongLong();
    }

    return TimeStamp(TimeStamp::msFromTime(l_hour, l_min, l_sec, l_ms));
}

qint64 TimeStamp::msFromTime(qint64 a_hour, qint64 a_min, qint64 a_sec, qint64 a_msec)
{
    a_min += (a_hour * 60);
    a_sec += (a_min * 60);
    a_msec += (a_sec * 1000);
    return a_msec;
}


SubtitleItem::SubtitleItem()
{
    qRegisterMetaType<SubtitleItem>("SubtitleItem");
}

TimeStamp SubtitleItem::start() const
{
    return m_start;
}

void SubtitleItem::setStart(const TimeStamp& startTime)
{
    m_start = startTime;
}

TimeStamp SubtitleItem::end() const
{
    return m_end;
}

void SubtitleItem::setEnd(const TimeStamp& endTime)
{
    m_end = endTime;
}

TimeStamp SubtitleItem::duration() const {
    return m_end - m_start;
}

QString SubtitleItem::text(bool withTags) const
{
    QString l_ret = m_text;
    if (!withTags) {
        l_ret.remove("<b>").remove("</b>");
        l_ret.remove("<i>").remove("</i>");
        l_ret.remove("<u>").remove("</u>");
    }
    return l_ret;
}

void SubtitleItem::setText(const QString& text, const TextAtributes& a_atributes)
{
    m_text = text;
    if (a_atributes.testFlag(TextAtribute::Bold)) {
        m_text.remove("<b>").remove("</b>");
        m_text = "<b>" + m_text + "</b>";
    }
    if (a_atributes.testFlag(TextAtribute::Italic)) {
        m_text.remove("<i>").remove("</i>");
        m_text = "<i>" + m_text + "</i>";
    }
    if (a_atributes.testFlag(TextAtribute::Underline)) {
        m_text.remove("<u>").remove("</u>");
        m_text = "<u>" + m_text + "</u>";
    }
}

bool SubtitleItem::operator ==(const SubtitleItem& a_other) const
{
    if (m_start != a_other.m_start) return false;
    if (m_end != a_other.m_end) return false;
    if (m_text != a_other.m_text) return false;
    return true;
}

bool SubtitleItem::operator !=(const SubtitleItem& a_other) const
{
    return !(*this == a_other);
}

bool SubtitleItem::isBold() const
{
    return m_text.contains("<b>");
}

bool SubtitleItem::isItalic() const
{
    return m_text.contains("<i>");
}

bool SubtitleItem::isUnderline() const
{
    return m_text.contains("<u>");
}
