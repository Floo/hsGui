#ifndef SCHLIESSEN_H
#define SCHLIESSEN_H

#include <QtCore>
#include <QProcess>
#include <QDialog>
#include <QtNetwork/QTcpSocket>

#include "hsguinetcmd.h"

namespace Ui {
    class Schliessen;
}

class Schliessen : public QDialog
{
    Q_OBJECT

public:
    explicit Schliessen(QWidget *parent = 0);
    ~Schliessen();
    void setFullScreen(bool fullscreen);
    void setMouseZeiger(bool mousezeiger);
    void setTcpClient(QTcpSocket *client) {tcpClient = client;}

private:
    bool FullScreen;
    Ui::Schliessen *ui;
    QTcpSocket *tcpClient;
    void lichtOrientSzeneAus();

private slots:
    void on_pushButton_Close_clicked();
    void on_pushButton_TouchBeenden_clicked();
    void on_pushButton_LichtTouchBeenden_clicked();
    void closeApp(qint64);
};

#endif // SCHLIESSEN_H
