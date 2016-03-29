#include "musikplayer.h"
#include "ui_hsgui.h"

MusikPlayer::MusikPlayer(Ui::hsGui *_ui, QObject *parent) : QObject(parent), ui(_ui)
{
    m_initialized = false;
    m_GUIMode = 0;
    m_Input = 0;
    m_playMode = 0;

    m_port = 0;
    m_host = "";

    udpsocket = new QUdpSocket();
    udpsocket->bind(8001);

    m_netmanager = new QNetworkAccessManager(this);
    connect(m_netmanager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(sl_cover_fetch_done(QNetworkReply*)));

    m_volUpTimer = new QTimer;
    m_volUpTimer->setInterval(150);
    m_volDownTimer = new QTimer;
    m_volDownTimer->setInterval(150);
    connect(m_volUpTimer, SIGNAL(timeout()), this, SLOT(sendVolUp()));
    connect(m_volDownTimer, SIGNAL(timeout()), this, SLOT(sendVolDown()));

    ui->label_ErrorMP->setVisible(false);
    ui->label_ErrorMP->setStyleSheet("QLabel { color : red; }");

    connect(&networkthread, SIGNAL(error(int, QString)), this, SLOT(displayError(int,QString)));
    connect(&networkthread, SIGNAL(newReply(QString)), this, SLOT(displayReply(QString)));
    connect(udpsocket, SIGNAL(readyRead()), this, SLOT(pendingUDPDatagram()));

    initConnections();

    QByteArray bytearray = QString("[RendererBitteMelden!]").toUtf8();
    udpsocket->writeDatagram(bytearray, QHostAddress::Broadcast, 8002);

    //TODO m_host und m_port speichern und beim Neustart laden
    if (!m_host.isEmpty() && m_port > 0)
    {
        loadRadioList();
    }
}

MusikPlayer::~MusikPlayer()
{

}

void MusikPlayer::initConnections()
{
    QList<QToolButton*> btnlst;
    btnlst = parent()->findChildren<QToolButton*>();
    foreach (QToolButton *btn, btnlst) {
        if (btn->objectName().contains("toolButton_Radio"))
        {
            connect(btn, SIGNAL(clicked()), this, SLOT(btnRadioList_clicked()));
        }
    }
    foreach (QToolButton *btn, btnlst) {
        if (btn->objectName().contains("toolButton_Mode"))
        {
            connect(btn, SIGNAL(clicked()), this, SLOT(btnMode_clicked()));
        }
    }
    connect(ui->toolButton_Play, SIGNAL(clicked()), this, SLOT(btnPlay_clicked()));
    connect(ui->toolButton_Stop, SIGNAL(clicked()), this, SLOT(btnStop_clicked()));
    connect(ui->btnVolUp, SIGNAL(pressed()), m_volUpTimer, SLOT(start()));
    connect(ui->btnVolDown, SIGNAL(pressed()), m_volDownTimer, SLOT(start()));
    connect(ui->btnVolUp, SIGNAL(released()), m_volUpTimer, SLOT(stop()));
    connect(ui->btnVolDown, SIGNAL(released()), m_volDownTimer, SLOT(stop()));
    connect(ui->btnMute, SIGNAL(clicked()), this, SLOT(sendMute()));
}

void MusikPlayer::btnRadioList_clicked()
{
    int index;
    QToolButton *btn = (QToolButton*)sender();
    QString name = btn->objectName();
    index = name.right(1).toInt();
    networkthread.sendCommand(m_host, m_port, QString("set radio %1").arg(index - 1));
}

void MusikPlayer::btnPlay_clicked()
{
    if (m_playMode == 0)
        networkthread.sendCommand(m_host, m_port, QString("set pause"));
    else
        networkthread.sendCommand(m_host, m_port, QString("set play"));
}

void MusikPlayer::btnStop_clicked()
{
    networkthread.sendCommand(m_host, m_port, QString("set stop"));
}

void MusikPlayer::btnMode_clicked()
{
    int index;
    QToolButton *btn = (QToolButton*)sender();
    QString name = btn->objectName();
    if (name.contains("ModeStandby"))
        networkthread.sendCommand(m_host, m_port, "set standby");
    if (name.contains("ModeUPNP"))
        networkthread.sendCommand(m_host, m_port, "set mode upnp");
    if (name.contains("ModeRadio"))
        networkthread.sendCommand(m_host, m_port, "set mode radio");
    if (name.contains("ModeSPDIF"))
    {
        if (m_GUIMode == 3)
        {
            if (m_Input == 3)
                index = 0;
            else
                index = m_Input + 1;
            networkthread.sendCommand(m_host, m_port, QString("set spdifinput %1").arg(index));
        }
        else
        {
            networkthread.sendCommand(m_host, m_port, "set mode spdif");
        }
    }
}

void MusikPlayer::pendingUDPDatagram()
{
    while (udpsocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress hostSender;
        quint16 portSender;
        datagram.resize(udpsocket->pendingDatagramSize());
        udpsocket->readDatagram(datagram.data(), datagram.size(), &hostSender, &portSender);
        QString str = QString(datagram);
        if (str.contains("[RaspiDAC]"))
        {
            m_host = hostSender.toString();
            applyStatus(str);
        }

    }
}

void MusikPlayer::setModeButtons()
{
    ui->toolButton_ModeStandby->setStyleSheet("font-weight: normal");
    ui->toolButton_ModeRadio->setStyleSheet("font-weight: normal");
    ui->toolButton_ModeUPNP->setStyleSheet("font-weight: normal");
    ui->toolButton_ModeSPDIF->setStyleSheet("font-weight: normal");

    if (m_Input < inputList.size())
        ui->toolButton_ModeSPDIF->setText(QString("Input %1 (%2)").arg(m_Input + 1).arg(inputList.at(m_Input)));
    else
        ui->toolButton_ModeSPDIF->setText(QString("Input %1").arg(m_Input + 1));

    switch (m_GUIMode) {
    case 0:
        ui->toolButton_ModeStandby->setStyleSheet("font-weight: bold");
        break;
    case 1:
        ui->toolButton_ModeUPNP->setStyleSheet("font-weight: bold");
        break;
    case 2:
        ui->toolButton_ModeRadio->setStyleSheet("font-weight: bold");
        break;
    case 3:
        ui->toolButton_ModeSPDIF->setStyleSheet("font-weight: bold");
        break;
    }
}

void MusikPlayer::applyStatus(QString &str, bool getMD)
{
    qDebug() << "MusikPlayer::applyStatus: " << str;
    if (str.contains("[RaspiDAC]"))
    {
        str.remove("[RaspiDAC]");
        QStringList lst = str.split(";");
        //Port für TCP-Connection
        m_port = lst.at(5).toInt();
        //GUI Mode (Standby, UPNP, Radio, SPDIF)
        m_GUIMode = lst.at(1).toInt();
        //Play Mode
        m_playMode = lst.at(2).toInt();
        if (lst.at(2).toInt() == 0) //PLAY
        {
            ui->toolButton_Play->setIcon(QIcon(QString(":/new/musikplayer/resource/musikplayer/pause.png")));
            ui->label_PlayMode->setPixmap(QPixmap(QString(":/new/musikplayer/resource/musikplayer/play.png")));
        }
        if (lst.at(2).toInt() == 1) //PAUSE
        {
            ui->toolButton_Play->setIcon(QIcon(QString(":/new/musikplayer/resource/musikplayer/play.png")));
            ui->label_PlayMode->setPixmap(QPixmap(QString(":/new/musikplayer/resource/musikplayer/pause.png")));
        }
        if (lst.at(2).toInt() == 2) //STOP
        {
            ui->label_PlayMode->setPixmap(QPixmap(QString(":/new/musikplayer/resource/musikplayer/stop.png")));
            ui->toolButton_Play->setIcon(QIcon(QString(":/new/musikplayer/resource/musikplayer/play.png")));
        }
        //SPDIF Input
        m_Input = lst.at(3).toInt();

        if (lst.at(4).contains(("true")))
        {
            qDebug() << "getRadioList";
            loadRadioList();
        }

        if (lst.at(0).contains("true") && getMD && !lst.at(4).contains("true"))
        {
            qDebug() << "get MetaData";
            //MetaData abfragen
            getMetaData();
        }
        setModeButtons();
    }
}


void MusikPlayer::displayError(int socketError, const QString &message)
{
    ui->label_ErrorMP->setVisible(true);
    switch (socketError) {
    case QAbstractSocket::HostNotFoundError:
        ui->label_ErrorMP->setText("Keine Verbindung zum RaspiDAC. (Host nicht erreichbar.)");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        ui->label_ErrorMP->setText("RaspiDAC nicht erreichbar. (Connection refused.)");
        break;
    default:
        ui->label_ErrorMP->setText(QString("Ein Fehler ist aufgetreten: %1.").arg(message));
    }
}

void MusikPlayer::displayReply(const QString &message)
{
    ui->label_ErrorMP->setVisible(false);
    if(message.contains("[radioList]"))
    {
        QFont fnt;
        QString list = message;
        list.remove("[radioList]");
        QStringList radioList = list.split(";");

        QList<QToolButton*> btnlst;
        btnlst = parent()->findChildren<QToolButton*>();
        foreach (QToolButton *btn, btnlst) {
            if (btn->objectName().contains("toolButton_Radio"))
            {
                fnt = btn->font();
                btn->setText("...");
                btn->setEnabled(false);
            }
        }
        QFontMetrics fntmetr = QFontMetrics(fnt);

        foreach (QToolButton *btn, btnlst) {
            if (btn->objectName().contains("toolButton_Radio"))
            {
                QString btn_name = btn->objectName();
                QString btn_text = radioList.at(btn_name.right(1).toInt() - 1);
                if (btn_name.right(1).toInt() < radioList.size())
                {
                    while (fntmetr.width(btn_text) >= (btn->width() - 10))
                    {
                        btn_text.remove(btn_text.length() - 1, 1);
                    }
                    btn->setText(btn_text);
                    btn->setEnabled(true);
                }
            }

        }
    }
    else if (message.contains("[MetaData]"))
    {
        bool vis = true;
        QString list = message;
        list.remove("[MetaData]");
        QStringList mdList = list.split(";");
        ui->label_Album->setText(mdList.at(0));
        ui->label_Artist->setText(mdList.at(1));
        ui->label_Titel->setText(mdList.at(2));

        if (m_GUIMode == 2)
            vis = false;

        ui->label_AlbumLabel->setVisible(vis);
        ui->label_ArtistLabel->setVisible(vis);
        ui->label_TitelLabel->setVisible(vis);

        if (!mdList.at(3).isEmpty()) {
            fetch_cover(mdList.at(3));
        } else {
            no_cover_available();
        }
        if (!m_initialized)
            getInputList();
        m_initialized = true;
    }
    else if (message.contains("[RaspiDAC]"))
    {
        QString list = message;
        applyStatus(list, false);
    }
    else if (message.contains("[inputList]"))
    {
        QString list = message;
        list.remove("[inputList]");
        inputList = list.split(";");
        setModeButtons();
    }
    if (!m_initialized)
    {
        getMetaData();
    }
}

void MusikPlayer::loadRadioList()
{
    networkthread.sendCommand(m_host, m_port, "get radiolist");
}

void MusikPlayer::getMetaData()
{
    networkthread.sendCommand(m_host, m_port, "get metadata");
}

void MusikPlayer::getStatus()
{
    networkthread.sendCommand(m_host, m_port, "get status");
}

void MusikPlayer::getInputList()
{
    networkthread.sendCommand(m_host, m_port, "get inputlist");
}

void MusikPlayer::fetch_cover(const QString& URI)
{
    if (!m_netmanager) {
        return;
    }
    m_netmanager->get(QNetworkRequest(QUrl(URI)));
}

void MusikPlayer::sl_cover_fetch_done(QNetworkReply* reply)
{
    qDebug() << "GUI_Player::sl_cover_fetch_done";
    if (reply->error() != QNetworkReply::NoError) {
        no_cover_available();
        return;
    }

    QString smime =
        reply->header(QNetworkRequest::ContentTypeHeader).toString();
    int scolon;
    if ((scolon = smime.indexOf(";")) > 0) {
        smime = smime.left(scolon);
    }
    QByteArray imtype;
    if (!smime.compare("image/png", Qt::CaseInsensitive)) {
        imtype = "PNG";
    } else     if (!smime.compare("image/jpeg", Qt::CaseInsensitive)) {
        imtype = "JPG";
    } else     if (!smime.compare("image/gif", Qt::CaseInsensitive)) {
        imtype = "GIF";
    } else {
        qDebug() << "GUI_Player::sl_cover_fetch_done: unsupported mime type: "<<
            smime;
        no_cover_available();
        return;
    }
    QImageReader reader((QIODevice*)reply, imtype);
    reader.setAutoDetectImageFormat(false);

    QImage image;
    if (!reader.read(&image)) {
        qDebug() << "GUI_Player::sl_vover_fetch_done: image read failed " <<
            reader.errorString();
        no_cover_available();
        return;
    }

    QPixmap pixmap;
    //pixmap.convertFromImage(image);
    pixmap = QPixmap::fromImage(image);
    ui->label_AlbumArt->setPixmap(pixmap);
//    ui->albumCover->setIcon(QIcon(pixmap));
//    ui->albumCover->repaint();
    reply->deleteLater();
}

void MusikPlayer::no_cover_available()
{
    ui->label_AlbumArt->setPixmap(QPixmap(QString(":/new/musikplayer/resource/musikplayer/logo.png")));
}

void MusikPlayer::sendVolUp()
{
    networkthread.sendCommand(m_host, m_port, "set pm8000 vol+");
}

void MusikPlayer::sendVolDown()
{
    networkthread.sendCommand(m_host, m_port, "set pm8000 vol-");
}

void MusikPlayer::sendMute()
{
    networkthread.sendCommand(m_host, m_port, "set pm8000 mute");
}
