#ifndef SUBTITLEITEM_H
#define SUBTITLEITEM_H

#include <QTime>
#include <QUuid>
#include <QObject>

class TimeStamp {
public:
    TimeStamp(qint64 a_value = 0);

    qint64 value() const;
    void setValue(const qint64& value);

    TimeStamp& operator = (qint64 a_value) {
        m_value = a_value;
        return *this;
    }

    bool operator == (qint64 a_value) const;
    bool operator == (const TimeStamp& a_other) const;
    bool operator >= (qint64 a_value) const;
    bool operator >= (const TimeStamp& a_other) const;
    bool operator <= (qint64 a_value) const;
    bool operator <= (const TimeStamp& a_other) const;
    bool operator > (qint64 a_value) const;
    bool operator > (const TimeStamp& a_other) const;
    bool operator < (qint64 a_value) const;
    bool operator < (const TimeStamp& a_other) const;
    bool operator != (qint64 a_value) const;
    bool operator != (const TimeStamp& a_other) const;

    TimeStamp operator -(qint64 a_value) const;
    TimeStamp operator - (const TimeStamp& a_other) const;
    TimeStamp operator + (qint64 a_value) const;
    TimeStamp operator + (const TimeStamp& a_other) const;

    enum class Format {
        Human,
        Str
    };

    QString toString(Format a_format = Format::Human);

    static TimeStamp fromString(Format a_format, QString a_string);
    static qint64 msFromTime(qint64 a_hour, qint64 a_min, qint64 a_sec, qint64 a_msec);

private:
    qint64 m_value = 0;
};

class SubtitleItem
{
private:
    TimeStamp m_start;
    TimeStamp m_end;
    QString m_text;

public:
    SubtitleItem();

    TimeStamp start() const;
    void setStart(const TimeStamp& start);
    TimeStamp end() const;
    void setEnd(const TimeStamp& end);
    TimeStamp duration() const;

    bool contains(const TimeStamp& time) const;

    enum class TextAtribute {
        None        = 0x00,
        Bold        = 0x01,
        Italic      = 0x02,
        Underline   = 0x04
    };

    Q_DECLARE_FLAGS(TextAtributes, TextAtribute)

    QString text(bool withTags = true) const;
    void setText(const QString& text, const TextAtributes& a_atributes = TextAtribute::None);

    bool operator == (const SubtitleItem& a_other) const;
    bool operator != (const SubtitleItem& a_other) const;

    bool isBold() const;
    bool isItalic() const;
    bool isUnderline() const;
};

Q_DECLARE_METATYPE(SubtitleItem);
Q_DECLARE_OPERATORS_FOR_FLAGS(SubtitleItem::TextAtributes)

#endif // SUBTITLEITEM_H
