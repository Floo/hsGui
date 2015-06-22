#include "allgemeineeinstellungen.h"
#include "ui_allgemeineeinstellungen.h"

AllgemeineEinstellungen::AllgemeineEinstellungen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllgemeineEinstellungen)
{
    ui->setupUi(this);
}

AllgemeineEinstellungen::~AllgemeineEinstellungen()
{
    delete ui;
}

void AllgemeineEinstellungen::setMouseZeiger(bool mousezeiger)
{
    mousezeiger?setCursor(Qt::ArrowCursor):setCursor(Qt::BlankCursor);
}

void AllgemeineEinstellungen::setFullScreen(bool fullscreen)
{
    fullscreen?setWindowState(Qt::WindowFullScreen):setWindowState(Qt::WindowActive);
}

void AllgemeineEinstellungen::on_pushButton_restartHaussteuerung_clicked()
{
    QProcess::startDetached("/home/florian/Desktop/restart_hs.sh");
}

void AllgemeineEinstellungen::setLogText(QString text)
{
    ui->textBrowser_LogFile->setText(text);
}

void AllgemeineEinstellungen::on_btnSliderDown_clicked()
{
    QScrollBar *sb = ui->textBrowser_LogFile->verticalScrollBar();
    sb->setValue(sb->value() - sb->singleStep());
}

void AllgemeineEinstellungen::on_btnSliderUp_clicked()
{
    QScrollBar *sb = ui->textBrowser_LogFile->verticalScrollBar();
    sb->setValue(sb->value() + sb->singleStep());
}

void AllgemeineEinstellungen::on_btnSliderDownMax_clicked()
{
    QScrollBar *sb = ui->textBrowser_LogFile->verticalScrollBar();
    sb->setValue(sb->minimum());
}

void AllgemeineEinstellungen::on_btnSliderUpMax_clicked()
{
    QScrollBar *sb = ui->textBrowser_LogFile->verticalScrollBar();
    sb->setValue(sb->maximum());
}
