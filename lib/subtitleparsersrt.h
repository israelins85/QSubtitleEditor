#ifndef SUBTITLEPARSERSRT_H
#define SUBTITLEPARSERSRT_H

#include "subtitleparser.h"

class SubtitleParserSrt : public SubtitleParser
{
private:
    qint32 m_number = 0;
public:
    SubtitleParserSrt(QObject* a_parent = nullptr);

    virtual bool step();
};

#endif // SUBTITLEPARSERSRT_H
