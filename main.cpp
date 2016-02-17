//#include <QtGui/QApplication>
#include <QtGui>
#include "hsgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool fullscreen = false;
    bool mousezeiger = true;
    int port = 7022;
    QString ip("127.0.0.1");

    //Kommandozeilenargumente auswerten
    for(int i = 1; i < a.arguments().count(); i++)
    {
        QString arg(a.arguments().at(i));
        switch(arg[1].toLatin1())
        {
        case 104: //h
            //Hilfe ausgeben
            printf("\nProgrammaufruf: hsGui [OPTION]\n"\
                   "Folgende Optionen stehen"\
                   "Ihnen zur Verfuegung:\n\n"\
                   "\t-a [\"127.0.0.1\"] IP-Adresse des Servers\n"\
                   "\t-p [7022] Server-Port\n"\
                   "\t-f  Darstellung als Fullsreen\n"\
                   "\t-m  Mousezeiger verbergen\n"\
                   "\t-h  Dieser Text\n"\
                   "\n");
            exit(0);
        case 97: //a
            ip = arg.right(arg.size() - 2);
            break;
        case 112: //p
            port = arg.right((arg.size() - 2)).toInt();
            break;
        case 102: //f
            fullscreen = true;
            break;
        case 109: //m
            mousezeiger = false;
            break;
        }
    }
    //Ende der Auswertung
    if(fullscreen)
    {
        QProcess::startDetached("/home/florian/Desktop/hide_panel.sh");
    }
    hsGui w(&ip, port, fullscreen, mousezeiger);
    if(fullscreen) {w.setWindowState(Qt::WindowFullScreen);}
    if(!mousezeiger) {w.setCursor(Qt::BlankCursor);}
    w.show();

    return a.exec();
}
