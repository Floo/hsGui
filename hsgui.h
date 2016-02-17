#ifndef HSGUI_H
#define HSGUI_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore>
#include <stdio.h>

#include "ui_hsgui.h"
#include "jalousieeinstellungen.h"
#include "lichteinstellungen.h"
#include "allgemeineeinstellungen.h"
#include "schliessen.h"
#include "hsguinetcmd.h"
#include "iniparser.h"
#include "hsguiglobal.h"


namespace Ui {
    class hsGui;
}

class MusikPlayer;


class hsGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit hsGui(QString *ipTcpConnection, int portTcpConnection, bool fullscreen, bool mousezeiger, QWidget *parent = 0);
    ~hsGui();

private:
    QString *ipTCPConnection;
    int portTCPConnection;
    QList<QToolButton*> lichtAusToolButton;
    QList<QToolButton*> lichtAnToolButton;
    QList<QToolButton*> lichtDownToolButton;
    QList<QToolButton*> lichtUpToolButton;
    QList<QToolButton*> szenenToolButton;
    QList<QLabel*> lichtLabel;
    QList<QString> szenenNamen;
    QList<QString> lichtDevices;
    QList<QString> lichtDeviceNamen;
    QTemporaryFile *hslightFile;
    QTemporaryFile *hsconfFile;
    int countDate;
    bool orient;
    Ui::hsGui *ui;
    QTcpSocket *tcpClient;
    QTimer *timerRegenRadar;
    QTimer *timerForecast;
    QTimer *timerReconnectTCP;
//    QUrl lastValidRegenRadarUrl;
    QNetworkAccessManager *networkAccess_forcast;
    QNetworkAccessManager *networkAccess_regenradar;
    QNetworkAccessManager *networkAccess_wetterplot;
    int quarter, quarterTry;
    confElement confWidgetsTerrasse[6];
    confLueftung confLueftungTab;
    JalousieEinstellungen *je;
    LichtEinstellungen *le;
    Schliessen *sch;
    AllgemeineEinstellungen *ae;
    QString strRegenRadarGif;
    QMovie *regenRadarGif;
    QPixmap *wetterPlotPNG;
    QString strUrlRegenRadar;
    MusikPlayer *musikplayer;

    void initTCPConnection();
    void xmlReadForecast(QXmlStreamReader *xmlReader);
    void xmlReadForecastDate(QXmlStreamReader *xmlReader);
    void requestConfigFile(int value);
    void setSharedMemoryReceiveOnServer();
    void initLicht();
    void updateLicht();
    void updateTabGiessen();
    void initConfWidgets();
    void setUrlRegenRadar();

private slots:
    void on_pushButtonWetterPlot_clicked();
    void on_radioButton_AbluftHWRTemp_toggled(bool checked);
    void on_pushButton_AbluftOK_clicked();
    void on_pushButton_AbluftReset_clicked();
    void on_pushButton_WaterReset_clicked();
    void on_pushButton_WaterOK_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_checkBox_AutoWater_1_toggled(bool checked);
    void on_checkBox_AutoWater_2_toggled(bool checked);
    void sendJal();
    void updateRegenRadar();
    void updateVorhersage();
    void showJalEinstellungen();
    void showLichtEinstellungen();
    void showSchliessen();
    void showAllgemeineEinstellungen();
    void closeJalEinstellungen(int value);
    void closeLichtEinstellungen(int value);
    void closeSchliessen(int value);
    void closeAllgemeineEinstellungen(int value);
    void networkRequestFinished_forcast(QNetworkReply *reply);
    void networkRequestFinished_regenradar(QNetworkReply *reply);
    void networkRequestFinished_wetterplot(QNetworkReply *reply);
    void readFromTCPServer();
    void connectedTCPServer();
    void disconnectedTCPServer();
    void tryReconnectToTCPServer();
    void errorTCPServer(QAbstractSocket::SocketError error);
    void sendLicht();
    void sendSzene();
    void sendLichtDimmPressed();
    void sendLichtDimmReleased();
    void sendOrient();
    void setOrientIcon();
    void pushButton_Terrasse();
    void pushButton_Terrasse_Timer();
    void updateTabLueftung();
};

#endif // HSGUI_H
