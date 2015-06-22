#ifndef ALLGEMEINEEINSTELLUNGEN_H
#define ALLGEMEINEEINSTELLUNGEN_H

#include <QDialog>
#include <QtCore>
#include <QProcess>
#include <QtGui>

namespace Ui {
    class AllgemeineEinstellungen;
}

class AllgemeineEinstellungen : public QDialog
{
    Q_OBJECT

public:
    explicit AllgemeineEinstellungen(QWidget *parent = 0);
    ~AllgemeineEinstellungen();
    void setFullScreen(bool fullscreen);
    void setMouseZeiger(bool mousezeiger);
    void setLogText(QString text);

private:
    Ui::AllgemeineEinstellungen *ui;

private slots:
    void on_pushButton_restartHaussteuerung_clicked();
    void on_btnSliderDown_clicked();
    void on_btnSliderUp_clicked();
    void on_btnSliderDownMax_clicked();
    void on_btnSliderUpMax_clicked();
};

#endif // ALLGEMEINEEINSTELLUNGEN_H
