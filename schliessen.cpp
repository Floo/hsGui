#include "schliessen.h"
#include "ui_schliessen.h"

Schliessen::Schliessen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Schliessen)
{
    ui->setupUi(this);
}

Schliessen::~Schliessen()
{
    delete ui;
}

void Schliessen::setMouseZeiger(bool mousezeiger)
{
    mousezeiger?setCursor(Qt::ArrowCursor):setCursor(Qt::BlankCursor);
}

void Schliessen::setFullScreen(bool fullscreen)
{
    FullScreen = fullscreen;
    fullscreen?setWindowState(Qt::WindowFullScreen):setWindowState(Qt::WindowActive);
    //FullScreen = true;
}

void Schliessen::on_pushButton_LichtTouchBeenden_clicked()
{
    lichtOrientSzeneAus();
}

void Schliessen::on_pushButton_TouchBeenden_clicked()
{
    if(FullScreen)
    {
        QProcess::startDetached("/home/florian/Desktop/show_panel.sh shutdown");
    }
    qApp->exit();
}

void Schliessen::on_pushButton_Close_clicked()
{
    if(FullScreen)
    {
        QProcess::startDetached("/home/florian/Desktop/show_panel.sh");
    }
    qApp->exit();
}

void Schliessen::lichtOrientSzeneAus()
{
    connect(tcpClient, SIGNAL(bytesWritten(qint64)), this, SLOT(closeApp(qint64)));
    QString sendString = QString("cmd=%1 value=Aus\n").arg(CMD_SZENE);
    tcpClient->write(sendString.toLatin1());
    sendString = QString("cmd=%1 value=AUS\n").arg(CMD_FLOOR);
    tcpClient->write(sendString.toLatin1());
}

void Schliessen::closeApp(qint64)
{
    if (FullScreen)
    {
        QProcess::startDetached("/home/florian/Desktop/show_panel.sh shutdown");
    }
    qApp->exit();
}





