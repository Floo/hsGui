#ifndef MPNETWORKTHREAD_H
#define MPNETWORKTHREAD_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QtNetwork/QTcpSocket>


class MpNetworkThread : public QThread
{
    Q_OBJECT

public:
    MpNetworkThread(QObject *parent = 0);
    ~MpNetworkThread();
    void sendCommand(const QString &hostName, quint16 port, QString command);
    void run() Q_DECL_OVERRIDE;

signals:
    void newReply(const QString &fortune);
    void error(int socketError, const QString &message);


private:
    QString hostName;
    quint16 port;
    QMutex mutex;
    QString cmd;
    QWaitCondition cond;
    bool quit;
};

#endif // MPNETWORKTHREAD_H
