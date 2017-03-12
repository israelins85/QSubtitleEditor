#ifndef SUBTITLEPARSER_H
#define SUBTITLEPARSER_H

#include "subtitle.h"

#include <QReadWriteLock>

class SubtitleParser : public QObject
{
    Q_OBJECT

protected:
    Subtitle* m_subtitle;
    QString m_fileName;
    QString m_data;
    qint32 m_cursor = 0;
    QReadWriteLock m_runningRWLock;
    bool m_running = false;
    QReadWriteLock m_canceledRWLock;
    bool m_canceled = false;

    QString read(qint32 l_chars);
    QString readLine();

public:
    SubtitleParser(QObject* a_parent = nullptr);

    static SubtitleParser* fromFile(Subtitle* a_subtitle, QString a_fileName);

    bool setDataFromFile(const QString& a_fileName);

    QString videoFileName();

    bool setDataBinary(const QByteArray& data);
    bool setDataBinary(const QByteArray& data, const QByteArray& a_codec);

    qint32 cursor() const;
    qreal progress();

    bool atEnd() const;
    virtual bool step() = 0;

    Subtitle* subtitle() const;
    void setSubtitle(Subtitle* subtitle);

    bool start();
    void cancel();
    bool isRunning();
    bool isCanceled();

signals:
    void progressChanged(qreal a_progress);
    void finished(bool a_sucess);
};

#endif // SUBTITLEPARSER_H
