#ifndef LICHTEINSTELLUNGEN_H
#define LICHTEINSTELLUNGEN_H

#include <QDialog>
#include <QtGui>
#include <QtNetwork/QTcpSocket>

#include "hsguinetcmd.h"

namespace Ui {
    class LichtEinstellungen;
}

class LichtEinstellungen : public QDialog
{
    Q_OBJECT

public:
    explicit LichtEinstellungen(QWidget *parent = 0);
    ~LichtEinstellungen();
    void setResetTimerButton(int button, QString name);
    void updateLichtEinstellungen(QList<QString> *pLichtDevices, QList<QString> *pLichtDeviceNamen);
    void setTcpClient(QTcpSocket *pTcpClient);
    void setFullScreen(bool fullscreen);
    void setMouseZeiger(bool mousezeiger);


private:
    QList<QToolButton*> lichtResetTimerToolButton;
    QList<QString> *lichtDevices;
    QTcpSocket *tcpClient;
    Ui::LichtEinstellungen *ui;
    void initLichtEinstellungen();

private slots:
    void sendTimerReset();

};

#endif // LICHTEINSTELLUNGEN_H
