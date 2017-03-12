#include "subtitleparsersrt.h"

#include <QRegularExpression>

SubtitleParserSrt::SubtitleParserSrt(QObject* a_parent) : SubtitleParser(a_parent)
{}

bool SubtitleParserSrt::step()
{
    SubtitleItem l_item;

    while (m_data[m_cursor] == '\n')
        readLine();

    if (atEnd()) return true;

    // #
    qint32 l_number = readLine().trimmed().toInt();
    if (l_number != m_number + 1) return false;
    m_number = l_number;

    // hh:mm:ss,zzz --> hh:mm:ss,zzz
    QString l_timeStamp = readLine().trimmed();

    static QRegularExpression sl_rex("(\\d{2}:\\d{2}:\\d{2},\\d{3}) --> (\\d{2}:\\d{2}:\\d{2},\\d{3})");
    QRegularExpressionMatch l_match = sl_rex.match(l_timeStamp);
    if (!l_match.hasMatch()) return false;
    l_item.setStart(TimeStamp::fromString(TimeStamp::Format::Str, l_match.captured(1)));
    l_item.setEnd(TimeStamp::fromString(TimeStamp::Format::Str, l_match.captured(2)));

    // TEXT
    QString l_text;
    QString l_line = readLine();

    while ((l_line != "\n") && !l_line.isEmpty()) {
        l_text += l_line;
        l_line = readLine();
    }
    if (l_text.endsWith('\n')) l_text.chop(1);
    l_item.setText(l_text);

    m_subtitle->add(l_item);
    return true;
}
