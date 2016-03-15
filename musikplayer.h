#ifndef MUSIKPLAYER_H
#define MUSIKPLAYER_H

#include "mpnetworkthread.h"

#include <QObject>
#include <QMessageBox>
#include <QtNetwork/QAbstractSocket>
#include <QListWidget>
#include <QUdpSocket>
#include <QDebug>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QImageReader>
#include <QUrl>
#include <QTimer>

namespace Ui {
    class hsGui;
}

class MusikPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MusikPlayer(Ui::hsGui *_ui, QObject *parent = 0);
    ~MusikPlayer();

signals:

public slots:

private slots:
    void pendingUDPDatagram();
    void displayError(int socketError, const QString &message);
    void displayReply(const QString &message);
    void loadRadioList();
    void getMetaData();
    void getStatus();
    void btnRadioList_clicked();
    void btnMode_clicked();
    void btnPlay_clicked();
    void btnStop_clicked();
    void sl_cover_fetch_done(QNetworkReply* reply);
    void sendVolUp();
    void sendVolDown();
    void sendMute();

private:
    Ui::hsGui *ui;
    MpNetworkThread networkthread;
    QUdpSocket *udpsocket;
    QNetworkAccessManager *m_netmanager;

    void applyStatus(QString& str, bool getMD = true);
    int m_port;
    QString m_host;
    int m_GUIMode;
    int m_Input;
    int m_playMode;
    bool m_initialized;
    void setModeButtons();
    void initConnections();
    void fetch_cover(const QString& URI);
    void no_cover_available();
    QTimer *m_volUpTimer;
    QTimer *m_volDownTimer;
};

#endif // MUSIKPLAYER_H
