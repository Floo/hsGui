#include "jalousieeinstellungen.h"
#include "ui_jalousieeinstellungen.h"


JalousieEinstellungen::JalousieEinstellungen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JalousieEinstellungen)
{
    ui->setupUi(this);
    initConfWidgets();
    //QRect scr = QApplication::desktop()->screenGeometry();
    //move( scr.center() - rect().center() );
    //QRect aktiveArea = ui->frame->geometry();
    //setMask(QRegion(aktiveArea));
}

JalousieEinstellungen::~JalousieEinstellungen()
{
    delete ui;
}

void JalousieEinstellungen::setMouseZeiger(bool mousezeiger)
{
    mousezeiger?setCursor(Qt::ArrowCursor):setCursor(Qt::BlankCursor);
}

void JalousieEinstellungen::setFullScreen(bool fullscreen)
{
    fullscreen?setWindowState(Qt::WindowFullScreen):setWindowState(Qt::WindowActive);
}

void JalousieEinstellungen::validData(bool valid)
{
    ui->frame_Zeitautomatik->setEnabled(valid);
    ui->frame_Optionen->setEnabled(valid);
    ui->pushButton_JalOK->setEnabled(valid);
    ui->pushButton_JalReset->setEnabled(valid);
    on_checkBox_Zeitautomatik_clicked();
    on_radioButton_Auf_Uhrzeit_toggled(false);
    on_radioButton_Zu_Uhrzeit_toggled(false);
}

void JalousieEinstellungen::initConfWidgets()
{
    /*
    [blinds]
    up_time = 09:10
    down_time = 16:11
    auto_time = false
    weather = false
    wind_protection = true
    hytersis_time = 30
    brightness_thrs = 655
    wind_speed_thrs = 38
    open_on_rain = true
    outside_temp_thrs = 3
    jal_2_open = 1
    close_to_sun = true
    */
    QString keys[] = {QString("auto_time"),
                      QString("weather"),
                      QString("wind_protection"),
                      QString("open_on_rain"),
                      QString("jal_2_open"),
                      QString("close_to_sun"),
                      QString("up_time"),
                      QString("down_time")};
    QObject *objects[] = {ui->checkBox_Zeitautomatik,
                          ui->checkBox_Wettersteuerung,
                          ui->checkBox_OpenWind,
                          ui->checkBox_OpenRain,
                          ui->checkBox_TuerOffen,
                          ui->checkBox_aufLuecke,
                          ui->frame_Auf,
                          ui->frame_Zu};

    bool isTimes[] = {false, false, false, false, false, false, true, true};
    bool isBools[] = {true, true, true, true, true, true, false, false};
    for(int i = 0; i < 8; i++)
    {
        confWidgetsJal[i].section = QString("blinds");
        confWidgetsJal[i].object = objects[i];
        confWidgetsJal[i].key = keys[i];
        confWidgetsJal[i].isTime = isTimes[i];
        confWidgetsJal[i].isBool = isBools[i];
        confWidgetsJal[i].isInt = false;
    }
    ui->frame_Auf->children();
}

void JalousieEinstellungen::updateJalEinstellungen()
{
    dictionary *hsconf;
    QByteArray hsconfByteArray = hsconfFile->fileName().toLatin1();
    hsconf = iniparser_load(hsconfByteArray.data());
    ui->spinBox_Auf_Offset->setValue(0);
    ui->spinBox_Zu_Offset->setValue(0);
    ui->timeEdit_Auf->setTime(QTime::fromString("00:00"));
    ui->timeEdit_Zu->setTime(QTime::fromString("00:00"));
    for(int i = 0; i < 8; i++)
    {
        if(confWidgetsJal[i].isBool)
        {
            QAbstractButton *foo = (QAbstractButton*)confWidgetsJal[i].object;
            confWidgetsJal[i].boolValue = iniparser_getboolean(hsconf, QString(confWidgetsJal[i].section + ":" + confWidgetsJal[i].key).toLatin1(), false);
            foo->setChecked(confWidgetsJal[i].boolValue);
        }else if(confWidgetsJal[i].isTime)
        {
            QSpinBox *spinBox_Offset = confWidgetsJal[i].object->findChild<QSpinBox *>();
            QTimeEdit *timeEdit_Uhrzeit = confWidgetsJal[i].object->findChild<QTimeEdit *>();
            QList<QRadioButton *> listRadioButton = confWidgetsJal[i].object->findChildren<QRadioButton *>();
            confWidgetsJal[i].strValue = QString(iniparser_getstring(hsconf, QString(confWidgetsJal[i].section + ":" + confWidgetsJal[i].key).toLatin1(), NULL));
            if(confWidgetsJal[i].strValue.contains(":")) //Uhrzeit
            {
                timeEdit_Uhrzeit->setTime(QTime::fromString(confWidgetsJal[i].strValue, "h:m"));
                for(int k = 0; k < 3; k++)
                {
                    listRadioButton[k]->setChecked(listRadioButton[k]->objectName().contains("Uhrzeit"));
                }
            }
            else
            {
                if(confWidgetsJal[i].strValue.contains("SA"))
                {
                    for(int k = 0; k < 3; k++)
                    {
                        listRadioButton[k]->setChecked(listRadioButton[k]->objectName().contains("SA"));
                    }
                }
                else if(confWidgetsJal[i].strValue.contains("SU"))
                {
                    for(int k = 0; k < 3; k++)
                    {
                        listRadioButton[k]->setChecked(listRadioButton[k]->objectName().contains("SU"));
                    }
                }
                spinBox_Offset->setValue(confWidgetsJal[i].strValue.right(confWidgetsJal[i].strValue.size() - 2).toInt());
            }
        }
    }
    iniparser_freedict(hsconf);
    validData(true);
}


void JalousieEinstellungen::on_pushButton_JalReset_clicked()
{
    updateJalEinstellungen();
}

void JalousieEinstellungen::on_pushButton_JalOK_clicked()
{
    QString sendString;
    QString tmpString;
    for(int i = 0; i < 8; i++)
    {
        sendString  = QString("cmd=%1 value=%2key=%3\n").arg(CMD_SET_CONFIG);
        tmpString  = QString("%1%2");
        if(confWidgetsJal[i].isBool)
        {
            QAbstractButton *foo = (QAbstractButton*)confWidgetsJal[i].object;
            if(foo->isChecked() != confWidgetsJal[i].boolValue)
            {
                sendString = QString(sendString).arg(foo->isChecked()).arg(confWidgetsJal[i].section + ":" + confWidgetsJal[i].key);
                tcpClient->write(sendString.toLatin1());
                confWidgetsJal[i].boolValue = foo->isChecked();
            }
        }else if(confWidgetsJal[i].isTime)
        {
            QSpinBox *spinBox_Offset = confWidgetsJal[i].object->findChild<QSpinBox *>();
            QTimeEdit *timeEdit_Uhrzeit = confWidgetsJal[i].object->findChild<QTimeEdit *>();
            QList<QRadioButton *> listRadioButton = confWidgetsJal[i].object->findChildren<QRadioButton *>();
            for(int k = 0; k < 3; k++)
            {
                if(listRadioButton[k]->objectName().contains("Uhrzeit") && listRadioButton[k]->isChecked())
                {
                    tmpString = timeEdit_Uhrzeit->time().toString("hh:mm");
                    break;
                }
            }
            for(int k = 0; k < 3; k++)
            {
                if(listRadioButton[k]->objectName().contains("SA") && listRadioButton[k]->isChecked())
                {
                    if(spinBox_Offset->value() > -1)
                    {
                        tmpString = QString(tmpString).arg("SA+").arg(spinBox_Offset->value());
                    }else{
                        tmpString = QString(tmpString).arg("SA").arg(spinBox_Offset->value());
                    }
                    break;
                }
            }
            for(int k = 0; k < 3; k++)
            {
                if(listRadioButton[k]->objectName().contains("SU") && listRadioButton[k]->isChecked())
                {
                    if(spinBox_Offset->value() > -1)
                    {
                        tmpString = QString(tmpString).arg("SU+").arg(spinBox_Offset->value());
                    }else{
                        tmpString = QString(tmpString).arg("SU").arg(spinBox_Offset->value());
                    }
                    break;
                }
            }
            if(confWidgetsJal[i].strValue != tmpString)
            {
                confWidgetsJal[i].strValue = tmpString;
                sendString = QString(sendString).arg(tmpString).arg(confWidgetsJal[i].section + ":" + confWidgetsJal[i].key);
                tcpClient->write(sendString.toLatin1());
            }
        }
    }
    this->close();
}


void JalousieEinstellungen::on_radioButton_Zu_Uhrzeit_toggled(bool)
{
    ui->frame_Zu_Uhrzeit->setEnabled(ui->radioButton_Zu_Uhrzeit->isChecked());
    ui->frame_Zu_Offset->setEnabled(!ui->radioButton_Zu_Uhrzeit->isChecked());
}

void JalousieEinstellungen::on_radioButton_Auf_Uhrzeit_toggled(bool)
{
    ui->frame_Auf_Uhrzeit->setEnabled(ui->radioButton_Auf_Uhrzeit->isChecked());
    ui->frame_Auf_Offset->setEnabled(!ui->radioButton_Auf_Uhrzeit->isChecked());
}

void JalousieEinstellungen::on_checkBox_Zeitautomatik_clicked()
{
    ui->frame_Auf->setEnabled(ui->checkBox_Zeitautomatik->isChecked());
    ui->frame_Zu->setEnabled(ui->checkBox_Zeitautomatik->isChecked());
}

void JalousieEinstellungen::on_radioButton_Zu_SA_toggled(bool checked)
{
    ui->radioButton_Auf_SA->setEnabled(!checked);
    if(ui->radioButton_Auf_SA->isChecked() && checked)
    {
        ui->radioButton_Auf_SA->setChecked(false);
        ui->radioButton_Auf_SU->setChecked(true);
    }
}

void JalousieEinstellungen::on_radioButton_Zu_SU_toggled(bool checked)
{
    ui->radioButton_Auf_SU->setEnabled(!checked);
    if(ui->radioButton_Auf_SU->isChecked() && checked)
    {
        ui->radioButton_Auf_SU->setChecked(false);
        ui->radioButton_Auf_SA->setChecked(true);
    }
}

void JalousieEinstellungen::on_radioButton_Auf_SA_toggled(bool checked)
{
    ui->radioButton_Auf_SU->setEnabled(!checked);
    if(ui->radioButton_Auf_SU->isChecked() && checked)
    {
        ui->radioButton_Auf_SU->setChecked(false);
        ui->radioButton_Auf_SA->setChecked(true);
    }
}

void JalousieEinstellungen::on_radioButton_Auf_SU_toggled(bool checked)
{
    ui->radioButton_Zu_SU->setEnabled(!checked);
    if(ui->radioButton_Zu_SU->isChecked() && checked)
    {
        ui->radioButton_Zu_SU->setChecked(false);
        ui->radioButton_Zu_SA->setChecked(true);
    }
}
