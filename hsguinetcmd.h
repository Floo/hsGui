#ifndef HSGUINETCMD_H
#define HSGUINETCMD_H

//Messagetypen definieren
#define NO_CMD 0
#define CMD_JAL 1
#define CMD_CMD 2
#define CMD_FLOOR 3
#define CMD_ABLUFT 4
#define CMD_HWR 5
#define CMD_LICHT 6
#define CMD_SZENE 7
#define CMD_WETT 8
#define CMD_HELL 9
#define CMD_GET_HSINFO 10 //fordert den Shared_Memory an, nur beim ersten Mal
//danach wird Shared-Memory nach jeder Aenderung geschickt
#define CMD_GET_CONFIG 11 //fordert die Config-Files an
#define CMD_SEND_HSINFO 12 //wie nur in der Hausteuerung verwendet
#define CMD_SET_CONFIG 13 //ein Wert in haussteuerung.conf setzen "key=section:key value=wert"
#define CMD_DIMM 14 //Dimm-Befehl fuer eine Lampe Format: value=essen:DIMM_UP_START
#define CMD_LICHT_RESET_TIMER 15 //loescht alle Timer im Dimmer
#define CMD_E6_INIT 16 //E6-Device muss neu initialisiert werden
#define CMD_WATER 17 //value=VENTIL_1_START; value=VENTIL_2_STOP; value=VENTIL_1_TIMER; value=VENTIL_2_TIMER:10 (Dauer in min)

#define VENTIL_1 0
#define VENTIL_2 1

#define DIMM_UP_START 1
#define DIMM_UP_STOP 2
#define DIMM_DOWN_START 3
#define DIMM_DOWN_STOP 4

#define CONFIG_HS 1 //haussteuerung.conf soll gesendet werden
#define CONFIG_LIGHT 2 //hslight.conf soll gesendet werden
#define LOGFILE 3 //haussteuerung.log soll gesendet werden, nur erste 100 Zeilen
#define LOGFILE_INVERS 4 //haussteuerung.log soll gesendet werden, letzte 100 Zeilen rückwärts

#define MAIN_IS_CONNECTED 1
#define HWR_IS_CONNECTED 2
#define TERRASSE_IS_CONNECTED 4


// FS20-Befehle
#define FS20_AN 0x11
    // "an, alter Wert"
#define FS20_DIMM_UP 0x13
    // "Dim Up"
#define FS20_DIMM_DOWN 0x14
    // "Dim Down"
#define FS20_AN_100 0x10
    // "100% an"
#define FS20_AN_93 0x0F
    // "93.75% an"
#define FS20_AN_87 0x0E
    // "87.5% an"
#define FS20_AN_81 0x0D
    // "81.25% an"
#define FS20_AN_75 0x0C
    // "75% an"
#define FS20_AN_68 0x0B
    // "68.75% an"
#define FS20_AN_62 0x0A
    // "62.5% an"
#define FS20_AN_56 0x09
    // "56.25% an"
#define FS20_AN_50 0x08
    // "50% an"
#define FS20_AN_43 0x07
    // "43.75% an"
#define FS20_AN_37 0x06
    // "37.5% an"
#define FS20_AN_31 0x05
    // "31.25% an"
#define FS20_AN_25 0x04
    // "25% an"
#define FS20_AN_18 0x03
    // "18.75% an"
#define FS20_AN_12 0x02
    // "12.5% an"
#define FS20_AN_06 0x01
    // "6.25% an"
#define FS20_AUS 0x00
    // "Aus"
#define FS20_TOGGLE 0x12
    // "Toggle"
#define FS20_RESET 0x1B
    // "Reset"
#define FS20_DEL_OFF_TIMER 0x3D
    // "Delete Off-Timer" 61d
#define FS20_DEL_ON_TIMER 0x3C
    // "Delete On-Timer" 60d


#endif // HSGUINETCMD_H
