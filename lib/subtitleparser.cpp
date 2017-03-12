#include "subtitleparser.h"
#include "subtitleparsersrt.h"

#include <QFile>
#include <QTextCodec>
#include <QCoreApplication>
#include <QDir>
#include <QMimeDatabase>
#include <QDebug>
#include <QtConcurrent>
#include <QThread>

QString SubtitleParser::read(qint32 l_chars)
{
    QString l_ret = m_data.mid(m_cursor, l_chars);
    m_cursor += l_chars;
    if (m_data.size()) {
        emit progressChanged(qreal(m_cursor) / qreal(m_data.size()));
    }
    return l_ret;
}

QString SubtitleParser::readLine()
{
    qint32 l_indexOfLf = m_data.indexOf('\n', m_cursor);
    if (l_indexOfLf < 0) l_indexOfLf = m_data.size();
    return read(l_indexOfLf - m_cursor + 1);
}

SubtitleParser::SubtitleParser(QObject* a_parent) :
    QObject(a_parent), m_running(false), m_canceled(false)
{}

bool SubtitleParser::setDataFromFile(const QString& a_fileName)
{
    QFile l_file(a_fileName);
    if (!l_file.open(QFile::ReadOnly)) return false;
    m_fileName = a_fileName;
    return setDataBinary(l_file.readAll());
}

QString SubtitleParser::videoFileName()
{
    QFileInfo l_fi(m_fileName);
    QDir l_dir = l_fi.absoluteDir();
    QString l_completeBaseName = l_fi.completeBaseName();
    l_fi.setFile(l_completeBaseName);
    QString l_completeBaseNameWithoutLang = l_fi.completeBaseName();

    qDebug() << l_dir;

    QFileInfoList l_fileInfoList = l_dir.entryInfoList(QDir::Files, QDir::Name);
    QMimeDatabase db;
    QFileInfoList l_fileInfoListEq;
    QFileInfoList l_fileInfoListOtherLang;

    for (qint32 i = 0; i < l_fileInfoList.size(); ++i) {
        l_fi = l_fileInfoList[i];
        QString l_curBaseName = l_fi.completeBaseName();
        if (l_curBaseName == l_completeBaseName) {
            l_fileInfoListEq += l_fi;
        } else
        if (l_curBaseName == l_completeBaseNameWithoutLang) {
            l_fileInfoListOtherLang += l_fi;
        }
    }

    l_fileInfoList = l_fileInfoListEq + l_fileInfoListOtherLang;

    for (qint32 i = 0; i < l_fileInfoList.size(); ++i) {
        l_fi = l_fileInfoList[i];
        QMimeType type = db.mimeTypeForFile(l_fi.absoluteFilePath());
        qDebug() << l_fi.absoluteFilePath() <<
                    type.name();

        if (type.name().contains("video", Qt::CaseInsensitive))
            return l_fi.absoluteFilePath();
    }

    return QString();
}

bool SubtitleParser::setDataBinary(const QByteArray& data)
{
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    m_data = codec->toUnicode(data.constData(), data.size(), &state);
    if (state.invalidChars > 0) {
        codec = QTextCodec::codecForUtfText(data, QTextCodec::codecForName("Windows-1252"));
        m_data = codec->toUnicode(data);
    }
    // ensure only LF
    m_data.replace("\r\n", "\n");
    return true;
}

bool SubtitleParser::setDataBinary(const QByteArray& data, const QByteArray& a_codec)
{
    QTextCodec *codec = QTextCodec::codecForName(a_codec);
    m_data = codec->toUnicode(data.constData(), data.size());
    // ensure only LF
    m_data.replace("\r\n", "\n");
    return true;
}

qint32 SubtitleParser::cursor() const
{
    return m_cursor;
}

bool SubtitleParser::atEnd() const
{
    return m_cursor == m_data.size();
}

SubtitleParser* SubtitleParser::fromFile(Subtitle* a_subtitle, QString a_fileName)
{
    SubtitleParser* l_parser = nullptr;

    if (a_fileName.endsWith(".srt", Qt::CaseInsensitive))
        l_parser = new SubtitleParserSrt();

    if (l_parser) {
        l_parser->setSubtitle(a_subtitle);
        l_parser->setDataFromFile(a_fileName);
    }

    return l_parser;
}

Subtitle* SubtitleParser::subtitle() const
{
    return m_subtitle;
}

void SubtitleParser::setSubtitle(Subtitle* subtitle)
{
    m_subtitle = subtitle;
}

bool SubtitleParser::start()
{
    QWriteLocker l_runningLocker(&m_runningRWLock);
    if (m_running) return false;
    m_running = true;
    QWriteLocker l_canceledLocker(&m_canceledRWLock);
    m_canceled = false;

    QtConcurrent::run([this]() {
        while (!atEnd()) {
            step();
            QReadLocker l_canceledLocker(&m_canceledRWLock);
            if (m_canceled) break;
        }
        emit finished(!m_canceled);
        QWriteLocker l_runningLocker(&m_runningRWLock);
        m_running = false;
    });

    return true;
}

void SubtitleParser::cancel()
{
    QWriteLocker l_canceledLocker(&m_canceledRWLock);
    m_canceled = true;
}

bool SubtitleParser::isRunning()
{
    QReadLocker l_runningLocker(&m_runningRWLock);
    return m_running;
}

bool SubtitleParser::isCanceled()
{
    QReadLocker l_canceledLocker(&m_canceledRWLock);
    return m_canceled;
}
