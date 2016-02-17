#include "hsgui.h"
#include "ui_hsgui.h"
#include "musikplayer.h"

hsGui::hsGui(QString *ipTcpConnection, int portTcpConnection, bool fullscreen, bool mousezeiger, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::hsGui)
{
    tcpClient = new QTcpSocket(this);
    orient = false;
    ipTCPConnection = ipTcpConnection;
    portTCPConnection = portTcpConnection;
    ui->setupUi(this);
    musikplayer = new MusikPlayer(ui, this);

    networkAccess_forcast = new QNetworkAccessManager(this);
    networkAccess_regenradar = new QNetworkAccessManager(this);
    networkAccess_wetterplot = new QNetworkAccessManager(this);

    je = new JalousieEinstellungen();
    je->setWindowFlags(Qt::FramelessWindowHint);
    je->setFullScreen(fullscreen);
    je->setMouseZeiger(mousezeiger);
    le = new LichtEinstellungen();
    le->setWindowFlags(Qt::FramelessWindowHint);
    le->setFullScreen(fullscreen);
    le->setMouseZeiger(mousezeiger);
    le->setTcpClient(tcpClient);
    sch = new Schliessen();
    sch->setWindowFlags(Qt::FramelessWindowHint);
    sch->setFullScreen(fullscreen);
    sch->setMouseZeiger(mousezeiger);
    sch->setTcpClient(tcpClient);
    ae = new AllgemeineEinstellungen();
    ae->setWindowFlags(Qt::FramelessWindowHint);
    ae->setFullScreen(fullscreen);
    ae->setMouseZeiger(mousezeiger);

    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readFromTCPServer()));
    connect(tcpClient, SIGNAL(connected()), this, SLOT(connectedTCPServer()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(disconnectedTCPServer()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorTCPServer(QAbstractSocket::SocketError)));

    connect(ui->toolButton_IMPAUF_0, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_IMPAUF_1, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_IMPAUF_2, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_IMPAUF_3, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_IMPAUF_ALL, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AUF_0, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AUF_1, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AUF_2, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AUF_3, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AUF_ALL, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_STP_0, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_STP_1, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_STP_2, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_STP_3, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_STP_ALL, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AB_0, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AB_1, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AB_2, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AB_3, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_AB_ALL, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_IMPAB_0, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_IMPAB_1, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_IMPAB_2, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_IMPAB_3, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_IMPAB_ALL, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_SUN_0, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_SUN_1, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_SUN_2, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_SUN_3, SIGNAL(clicked()), this, SLOT(sendJal()));
    connect(ui->toolButton_SUN_ALL, SIGNAL(clicked()), this, SLOT(sendJal()));

    connect(ui->toolButton_JalEinstellungen, SIGNAL(clicked()), this, SLOT(showJalEinstellungen()));
    connect(ui->toolButton_LichtEinstellungen, SIGNAL(clicked()), this, SLOT(showLichtEinstellungen()));
    connect(ui->toolButton_Close, SIGNAL(clicked()), this, SLOT(showSchliessen()));
    connect(ui->toolButton_Setup, SIGNAL(clicked()), this, SLOT(showAllgemeineEinstellungen()));
    connect(je, SIGNAL(finished(int)), this, SLOT(closeJalEinstellungen(int)));
    connect(le, SIGNAL(finished(int)), this, SLOT(closeLichtEinstellungen(int)));
    connect(sch, SIGNAL(finished(int)), this, SLOT(closeSchliessen(int)));
    connect(ae, SIGNAL(finished(int)), this, SLOT(closeAllgemeineEinstellungen(int)));
    connect(networkAccess_forcast, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkRequestFinished_forcast(QNetworkReply*)));
    connect(networkAccess_regenradar, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkRequestFinished_regenradar(QNetworkReply*)));
    connect(networkAccess_wetterplot, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkRequestFinished_wetterplot(QNetworkReply*)));

    connect(ui->toolButton_Licht_0_AUS, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_0_AN, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_0_DOWN, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_0_DOWN, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_0_UP, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_0_UP, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_1_AUS, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_1_AN, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_1_DOWN, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_1_DOWN, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_1_UP, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_1_UP, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_2_AUS, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_2_AN, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_2_DOWN, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_2_DOWN, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_2_UP, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_2_UP, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));;
    connect(ui->toolButton_Licht_3_AUS, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_3_AN, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_3_DOWN, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_3_DOWN, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_3_UP, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_3_UP, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_4_AUS, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_4_AN, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_4_DOWN, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_4_DOWN, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_4_UP, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_4_UP, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_5_AUS, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_5_AN, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_5_DOWN, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_5_DOWN, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_5_UP, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_5_UP, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_6_AUS, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_6_AN, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_6_DOWN, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_6_DOWN, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_6_UP, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_6_UP, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_7_AUS, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_7_AN, SIGNAL(clicked()), this, SLOT(sendLicht()));
    connect(ui->toolButton_Licht_7_DOWN, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_7_DOWN, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Licht_7_UP, SIGNAL(pressed()), this, SLOT(sendLichtDimmPressed()));
    connect(ui->toolButton_Licht_7_UP, SIGNAL(released()), this, SLOT(sendLichtDimmReleased()));
    connect(ui->toolButton_Szene_0, SIGNAL(clicked()), this, SLOT(sendSzene()));
    connect(ui->toolButton_Szene_1, SIGNAL(clicked()), this, SLOT(sendSzene()));
    connect(ui->toolButton_Szene_2, SIGNAL(clicked()), this, SLOT(sendSzene()));
    connect(ui->toolButton_Szene_3, SIGNAL(clicked()), this, SLOT(sendSzene()));
    connect(ui->toolButton_Szene_4, SIGNAL(clicked()), this, SLOT(sendSzene()));
    connect(ui->toolButton_Szene_5, SIGNAL(clicked()), this, SLOT(sendSzene()));
    connect(ui->toolButton_Licht_Orient, SIGNAL(clicked()), this, SLOT(sendOrient()));
    connect(ui->pushButton_Open_1, SIGNAL(clicked()), this, SLOT(pushButton_Terrasse()));
    connect(ui->pushButton_Open_2, SIGNAL(clicked()), this, SLOT(pushButton_Terrasse()));
    connect(ui->pushButton_Close_1, SIGNAL(clicked()), this, SLOT(pushButton_Terrasse()));
    connect(ui->pushButton_Close_2, SIGNAL(clicked()), this, SLOT(pushButton_Terrasse()));
    connect(ui->pushButton_StartTimer_1, SIGNAL(clicked()), this, SLOT(pushButton_Terrasse_Timer()));
    connect(ui->pushButton_StartTimer_2, SIGNAL(clicked()), this, SLOT(pushButton_Terrasse_Timer()));

    quarter = 0;
    quarterTry = 0;
    initConfWidgets();
    updateVorhersage();

    regenRadarGif = NULL;
    wetterPlotPNG = NULL;

    timerRegenRadar = new QTimer(this);
    timerForecast = new QTimer(this);
    timerReconnectTCP = new QTimer(this);
    connect(timerRegenRadar, SIGNAL(timeout()), this, SLOT(updateRegenRadar()));
    connect(timerForecast, SIGNAL(timeout()), this, SLOT(updateVorhersage()));
    connect(timerReconnectTCP, SIGNAL(timeout()), this, SLOT(tryReconnectToTCPServer()));

    timerRegenRadar->start(60000);
    timerForecast->start(15 * 60000);
    timerReconnectTCP->setSingleShot(true);
    timerReconnectTCP->setInterval(10000);
    hsconfFile = new QTemporaryFile();
    je->setHsconfFile(hsconfFile);
    hslightFile = new QTemporaryFile();

    tcpClient->connectToHost(*ipTcpConnection, portTcpConnection);
    initLicht();
}

hsGui::~hsGui()
{
    tcpClient->close();
    delete ui;
}

void hsGui::initTCPConnection()
{
    setSharedMemoryReceiveOnServer();
    requestConfigFile(CONFIG_HS);
    requestConfigFile(CONFIG_LIGHT);
}

void hsGui::tryReconnectToTCPServer()
{
    tcpClient->connectToHost(*ipTCPConnection, portTCPConnection);
}

void hsGui::disconnectedTCPServer()
{
    ui->label_ConnectedGUI->setEnabled(false);
    ui->label_ConnectedMain->setEnabled(false);
    ui->label_ConnectedHWR->setEnabled(false);
    ui->label_ConnectedTerrasse->setEnabled(false);
    tryReconnectToTCPServer();
}

void hsGui::errorTCPServer(QAbstractSocket::SocketError error)
{
    if(error == QAbstractSocket::ConnectionRefusedError || error == QAbstractSocket::NetworkError)
    {
        timerReconnectTCP->start();
    }
    else if(error == QAbstractSocket::SocketTimeoutError)
    {
        if(tcpClient->state() == QAbstractSocket::UnconnectedState)
        {
            tcpClient->connectToHost(*ipTCPConnection, portTCPConnection);
        }
    }
}

void hsGui::sendOrient()
{
    QString sendString;
    orient = !orient;
    setOrientIcon();
    if(orient)
    {
        sendString = QString("cmd=%1 value=AN\n").arg(CMD_FLOOR);
    }else{
        sendString = QString("cmd=%1 value=AUS\n").arg(CMD_FLOOR);
    }
    tcpClient->write(sendString.toLatin1());
}

void hsGui::setOrientIcon()
{
    QString buttonSymbolResource;
    if(orient)
    {
        buttonSymbolResource = QString(":/new/licht/resource/Licht_An.png");
    }else{
        buttonSymbolResource = QString(":/new/licht/resource/Licht_Aus.png");
    }
    ui->toolButton_Licht_Orient->setIcon(QIcon(buttonSymbolResource));
}

void hsGui::sendJal()
{
    QToolButton *toolButton = (QToolButton*)sender();
    QString value = toolButton->objectName();
    value.remove(0, 11);
    QString sendString = QString("cmd=%1 value=%2\n").arg(CMD_JAL).arg(value);
    tcpClient->write(sendString.toLatin1());
}

void hsGui::sendLicht()
{
    int index, cmd;
    QToolButton *toolButton = (QToolButton*)sender();
    QString name = toolButton->objectName();
    name.remove(0, 17);
    index = name.left(1).toInt();
    name.remove(0, 2);
    if(name == "AN") {cmd = FS20_AN;}
    if(name == "AUS") {cmd = FS20_AUS;}
    QString sendString = QString("cmd=%1 value=%2:%3\n").arg(CMD_LICHT).arg(lichtDevices[index]).arg(cmd);
    tcpClient->write(sendString.toLatin1());
}

void hsGui::sendLichtDimmPressed()
{
    int index, cmd;
    QToolButton *toolButton = (QToolButton*)sender();
    QString name = toolButton->objectName();
    name.remove(0, 17);
    index = name.left(1).toInt();
    name.remove(0, 2);
    if(name == "DOWN") {cmd = DIMM_DOWN_START;}
    if(name == "UP") {cmd = DIMM_UP_START;}
    QString sendString = QString("cmd=%1 value=%2:%3\n").arg(CMD_DIMM).arg(lichtDevices[index]).arg(cmd);
    tcpClient->write(sendString.toLatin1());
}

void hsGui::sendLichtDimmReleased()
{
    int index, cmd;
    QToolButton *toolButton = (QToolButton*)sender();
    QString name = toolButton->objectName();
    name.remove(0, 17);
    index = name.left(1).toInt();
    name.remove(0, 2);
    if(name == "DOWN") {cmd = DIMM_DOWN_STOP;}
    if(name == "UP") {cmd = DIMM_UP_STOP;}
    QString sendString = QString("cmd=%1 value=%2:%3\n").arg(CMD_DIMM).arg(lichtDevices[index]).arg(cmd);
    tcpClient->write(sendString.toLatin1());
}

void hsGui::sendSzene()
{
    int index;
    QToolButton *toolButton = (QToolButton*)sender();
    QString name = toolButton->objectName();
    name.remove(0, 17);
    index = name.toInt();
    QString sendString = QString("cmd=%1 value=%2\n").arg(CMD_SZENE).arg(szenenNamen[index]);
    tcpClient->write(sendString.toLatin1());
}

void hsGui::initLicht()
{
    int i;
    //alle Buttons und Beschriftungen in Listen schreiben
    lichtAusToolButton << ui->toolButton_Licht_0_AUS;
    lichtAusToolButton << ui->toolButton_Licht_1_AUS;
    lichtAusToolButton << ui->toolButton_Licht_2_AUS;
    lichtAusToolButton << ui->toolButton_Licht_3_AUS;
    lichtAusToolButton << ui->toolButton_Licht_4_AUS;
    lichtAusToolButton << ui->toolButton_Licht_5_AUS;
    lichtAusToolButton << ui->toolButton_Licht_6_AUS;
    lichtAusToolButton << ui->toolButton_Licht_7_AUS;
    lichtAnToolButton << ui->toolButton_Licht_0_AN;
    lichtAnToolButton << ui->toolButton_Licht_1_AN;
    lichtAnToolButton << ui->toolButton_Licht_2_AN;
    lichtAnToolButton << ui->toolButton_Licht_3_AN;
    lichtAnToolButton << ui->toolButton_Licht_4_AN;
    lichtAnToolButton << ui->toolButton_Licht_5_AN;
    lichtAnToolButton << ui->toolButton_Licht_6_AN;
    lichtAnToolButton << ui->toolButton_Licht_7_AN;
    lichtDownToolButton << ui->toolButton_Licht_0_DOWN;
    lichtDownToolButton << ui->toolButton_Licht_1_DOWN;
    lichtDownToolButton << ui->toolButton_Licht_2_DOWN;
    lichtDownToolButton << ui->toolButton_Licht_3_DOWN;
    lichtDownToolButton << ui->toolButton_Licht_4_DOWN;
    lichtDownToolButton << ui->toolButton_Licht_5_DOWN;
    lichtDownToolButton << ui->toolButton_Licht_6_DOWN;
    lichtDownToolButton << ui->toolButton_Licht_7_DOWN;
    lichtUpToolButton << ui->toolButton_Licht_0_UP;
    lichtUpToolButton << ui->toolButton_Licht_1_UP;
    lichtUpToolButton << ui->toolButton_Licht_2_UP;
    lichtUpToolButton << ui->toolButton_Licht_3_UP;
    lichtUpToolButton << ui->toolButton_Licht_4_UP;
    lichtUpToolButton << ui->toolButton_Licht_5_UP;
    lichtUpToolButton << ui->toolButton_Licht_6_UP;
    lichtUpToolButton << ui->toolButton_Licht_7_UP;
    lichtLabel << ui->label_Licht_0;
    lichtLabel << ui->label_Licht_1;
    lichtLabel << ui->label_Licht_2;
    lichtLabel << ui->label_Licht_3;
    lichtLabel << ui->label_Licht_4;
    lichtLabel << ui->label_Licht_5;
    lichtLabel << ui->label_Licht_6;
    lichtLabel << ui->label_Licht_7;
    szenenToolButton << ui->toolButton_Szene_0;
    szenenToolButton << ui->toolButton_Szene_1;
    szenenToolButton << ui->toolButton_Szene_2;
    szenenToolButton << ui->toolButton_Szene_3;
    szenenToolButton << ui->toolButton_Szene_4;
    szenenToolButton << ui->toolButton_Szene_5;
    //...und initialisieren
    for(i = 0; i <= 7; i++)
    {
        lichtDevices << QString("");
        lichtDeviceNamen << QString("...");
        lichtLabel[i]->setText(lichtDeviceNamen[i]);
        lichtAusToolButton[i]->setEnabled(false);
        lichtAnToolButton[i]->setEnabled(false);
        lichtDownToolButton[i]->setEnabled(false);
        lichtUpToolButton[i]->setEnabled(false);
        lichtLabel[i]->setEnabled(false);
    }
    for(i = 0; i <= 5; i++)
    {
        szenenNamen << QString("...");
        szenenToolButton[i]->setText(szenenNamen[i]);
        szenenToolButton[i]->setEnabled(false);
    }
}

void hsGui::updateLicht()
{
    int i;
    QByteArray hslightByteArray;
    QString devices, scenes;
    QByteArray section;
    QStringList deviceList, sceneList;
    dictionary *hslight;

    hslightByteArray = hslightFile->fileName().toLatin1();
    hslight = iniparser_load(hslightByteArray.data());
    devices = QString(iniparser_getstring(hslight, "system:devices", NULL));
    scenes = QString(iniparser_getstring(hslight, "system:scenes", NULL));
    deviceList = devices.split(QChar(','), QString::SkipEmptyParts);
    sceneList = scenes.split(QChar(','), QString::SkipEmptyParts);

    for(i = 0; i < deviceList.size(); i++)
    {
        lichtDevices[i] = deviceList[i].trimmed();
        section = QByteArray(QString("deviceName:%1").arg(deviceList.at(i).trimmed()).toLatin1());
        lichtDeviceNamen[i] = QString(iniparser_getstring(hslight, section.data(), (char*)"..."));
    }

    for(i = 0; i < sceneList.size(); i++)
    {
        szenenNamen[i] = sceneList[i].trimmed();
    }

    iniparser_freedict(hslight);

    for(i = 0; i <= 7; i++)
    {
        lichtLabel[i]->setText(lichtDeviceNamen[i]);
        if(lichtDevices[i] != "")
        {
            lichtAusToolButton[i]->setEnabled(true);
            lichtAnToolButton[i]->setEnabled(true);
            lichtDownToolButton[i]->setEnabled(true);
            lichtUpToolButton[i]->setEnabled(true);
            lichtLabel[i]->setEnabled(true);
        }

    }
    for(i = 0; i <= 5; i++)
    {
        szenenToolButton[i]->setText(szenenNamen[i]);
        if(szenenNamen[i] != "...")
        {
            szenenToolButton[i]->setEnabled(true);
        }
    }
    le->updateLichtEinstellungen(&lichtDevices, &lichtDeviceNamen);
}


void hsGui::updateRegenRadar()
{
    if(!strUrlRegenRadar.isEmpty())
    {
        QUrl urlRegenRadar = QUrl(strUrlRegenRadar);
        networkAccess_regenradar->get(QNetworkRequest(urlRegenRadar));
    }
}

//Berlin: http://api.wetter.com/forecast/weather/city/DE0001020/project/hsgui/cs/d69f66587b4d2171cd1176b2f1afba8d
//Tempelhof: http://api.wetter.com/forecast/weather/city/DE0001020466/project/hsgui/cs/79a5de1144f1ea5b0c62e0edf753f1c7

void hsGui::updateVorhersage()
{
    networkAccess_forcast->get(QNetworkRequest(QUrl("http://api.wetter.com/forecast/weather/city/DE0001020466/project/hsgui/cs/79a5de1144f1ea5b0c62e0edf753f1c7")));
}

void hsGui::networkRequestFinished_forcast(QNetworkReply *reply)
{
    QXmlStreamReader xmlReader(reply);
    xmlReader.readNext();
    while(!xmlReader.atEnd())
    {
        if(xmlReader.isStartElement() && (xmlReader.name() == "forecast")){
            countDate = 0;
            xmlReadForecast(&xmlReader);
        }
        xmlReader.readNext();
    }
    reply->deleteLater();
}

void hsGui::networkRequestFinished_regenradar(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        if (reply->bytesAvailable() < 1000)
        {
             return;
        }
        if (regenRadarGif != NULL)
        {
            delete(regenRadarGif);
        }
        regenRadarGif = new QMovie();
        regenRadarGif->setDevice(reply);
        regenRadarGif->setCacheMode(QMovie::CacheAll);
        ui->labelRegenRadar->setMovie(regenRadarGif);
        regenRadarGif->start();
    }
}

void hsGui::networkRequestFinished_wetterplot(QNetworkReply *reply)
{
    if ( wetterPlotPNG != NULL)
    {
        delete(wetterPlotPNG);
    }
    wetterPlotPNG = new QPixmap();
    wetterPlotPNG->loadFromData(reply->readAll());
    ui->labelWetterPlot->setPixmap(*wetterPlotPNG);
}


void hsGui::xmlReadForecast(QXmlStreamReader *xmlReader)
{
    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement()){
            if (xmlReader->name() == "forecast"){
                xmlReader->readNext();
                break;
            }else {
                xmlReader->readNext();
            }
        }
        if (xmlReader->isStartElement()){
            if (xmlReader->name() == "date"){
                xmlReadForecastDate(xmlReader);
            }
        }else{
            xmlReader->readNext();
        }
    }
}

void hsGui::xmlReadForecastDate(QXmlStreamReader *xmlReader)
{
    uint time;
    int minTemp, maxTemp, regenWahrscheinlichkeit, wetterSymbol;
    QString wetterSymbolResource;
    xmlReader->readNext();
    while (!xmlReader->atEnd())
    {
        if (xmlReader->isEndElement())
        {
            if (xmlReader->name() == "date"){
                //Darstellung aktualisieren
                QString foo = ui->label_Temp_1->text();
                QLabel *labelTag[] = {ui->label_Tag_1, ui->label_Tag_2, ui->label_Tag_3};
                QLabel *labelTemp[] = {ui->label_Temp_1, ui->label_Temp_2, ui->label_Temp_3};
                QLabel *labelWetter[] = {ui->label_Wetter_1, ui->label_Wetter_2, ui->label_Wetter_3};
                if (wetterSymbol > 9) wetterSymbol = wetterSymbol/10;
                wetterSymbolResource = QString(":/wetter.com/png/resource/wetter.com/d_%1_b.png").arg(wetterSymbol);
                labelWetter[countDate]->setPixmap(QPixmap(wetterSymbolResource));
                labelTemp[countDate]->setText(QString("%1/%2%3C").arg(maxTemp).arg(minTemp).arg(QChar(176)));
                foo = ui->label_Temp_1->text();
                QDateTime dateTime;
                dateTime.setTime_t(time);
                labelTag[countDate]->setText(QString("%1").arg(dateTime.toString("dddd")));
                countDate++;
                xmlReader->readNext();
                break;
            }else{
                xmlReader->readNext();
            }
        }
        if (xmlReader->isStartElement())
        {
            if (xmlReader->name() == "d"){ //Linux-Timestamp, Localtime
                time = xmlReader->readElementText().toInt();
            } else if (xmlReader->name() == "w"){ //wettersymbol aus 1. Ziffer
                wetterSymbol = xmlReader->readElementText().toInt();
            } else if (xmlReader->name() == "pc"){ //Niederschlagswahrscheinlichkeit in %
                regenWahrscheinlichkeit = xmlReader->readElementText().toInt();
            } else if (xmlReader->name() == "tn"){ //Minimaltemperatur
                minTemp = xmlReader->readElementText().toInt();
            } else if (xmlReader->name() == "tx"){ //Maximaltemperatur
                maxTemp = xmlReader->readElementText().toInt();
            } else if (xmlReader->name() == "time"){
                while (!(xmlReader->isEndElement() && (xmlReader->name() == "time"))){
                    xmlReader->readNext();
                }
            } else {
                xmlReader->readNext();
            }
        }else{
            xmlReader->readNext();
        }
    }

}

void hsGui::showJalEinstellungen()
{
    requestConfigFile(CONFIG_HS);
    je->setTcpClient(tcpClient);
    ui->centralWidget->setDisabled(true);
    je->validData(false);
    je->show();
}

void hsGui::showLichtEinstellungen()
{
    ui->centralWidget->setDisabled(true);
    le->show();
}

void hsGui::showAllgemeineEinstellungen()
{
    requestConfigFile(LOGFILE_INVERS);
    ui->centralWidget->setDisabled(true);
    ae->show();
}

void hsGui::closeJalEinstellungen(int)
{
    ui->centralWidget->setDisabled(false);
}

void hsGui::closeLichtEinstellungen(int)
{
    ui->centralWidget->setDisabled(false);
}

void hsGui::showSchliessen()
{
    sch->setTcpClient(tcpClient);
    ui->centralWidget->setDisabled(true);
    sch->show();
}

void hsGui::closeSchliessen(int)
{
    ui->centralWidget->setDisabled(false);
}

void hsGui::closeAllgemeineEinstellungen(int)
{
    ui->centralWidget->setDisabled(false);
}

void hsGui::readFromTCPServer()
{
    QByteArray recvByteArray, xmlByteArray;
    QByteArray xmlStartTag("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    QString tmp, iniValue;
    QByteArray tmpByteArray;
    int flag;
    bool stateVentil;
    if(tcpClient->bytesAvailable() > 0)
    {
        recvByteArray = tcpClient->read(tcpClient->bytesAvailable());
        while(recvByteArray.count(xmlStartTag) > 0) //es koennen mehrere XML-Streams mit einem TCP-Paket angekommen sein
        {
            xmlByteArray = recvByteArray.right(recvByteArray.size() - recvByteArray.lastIndexOf(xmlStartTag));
            recvByteArray.truncate(recvByteArray.lastIndexOf(xmlStartTag));
            QXmlStreamReader tcpXMLStream(xmlByteArray);
            tcpXMLStream.readNext();
            while(!tcpXMLStream.atEnd())
            {
                if( tcpXMLStream.isStartElement())
                {
                    if(tcpXMLStream.name() == "hsinfo")
                    {
                        tcpXMLStream.readNext();
                        while(!tcpXMLStream.atEnd())
                        {
                            if(tcpXMLStream.name() == "TempInnen")
                            {
                                ui->label_Temp_innen->setText(QString("%1%2C").arg(tcpXMLStream.readElementText()).arg(QChar(176)));
                                ui->label_TempZentraleAbluft->setText(ui->label_Temp_innen->text());
                            }else if(tcpXMLStream.name() == "TempHWR")
                            {
                                ui->label_TempHWR->setText(QString("%1%2C").arg(tcpXMLStream.readElementText()).arg(QChar(176)));
                            }else if(tcpXMLStream.name() == "TempAussen")
                            {
                                ui->label_Temp_aussen->setText(QString("%1%2C").arg(tcpXMLStream.readElementText()).arg(QChar(176)));
                            }else if(tcpXMLStream.name() == "Orient")
                            {
                                orient = (bool)tcpXMLStream.readElementText().compare(QString("Aus"));
                                setOrientIcon();
                            }else if(tcpXMLStream.name() == "HWR")
                            {
                                ui->label_StatusAbluftHWR->setText(tcpXMLStream.readElementText());
                            }else if(tcpXMLStream.name() == "Abluft")
                            {
                                ui->label_StatusZentraleAbluft->setText(tcpXMLStream.readElementText());
                            }else if(tcpXMLStream.name() == "TCPConnected")
                            {
                                flag = tcpXMLStream.readElementText().toInt();
                                ui->label_ConnectedMain->setEnabled(flag & MAIN_IS_CONNECTED);
                                ui->label_ConnectedHWR->setEnabled(flag & HWR_IS_CONNECTED);
                                ui->label_ConnectedTerrasse->setEnabled(flag & TERRASSE_IS_CONNECTED);
                            }else if(tcpXMLStream.name() == "Ventil_1")
                            {
                                stateVentil = (tcpXMLStream.readElementText() == QString("Aus"));
                                ui->pushButton_Open_1->setChecked(!stateVentil);
                                ui->pushButton_Close_1->setChecked(stateVentil);
                                if(stateVentil)
                                {
                                    //ui->pushButton_Open_1->setText(QString("Oeffnen"));
                                    ui->pushButton_Open_1->setText(QString::fromUtf8("Öffnen").toLatin1());
                                    ui->pushButton_Close_1->setText(QString("Geschlossen"));
                                }else{
                                    ui->pushButton_Open_1->setText(QString("Offen"));
                                    ui->pushButton_Close_1->setText(QString::fromUtf8("Schließen").toLatin1());
                                }
                            }else if(tcpXMLStream.name() == "Ventil_2")
                            {
                                stateVentil = (tcpXMLStream.readElementText() == QString("Aus"));
                                ui->pushButton_Open_2->setChecked(!stateVentil);
                                ui->pushButton_Close_2->setChecked(stateVentil);
                                if(stateVentil)
                                {
                                    ui->pushButton_Open_2->setText(QString::fromUtf8("Öffnen").toLatin1());
                                    ui->pushButton_Close_2->setText(QString("Geschlossen"));
                                }else{
                                    ui->pushButton_Open_2->setText(QString("Offen"));
                                    ui->pushButton_Close_2->setText(QString::fromUtf8("Schließen").toLatin1());
                                }
                            }
                            tcpXMLStream.readNext();
                        }
                    }
                    else if(tcpXMLStream.name() == "hs_conf")
                    {
                        hsconfFile->open();
                        QString foo(tcpXMLStream.readElementText());
                        QTextStream out(hsconfFile);
                        out << foo;
                        hsconfFile->close();
                        tmp = hsconfFile->fileName();
                        if(ui->tabWidget->currentIndex() == 4)
                        {
                            updateTabGiessen();
                        }
                        if(ui->tabWidget->currentIndex() == 3)
                        {
                            updateTabLueftung();
                        }
                        if(je->isVisible())
                        {
                            je->updateJalEinstellungen();
                        }
                        setUrlRegenRadar();
                    }
                    else if(tcpXMLStream.name() == "hslight")
                    {
                        hslightFile->open();
                        QString foo(tcpXMLStream.readElementText());
                        QTextStream out(hslightFile);
                        out << foo;
                        hslightFile->close();

                        updateLicht();
                    }
                    else if(tcpXMLStream.name() == "logfile")
                    {
                        ae->setLogText(tcpXMLStream.readElementText());
                    }
                    else if(tcpXMLStream.name() == "log_inv")
                    {
                        ae->setLogText(tcpXMLStream.readElementText());
                    }
                }
                tcpXMLStream.readNext();

            }
        }
    }
}

void hsGui::setUrlRegenRadar()
{
    dictionary *hsconf;
    QByteArray hsconfByteArray = hsconfFile->fileName().toLatin1();
    QString strUrlRegenRadar_old = strUrlRegenRadar;
    hsconf = iniparser_load(hsconfByteArray.data());
    strUrlRegenRadar = QString(iniparser_getstring(hsconf, "hsgui:url_regenradar", NULL));
    iniparser_freedict(hsconf);
    if(strUrlRegenRadar_old.compare(strUrlRegenRadar, Qt::CaseInsensitive) != 0)
    {
        updateRegenRadar();
    }
}

void hsGui::requestConfigFile(int value)
{
    QString sendString = QString("cmd=%1 value=%2\n").arg(CMD_GET_CONFIG).arg(value);
    tcpClient->write(sendString.toLatin1());
}

void hsGui::setSharedMemoryReceiveOnServer()
{
    QString sendString = QString("cmd=%1 value=\n").arg(CMD_GET_HSINFO);
    tcpClient->write(sendString.toLatin1());
}

void hsGui::connectedTCPServer()
{
    ui->label_ConnectedGUI->setEnabled(true);
    initTCPConnection();
}

void hsGui::pushButton_Terrasse()
{
    QString sendString;
    QToolButton *toolButton = (QToolButton*)sender();
    QString name = toolButton->objectName();
    if(name.contains("Open"))
    {
        sendString = QString("cmd=%1 value=VENTIL_%2_START\n").arg(CMD_WATER).arg(name.right(1));
    }
    else
    {
        sendString = QString("cmd=%1 value=VENTIL_%2_STOP\n").arg(CMD_WATER).arg(name.right(1));
    }
    tcpClient->write(sendString.toLatin1());
}

void hsGui::pushButton_Terrasse_Timer()
{
    int dauer;
    QToolButton *toolButton = (QToolButton*)sender();
    QString name = toolButton->objectName();
    if(name.right(1).toInt() == 1)
    {
        dauer = ui->spinBox_DauerTimer_1->value();
    }
    else
    {
        dauer = ui->spinBox_DauerTimer_2->value();
    }
    QString sendString = QString("cmd=%1 value=VENTIL_%2_TIMER:%3\n").arg(CMD_WATER).arg(name.right(1)).arg(dauer);
    tcpClient->write(sendString.toLatin1());
}


void hsGui::on_checkBox_AutoWater_1_toggled(bool checked)
{
    if(checked)
    {
        ui->spinBox_DauerWater_1->setEnabled(true);
        ui->timeEdit_StartWater_1->setEnabled(true);
        ui->label_StartWater_1->setEnabled(true);
        ui->label_DauerWater_1->setEnabled(true);
    }
    else
    {
        ui->spinBox_DauerWater_1->setEnabled(false);
        ui->timeEdit_StartWater_1->setEnabled(false);
        ui->label_StartWater_1->setEnabled(false);
        ui->label_DauerWater_1->setEnabled(false);
    }
}

void hsGui::on_checkBox_AutoWater_2_toggled(bool checked)
{
    if(checked)
    {
        ui->spinBox_DauerWater_2->setEnabled(true);
        ui->timeEdit_StartWater_2->setEnabled(true);
        ui->label_StartWater_2->setEnabled(true);
        ui->label_DauerWater_2->setEnabled(true);
    }
    else
    {
        ui->spinBox_DauerWater_2->setEnabled(false);
        ui->timeEdit_StartWater_2->setEnabled(false);
        ui->label_StartWater_2->setEnabled(false);
        ui->label_DauerWater_2->setEnabled(false);
    }
}

void hsGui::updateTabGiessen()
{
    dictionary *hsconf;
    QByteArray hsconfByteArray = hsconfFile->fileName().toLatin1();
    hsconf = iniparser_load(hsconfByteArray.data());
    for(int i = 0; i < 6; i++)
    {
        if(confWidgetsTerrasse[i].isBool)
        {
            QAbstractButton *foo = (QAbstractButton*)confWidgetsTerrasse[i].object;
            confWidgetsTerrasse[i].boolValue = iniparser_getboolean(hsconf, QString(confWidgetsTerrasse[i].section + ":" + confWidgetsTerrasse[i].key).toLatin1(), false);
            foo->setChecked(confWidgetsTerrasse[i].boolValue);
        }else if(confWidgetsTerrasse[i].isInt)
        {
            QSpinBox *foo = (QSpinBox*)confWidgetsTerrasse[i].object;
            confWidgetsTerrasse[i].intValue = iniparser_getint(hsconf, QString(confWidgetsTerrasse[i].section + ":" + confWidgetsTerrasse[i].key).toLatin1(), 0);
            foo->setValue(confWidgetsTerrasse[i].intValue);
        }else if(confWidgetsTerrasse[i].isTime)
        {
            QTimeEdit *foo = (QTimeEdit*)confWidgetsTerrasse[i].object;
            confWidgetsTerrasse[i].strValue = QString(iniparser_getstring(hsconf, QString(confWidgetsTerrasse[i].section + ":" + confWidgetsTerrasse[i].key).toLatin1(), (char*)"00:00"));
            foo->setTime(QTime::fromString(confWidgetsTerrasse[i].strValue, "h:m"));
        }
    }
    iniparser_freedict(hsconf);
    ui->tab_Giessen->setEnabled(true);
}

void hsGui::on_tabWidget_currentChanged(int index)
{
    if(index == 4)
    {
        ui->tab_Giessen->setEnabled(false);
        requestConfigFile(CONFIG_HS);
    }
    if(index == 3)
    {
        ui->tab_Lueftung->setEnabled(false);
        requestConfigFile(CONFIG_HS);
    }
    if(index == 2)
    {
        on_pushButtonWetterPlot_clicked();
    }
}

void hsGui::initConfWidgets()
{
    //fuer Terrasse
    QString keys[] = {QString("ventil_1_auto"), QString("ventil_2_auto"), QString("ventil_1_duration"),
                    QString("ventil_2_duration"), QString("ventil_1_start"), QString("ventil_2_start")};
    QObject *objects[] = {ui->checkBox_AutoWater_1, ui->checkBox_AutoWater_2, ui->spinBox_DauerWater_1,
                          ui->spinBox_DauerWater_2, ui->timeEdit_StartWater_1, ui->timeEdit_StartWater_2};
    bool isInts[] = {false, false, true, true, false, false};
    bool isTimes[] = {false, false, false, false, true, true};
    bool isBools[] = {true, true, false, false, false, false};

    for(int i = 0; i < 6; i++)
    {
        confWidgetsTerrasse[i].section = QString("irrigation");
        confWidgetsTerrasse[i].object = objects[i];
        confWidgetsTerrasse[i].key = keys[i];
        confWidgetsTerrasse[i].isTime = isTimes[i];
        confWidgetsTerrasse[i].isInt = isInts[i];
        confWidgetsTerrasse[i].isBool = isBools[i];
        confWidgetsTerrasse[i].hasChanged = false;
    }
}

void hsGui::on_pushButton_WaterOK_clicked()
{
    for(int i = 0; i < 6; i++)
    {
        if(confWidgetsTerrasse[i].isBool)
        {
            QAbstractButton *foo = (QAbstractButton*)confWidgetsTerrasse[i].object;
            if(foo->isChecked() != confWidgetsTerrasse[i].boolValue)
            {
                QString sendString = QString("cmd=%1 value=%2key=%3\n").arg(CMD_SET_CONFIG)
                           .arg(foo->isChecked()).arg(confWidgetsTerrasse[i].section + ":" + confWidgetsTerrasse[i].key);
                tcpClient->write(sendString.toLatin1());
                confWidgetsTerrasse[i].boolValue = foo->isChecked();
            }
        }else if(confWidgetsTerrasse[i].isInt)
        {
            QSpinBox *foo = (QSpinBox*)confWidgetsTerrasse[i].object;
            if(foo->value() != confWidgetsTerrasse[i].intValue)
            {
                QString sendString = QString("cmd=%1 value=%2key=%3\n").arg(CMD_SET_CONFIG)
                           .arg(foo->value()).arg(confWidgetsTerrasse[i].section + ":" + confWidgetsTerrasse[i].key);
                tcpClient->write(sendString.toLatin1());
                confWidgetsTerrasse[i].intValue = foo->value();
            }
        }else if(confWidgetsTerrasse[i].isTime)
        {
            QTimeEdit *foo = (QTimeEdit*)confWidgetsTerrasse[i].object;
            if(!(foo->time().toString("hh:mm").contains(confWidgetsTerrasse[i].strValue)))
            {
                QString sendString = QString("cmd=%1 value=%2key=%3\n").arg(CMD_SET_CONFIG)
                                     .arg(foo->time().toString("hh:mm")).arg(confWidgetsTerrasse[i].section + ":" + confWidgetsTerrasse[i].key);
                tcpClient->write(sendString.toLatin1());
                confWidgetsTerrasse->strValue = foo->time().toString("hh:mm");
            }
        }
    }
}

void hsGui::on_pushButton_WaterReset_clicked()
{
    for(int i = 0; i < 6; i++)
    {
        if(confWidgetsTerrasse[i].isBool)
        {
            QAbstractButton *foo = (QAbstractButton*)confWidgetsTerrasse[i].object;
            foo->setChecked(confWidgetsTerrasse[i].boolValue);
        }else if(confWidgetsTerrasse[i].isInt)
        {
            QSpinBox *foo = (QSpinBox*)confWidgetsTerrasse[i].object;
            foo->setValue(confWidgetsTerrasse[i].intValue);
        }else if(confWidgetsTerrasse[i].isTime)
        {
            QTimeEdit *foo = (QTimeEdit*)confWidgetsTerrasse[i].object;
            foo->setTime(QTime::fromString(confWidgetsTerrasse[i].strValue, "h:m"));
        }
    }
}



void hsGui::updateTabLueftung()
{
    dictionary *hsconf;
    QByteArray hsconfByteArray = hsconfFile->fileName().toLatin1();
    hsconf = iniparser_load(hsconfByteArray.data());
    confLueftungTab.bHWRTemp = iniparser_getboolean(hsconf, QString("hwr:auto").toLatin1(),0);
    confLueftungTab.bHWRDauer = iniparser_getboolean(hsconf, QString("hwr:permanent").toLatin1(),0);
    confLueftungTab.bACLow = iniparser_getboolean(hsconf, QString("a/c:low").toLatin1(),0);
    confLueftungTab.iHWRSchelle = iniparser_getint(hsconf, QString("hwr:threshold").toLatin1(), 0);
    on_pushButton_AbluftReset_clicked();
    iniparser_freedict(hsconf);
    ui->tab_Lueftung->setEnabled(true);
}

void hsGui::on_pushButton_AbluftReset_clicked()
{
    ui->spinBox_SchaltschwelleHWR->setValue(confLueftungTab.iHWRSchelle);
    ui->radioButton_ZentrAbluftAus->setChecked(!confLueftungTab.bACLow);
    ui->radioButton_ZentrAbluftAn->setChecked(confLueftungTab.bACLow);
    ui->radioButton_AbluftHWRAus->setChecked(!confLueftungTab.bHWRDauer && !confLueftungTab.bHWRTemp);
    ui->radioButton_AbluftHWRTemp->setChecked(confLueftungTab.bHWRTemp);
    ui->radioButton_AbluftHWRDauer->setChecked(confLueftungTab.bHWRDauer);
    on_radioButton_AbluftHWRTemp_toggled(confLueftungTab.bHWRTemp);
}

void hsGui::on_pushButton_AbluftOK_clicked()
{
    QString sendString;
    if(ui->spinBox_SchaltschwelleHWR->value() != confLueftungTab.iHWRSchelle)
    {
        confLueftungTab.iHWRSchelle = ui->spinBox_SchaltschwelleHWR->value();
        sendString = QString("cmd=%1 value=%2key=%3\n").arg(CMD_SET_CONFIG)
                         .arg(confLueftungTab.iHWRSchelle).arg(QString("hwr:threshold"));
        tcpClient->write(sendString.toLatin1());
    }
    if(confLueftungTab.bHWRDauer != ui->radioButton_AbluftHWRDauer->isChecked())
    {
        confLueftungTab.bHWRDauer = ui->radioButton_AbluftHWRDauer->isChecked();
        sendString = QString("cmd=%1 value=%2key=%3\n").arg(CMD_SET_CONFIG)
                         .arg(confLueftungTab.bHWRDauer).arg(QString("hwr:permanent"));
        tcpClient->write(sendString.toLatin1());
    }
    if(confLueftungTab.bHWRTemp != ui->radioButton_AbluftHWRTemp->isChecked())
    {
        confLueftungTab.bHWRTemp = ui->radioButton_AbluftHWRTemp->isChecked();
        sendString = QString("cmd=%1 value=%2key=%3\n").arg(CMD_SET_CONFIG)
                         .arg(confLueftungTab.bHWRTemp).arg(QString("hwr:auto"));
        tcpClient->write(sendString.toLatin1());
    }
    if(confLueftungTab.bACLow != ui->radioButton_ZentrAbluftAn->isChecked())
    {
        confLueftungTab.bACLow = ui->radioButton_ZentrAbluftAn->isChecked();
        sendString = QString("cmd=%1 value=%2key=%3\n").arg(CMD_SET_CONFIG)
                         .arg(confLueftungTab.bACLow).arg(QString("a/c:low"));
        tcpClient->write(sendString.toLatin1());
    }
}

void hsGui::on_radioButton_AbluftHWRTemp_toggled(bool checked)
{
    ui->label_Schaltschwelle->setEnabled(checked);
    ui->spinBox_SchaltschwelleHWR->setEnabled(checked);
    ui->toolButton_SchaltschwelleHWRDown->setEnabled(checked);
    ui->toolButton_SchaltschwelleHWRUp->setEnabled(checked);
}

void hsGui::on_pushButtonWetterPlot_clicked()
{
    int intKurve1, intKurve2, intZeitraum;
    if(ui->radioButton_Wetter_1_1->isChecked()){intKurve1 = 1;}
    if(ui->radioButton_Wetter_1_2->isChecked()){intKurve1 = 2;}
    if(ui->radioButton_Wetter_1_3->isChecked()){intKurve1 = 3;}
    if(ui->radioButton_Wetter_1_4->isChecked()){intKurve1 = 4;}
    if(ui->radioButton_Wetter_1_5->isChecked()){intKurve1 = 5;}
    if(ui->radioButton_Wetter_1_6->isChecked()){intKurve1 = 6;}
    if(ui->radioButton_Wetter_1_7->isChecked()){intKurve1 = 7;}
    if(ui->radioButton_Wetter_2_1->isChecked()){intKurve2 = 1;}
    if(ui->radioButton_Wetter_2_2->isChecked()){intKurve2 = 2;}
    if(ui->radioButton_Wetter_2_3->isChecked()){intKurve2 = 3;}
    if(ui->radioButton_Wetter_2_4->isChecked()){intKurve2 = 4;}
    if(ui->radioButton_Wetter_2_5->isChecked()){intKurve2 = 5;}
    if(ui->radioButton_Wetter_2_6->isChecked()){intKurve2 = 6;}
    if(ui->radioButton_Wetter_2_7->isChecked()){intKurve2 = 0;}
    if(ui->radioButton_Wetter_3_1->isChecked()){intZeitraum = 1;}
    if(ui->radioButton_Wetter_3_2->isChecked()){intZeitraum = 2;}
    if(ui->radioButton_Wetter_3_3->isChecked()){intZeitraum = 3;}
    if(ui->radioButton_Wetter_3_4->isChecked()){intZeitraum = 4;}

    QString strPlotWetterUrl = QString("http://192.168.178.10/plot_wetter.php");
    QUrl urlPlotWetter = QUrl(strPlotWetterUrl);
    QByteArray postData;
    postData.append(QString("intKurve1=%1").arg(intKurve1));
    postData.append(QString("&intKurve2=%1").arg(intKurve2));
    postData.append(QString("&intZeitraum=%1").arg(intZeitraum));
    postData.append("&boolGUI=1");
    QNetworkRequest *reqPlotWetter = new QNetworkRequest(urlPlotWetter);
    reqPlotWetter->setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg("florian").arg("vegas").toLatin1()).toBase64());
    networkAccess_wetterplot->post(*reqPlotWetter, postData);
}


