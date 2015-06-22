#ifndef JALOUSIEEINSTELLUNGEN_H
#define JALOUSIEEINSTELLUNGEN_H

#include <QDialog>
#include <QtGui>
#include <QtNetwork/QTcpSocket>

#include "iniparser.h"
#include "hsguiglobal.h"
#include "hsguinetcmd.h"

namespace Ui {
    class JalousieEinstellungen;
}


class JalousieEinstellungen : public QDialog
{
    Q_OBJECT

public:
    explicit JalousieEinstellungen(QWidget *parent = 0);
    ~JalousieEinstellungen();
    void setFullScreen(bool fullscreen);
    void setMouseZeiger(bool mousezeiger);
    void setTcpClient(QTcpSocket *client) {tcpClient = client;}
    void setHsconfFile(QTemporaryFile *tmp) {hsconfFile = tmp;}
    void updateJalEinstellungen();
    void validData(bool valid);

private:
    QTcpSocket *tcpClient;
    QTemporaryFile *hsconfFile;
    confElement confWidgetsJal[8];
    Ui::JalousieEinstellungen *ui;
    void initConfWidgets();

private slots:
    void on_radioButton_Auf_SU_toggled(bool checked);
    void on_radioButton_Auf_SA_toggled(bool checked);
    void on_radioButton_Zu_SU_toggled(bool checked);
    void on_radioButton_Zu_SA_toggled(bool checked);
    void on_checkBox_Zeitautomatik_clicked();
    void on_radioButton_Auf_Uhrzeit_toggled(bool checked);
    void on_radioButton_Zu_Uhrzeit_toggled(bool checked);
    void on_pushButton_JalOK_clicked();
    void on_pushButton_JalReset_clicked();
};

#endif // JALOUSIEEINSTELLUNGEN_H
