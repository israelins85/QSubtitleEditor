#include "mainwindow.h"
#include "subtitleparser.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QFileDialog>
#include <QMediaPlaylist>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->wdgVideo->setAspectRatioMode(Qt::KeepAspectRatio);

    m_mediaPlayer = new QMediaPlayer(this);
    m_mediaPlayer->setVideoOutput(ui->wdgVideo);

    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, ui->sliProgress, [this](qint64 a_position) {
        ui->sliProgress->setMaximum(int(a_position / 1000));
        ui->wdgVideo->setMaximumSize(0, 0);
        QTimer::singleShot(100, this, [=]() {
            ui->wdgVideo->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        });
    });

    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, ui->sliProgress, [this](qint64 a_position) {
        ui->sliProgress->blockSignals(true);
        ui->sliProgress->setValue(a_position / 1000);
        ui->sliProgress->blockSignals(false);

        showSubtitleItem();
    });

    connect(ui->sliProgress, &QSlider::valueChanged, m_mediaPlayer, [this](qint32 a_value) {
        m_mediaPlayer->setPosition(qint64(a_value) * 1000);
    });

    connect(&m_subTitle, &Subtitle::itemAdded, this, [this](qint32 a_idx, const SubtitleItem& a_item) {
        QTableWidgetItem* l_item = nullptr;

        ui->tblSubtitles->insertRow(a_idx);
        // start
        l_item = ui->tblSubtitles->item(a_idx, 0);
        if (!l_item) {
            l_item = new QTableWidgetItem();
            l_item->setTextAlignment(Qt::AlignCenter);
            ui->tblSubtitles->setItem(a_idx, 0, l_item);
        }
        l_item->setText(a_item.start().toString());

        // end
        l_item = ui->tblSubtitles->item(a_idx, 1);
        if (!l_item) {
            l_item = new QTableWidgetItem();
            l_item->setTextAlignment(Qt::AlignCenter);
            ui->tblSubtitles->setItem(a_idx, 1, l_item);
        }
        l_item->setText(a_item.end().toString());

        // duration
        l_item = ui->tblSubtitles->item(a_idx, 2);
        if (!l_item) {
            l_item = new QTableWidgetItem();
            l_item->setTextAlignment(Qt::AlignCenter);
            ui->tblSubtitles->setItem(a_idx, 2, l_item);
        }
        l_item->setText(a_item.duration().toString());

        // text
        l_item = ui->tblSubtitles->item(a_idx, 3);
        if (!l_item) {
            l_item = new QTableWidgetItem();
            l_item->setTextAlignment(Qt::AlignCenter);
            ui->tblSubtitles->setItem(a_idx, 3, l_item);
        }
        QFont l_font = l_item->font();
        l_font.setBold(a_item.isBold());
        l_font.setItalic(a_item.isItalic());
        l_font.setUnderline(a_item.isUnderline());
        l_item->setFont(l_font);
        l_item->setText(a_item.text(false));

        ui->tblSubtitles->resizeRowToContents(a_idx);
    });

    connect(&m_subTitle, &Subtitle::itemRemoved, this, [this](qint32 a_idx) {
        ui->tblSubtitles->removeRow(a_idx);
    });

    connect(&m_subTitle, &Subtitle::itensRemoved, this, [this]() {
        ui->tblSubtitles->setRowCount(0);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString l_file = QFileDialog::getOpenFileName(this, tr("Select the subtitle file:"), QString(), "SRT File (*.srt)");
    if (l_file.isEmpty()) return;

    QProgressDialog* l_dlg = new QProgressDialog(tr("Loading subtitle..."), tr("Abort Load"), 0, 10000, this);
    l_dlg->setWindowModality(Qt::WindowModal);

    m_subTitle.clear();
    SubtitleParser* l_parser = SubtitleParser::fromFile(&m_subTitle, l_file);

    connect(l_dlg, &QProgressDialog::canceled, l_parser, [l_parser]() {
        l_parser->cancel();
    });

    connect(l_parser, &SubtitleParser::progressChanged, l_dlg, [l_dlg](qreal a_progress) {
        if (l_dlg->wasCanceled()) return;
        l_dlg->setValue(a_progress * 10000);
    });

    connect(l_parser, &SubtitleParser::finished, this, [this, l_parser, l_dlg](bool a_sucess) {
        if (a_sucess) {
            QString l_videoFile = l_parser->videoFileName();

            if (!l_videoFile.isEmpty()) {
            qDebug() << QUrl::fromLocalFile(l_videoFile);
                m_mediaPlayer->setMedia(QUrl::fromLocalFile(l_videoFile));
                m_mediaPlayer->play();
                m_mediaPlayer->pause();
            }
        }
        l_parser->deleteLater();
        l_dlg->deleteLater();
    });

    l_parser->start();
}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_btnBackward_clicked()
{
    m_mediaPlayer->setPosition(m_mediaPlayer->position() - 10 * 1000);
}

void MainWindow::on_btnPlay_clicked()
{
    if (m_mediaPlayer->state() == QMediaPlayer::PlayingState)
        m_mediaPlayer->pause();
    else
        m_mediaPlayer->play();
}

void MainWindow::on_btnFoward_clicked()
{
    m_mediaPlayer->setPosition(m_mediaPlayer->position() + 10 * 1000);
}

void MainWindow::showSubtitleItem()
{
    showSubtitleItem(m_subTitle.byTime(m_mediaPlayer->position()));
}

void MainWindow::showSubtitleItem(const SubtitleItem& a_item)
{
    ui->wdgVideo->setCurrentSubtitleItem(a_item);
}

void MainWindow::resizeEvent(QResizeEvent* /*event*/)
{
    showSubtitleItem();
}
