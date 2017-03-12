#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lib/subtitle.h"

#include <QMainWindow>
#include <QMediaPlayer>

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
    void showSubtitleItem();
    void showSubtitleItem(const SubtitleItem& a_item);

private:
    Ui::MainWindow *ui;
    Subtitle m_subTitle;
    QMediaPlayer* m_mediaPlayer = nullptr;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
