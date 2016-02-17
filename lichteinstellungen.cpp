#include "lichteinstellungen.h"
#include "ui_lichteinstellungen.h"

LichtEinstellungen::LichtEinstellungen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LichtEinstellungen)
{
    ui->setupUi(this);
    connect(ui->lichtResetTimerToolButton_0, SIGNAL(clicked()), this, SLOT(sendTimerReset()));
    connect(ui->lichtResetTimerToolButton_1, SIGNAL(clicked()), this, SLOT(sendTimerReset()));
    connect(ui->lichtResetTimerToolButton_2, SIGNAL(clicked()), this, SLOT(sendTimerReset()));
    connect(ui->lichtResetTimerToolButton_3, SIGNAL(clicked()), this, SLOT(sendTimerReset()));
    connect(ui->lichtResetTimerToolButton_4, SIGNAL(clicked()), this, SLOT(sendTimerReset()));
    connect(ui->lichtResetTimerToolButton_5, SIGNAL(clicked()), this, SLOT(sendTimerReset()));
    connect(ui->lichtResetTimerToolButton_6, SIGNAL(clicked()), this, SLOT(sendTimerReset()));
    connect(ui->lichtResetTimerToolButton_7, SIGNAL(clicked()), this, SLOT(sendTimerReset()));

    //QRect scr = QApplication::desktop()->screenGeometry();
    //move( scr.center() - rect().center() );
    //QRect aktiveArea = ui->frame->geometry();
    //setMask(QRegion(aktiveArea));
    initLichtEinstellungen();
}

LichtEinstellungen::~LichtEinstellungen()
{
    delete ui;
}

void LichtEinstellungen::sendTimerReset()
{
    int index;
    QToolButton *toolButton = (QToolButton*)sender();
    QString name = toolButton->objectName();
    name.remove(0, 26);
    index = name.left(1).toInt();
    QString sendString = QString("cmd=%1 value=%2\n").arg(CMD_LICHT_RESET_TIMER).arg(lichtDevices->at(index));
    tcpClient->write(sendString.toLatin1());
}

void LichtEinstellungen::initLichtEinstellungen()
{
    lichtResetTimerToolButton << ui->lichtResetTimerToolButton_0;
    lichtResetTimerToolButton << ui->lichtResetTimerToolButton_1;
    lichtResetTimerToolButton << ui->lichtResetTimerToolButton_2;
    lichtResetTimerToolButton << ui->lichtResetTimerToolButton_3;
    lichtResetTimerToolButton << ui->lichtResetTimerToolButton_4;
    lichtResetTimerToolButton << ui->lichtResetTimerToolButton_5;
    lichtResetTimerToolButton << ui->lichtResetTimerToolButton_6;
    lichtResetTimerToolButton << ui->lichtResetTimerToolButton_7;
}


void LichtEinstellungen::updateLichtEinstellungen(QList<QString> *pLichtDevices, QList<QString> *pLichtDeviceNamen)
{
    int i;
    lichtDevices = pLichtDevices;
    for(i = 0; i <= 7; i++)
    {
        lichtResetTimerToolButton[i]->setText(pLichtDeviceNamen->at(i));
        lichtResetTimerToolButton[i]->setEnabled(pLichtDevices->at(i) != "");
    }
}

void LichtEinstellungen::setTcpClient(QTcpSocket *pTcpClient)
{
    tcpClient = pTcpClient;
}

void LichtEinstellungen::setMouseZeiger(bool mousezeiger)
{
    mousezeiger?setCursor(Qt::ArrowCursor):setCursor(Qt::BlankCursor);
}

void LichtEinstellungen::setFullScreen(bool fullscreen)
{
    fullscreen?setWindowState(Qt::WindowFullScreen):setWindowState(Qt::WindowActive);
}
