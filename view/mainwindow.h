#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lib/subtitle.h"

#include <QMainWindow>
#include <QMediaPlayer>
#include <QTimer>

namespace Ui {
    class MainWindow;
}

class QLabel;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionQuit_triggered();

    void on_btnBackward_clicked();

    void on_btnPlay_clicked();

    void on_btnFoward_clicked();

private:
    void syncSubtitle(qint64 a_position);
    void showSubtitleItem(const SubtitleItem& a_item);

private:
    Ui::MainWindow *ui;
    Subtitle m_subTitle;
    QMediaPlayer* m_mediaPlayer = nullptr;
    qint32 m_curSubtitleIdx = -1;
    QTimer m_timerForSyncSubtitle;
};

#endif // MAINWINDOW_H
