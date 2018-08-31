/****************************************************************************
**
**   Copyright © 2017 The KTS-Intek Ltd.
**   Contact: http://www.kts-intek.com.ua
**   bohdan@kts-intek.com.ua
**
**  This file is part of matilda-bbb.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

#include "settloader4matilda.h"

#include <QSettings>
#include <QDir>

#include <QStringList>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QtCore>
#include <QStandardPaths>

#include "moji_defy.h"

#ifdef HASSERIALLIB
#include <QSerialPort>
#include <QSerialPortInfo>
#endif


//----------------------------------------------------------------------------------------------------------------
SettLoader4matilda::SettLoader4matilda(QObject *parent) : QObject(parent)
{

}
//----------------------------------------------------------------------------------------------------------------
QHash<int, QVariant> SettLoader4matilda::loadAllSett()
{
    QHash<int,QVariant> hashSett;
    QList<int> listKeys;
    listKeys << SETT_ZIGBEE_PORT << SETT_ZIGBEE_PORT_SPEED << SETT_ZIGBEE_READ_TO << SETT_ZIGBEE_READ_TOB << SETT_ZIGBEE_AS_DATA
             << SETT_TCP_READ_TO << SETT_TCP_READ_TOB

             << SETT_WHITE_IP_LIST << SETT_BLACK_IP_LIST << SETT_TCP_PORT << SETT_TCP_HOST_ADDR << SETT_ENABLE_DA << SETT_PAC_LIST << SETT_PAC_DAY_PROFILE << SETT_PEREDAVATOR_MODE << SETT_MAGIC_WORD << SETT_DBG_PORT;

    for(int i = 0, iMax = listKeys.size(); i < iMax; i++)
        hashSett.insert(listKeys.at(i), loadOneSett(listKeys.at(i)));

    return hashSett;
}
//----------------------------------------------------------------------------------------------------------------
QHash<int, QVariant> SettLoader4matilda::loadAllSettPeredavator()
{
    QHash<int,QVariant> hashSett;
    QList<int> listKeys;
    listKeys << SETT_ZIGBEE_PORT << SETT_ZIGBEE_PORT_SPEED << SETT_ZIGBEE_READ_TO << SETT_ZIGBEE_READ_TOB << SETT_ZIGBEE_AS_DATA
             << SETT_TCP_READ_TO << SETT_TCP_READ_TOB

             << SETT_WHITE_IP_LIST << SETT_BLACK_IP_LIST << SETT_TCP_PORT << SETT_TCP_HOST_ADDR << SETT_ENABLE_DA << SETT_PAC_LIST << SETT_PAC_DAY_PROFILE << SETT_PEREDAVATOR_MODE << SETT_MAGIC_WORD << SETT_DBG_PORT;

    for(int i = 0, iMax = listKeys.size(); i < iMax; i++)
        hashSett.insert(listKeys.at(i), loadOneSett(listKeys.at(i)));

    return hashSett;
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::groupName4key(const int &key)
{
    //порівнювати з більшого значення до меншого
    if(key >= SETT_SNTP_SERVICE)
        return "main-sntp";

    if(key >= SETT_FIREFLY_SERVERPORT)
        return "main-firefly";

    if(key >= SETT_FORWARD_TABLE)
        return "main-forward";

    if(key >= SETT_WHITE_IP_LIST)
        return "matilda-server";

    if(key >= SETT_MAC_ENABLE)
        return  "matilda-active-client";

    if(key >= SETT_SOME_SETT)
        return "SlavaUkrajini";

    if(key >= SETT_ABOUT_MEMO)
        return "about";

    if(key >= SETT_GPRS_PORT_NAME)
        return "main-gprs";


    if(key >= SETT_POLL_FRWRD_TBL_4_PLL)
        return "poll";


    if(key >= SETT_TCP_READ_TO)
        return "main-tcp";



    if(key > 0 && key < SETT_TCP_READ_TO)
        return "main-zigbee";

    return "";
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::valueName4key(const int &key)
{
    QString valKey = QString();
    switch(key){
//peredavator  "main-zigbee"
    case SETT_ZIGBEE_PORT               : valKey = "portName"                   ; break;
    case SETT_ZIGBEE_PORT_SPEED         : valKey = "baudRate"                   ; break;
    case SETT_ZIGBEE_READ_TO            : valKey = "rTo"                        ; break;
    case SETT_ZIGBEE_READ_TOB           : valKey = "rToB"                       ; break;
    case SETT_ZIGBEE_AS_DATA            : valKey = "asData"                     ; break;

//tcp  "main-tcp"
    case SETT_TCP_READ_TO               : valKey = "tcpRT"                      ; break;
    case SETT_TCP_READ_TOB              : valKey = "tcpRTB"                     ; break;
    case SETT_TCP_PPPD_FIRST            : valKey = "pppdFirst"                  ; break;
    case SETT_TCP_MNOZNYK               : valKey = "mnznk"                      ; break;

        //poll
    case SETT_POLL_FRWRD_TBL_4_PLL      : valKey = "frwrd"                      ; break;
    case SETT_POLL_METER_RETRY          : valKey = "meterRetry"                 ; break;
    case SETT_POLL_METER_RETRY_FA       : valKey = "meterRetryFA"               ; break;
    case SETT_POLL_HARD_ADDRS           : valKey = "hardAddrs"                  ; break;
    case SETT_POLL_WAIT4EMB             : valKey = "wait4emb"                   ; break;
    case SETT_POLL_WAIT4EMB_RTRBFR      : valKey = "w4eRtrBfr"                  ; break;
    case SETT_POLL_WAIT4EMB_RTRAFT      : valKey = "w4eRtrAft"                  ; break;
    case SETT_POLL_TIME_CORR_ENBL       : valKey = "enTimeCorr"                 ; break;
    case SETT_POLL_DIFF_TIME_CORR       : valKey = "diffTimeCorr"               ; break;
    case SETT_POLL_WAIT4POLL            : valKey = "wait4poll"                  ; break;
    case SETT_POLL_AUTO_FRWRD_TBL       : valKey = "af"                         ; break;

        //main-gprs
    case SETT_GPRS_PORT_NAME            : valKey = "portName"                   ; break;
    case SETT_GPRS_BAUD_RATE            : valKey = "baudRate"                   ; break;
    case SETT_GPRS_APN                  : valKey = "apn"                        ; break;
    case SETT_GPRS_USERNAME             : valKey = "userName"                   ; break;
    case SETT_GPRS_PASSWORD             : valKey = "password"                   ; break;
    case SETT_GPRS_NUMBER               : valKey = "nmbr"                       ; break;
    case SETT_GPRS_PREFF_2G_3G          : valKey = "pref2g3g"                   ; break;

        //about
    case SETT_ABOUT_MEMO                : valKey = "memo"                       ; break;
    case SETT_ABOUT_COORDNTS            : valKey = "coorditantes"               ; break;
    case SETT_ABOUT_MEMOLINE            : valKey = "memoLine"                   ; break;


        //SlavaUkrajini
    case SETT_SOME_SETT                 : valKey = "HerojamSlava"               ; break;
    case SETT_SVAHA_SYNC_ENABLE         : valKey = "svaha-sync-en"              ; break;

        //"matilda-active-client"
    case SETT_MAC_ENABLE                : valKey = "enableMAC"                  ; break;
    case SETT_MAC_SVAHA_LIST            : valKey = "svaha-list"                 ; break;
    case SETT_MAC_DAY_PROFILE           : valKey = "svaha-day-profile"          ; break;

        //matilda vs peredavator   "matilda-server"
    case SETT_WHITE_IP_LIST             : valKey = "blockThisIp"                ; break;
    case SETT_BLACK_IP_LIST             : valKey = "whiteIpList"                ; break;
    case SETT_TCP_PORT                  : valKey = "peredavatorPort"            ; break;
    case SETT_TCP_HOST_ADDR             : valKey = "peredavatorName"            ; break;
    case SETT_ENABLE_DA                 : valKey = "enablePAC"                  ; break;
    case SETT_PAC_LIST                  : valKey = "peredavator-list"           ; break;
    case SETT_PAC_DAY_PROFILE           : valKey = "peredavator-day-profile"    ; break;
    case SETT_PEREDAVATOR_MODE          : valKey = "mode"                       ; break;
    case SETT_MAGIC_WORD                : valKey = "magic"                      ; break;
    case SETT_MAGIC_WORDH               : valKey = "magicH"                     ; break;

    case SETT_DBG_PORT                  : valKey = "uartPort"                   ; break;
    case SETT_MSERVER_PORT              : valKey = "matildaPort"                ; break;
    case SETT_UDP_BEACON_ENBL           : valKey = "udpBeacon"                  ; break;

        ///protocol v2
    case SETT_IP_ROUTE_TABLE            : valKey = "ipRoute"                    ; break;
    case SETT_PPP_SUPERVISOR            : valKey = "pppSupervisor"              ; break;



        //"main-forward"
    case SETT_FORWARD_TABLE             : valKey = "forward"                    ; break;


    case SETT_FIREFLY_SERVERPORT        : valKey = "server-port"                ; break;
    case SETT_FIREFLY_ACCESS_IP         : valKey = "access-ip"                  ; break;
    case SETT_FIREFLY_REMOTE_HOST       : valKey = "remote-hosts"               ; break;
    case SETT_FIREFLY_MODE              : valKey = "fireflyMode"                ; break;
    case SETT_FIREFLY_REALAY            : valKey = "fireflyRelay"               ; break;

    case SETT_SNTP_SERVICE              : valKey = "balans"                     ; break;

    default:                                                                           break;

    }
    return valKey;
}
//----------------------------------------------------------------------------------------------------------------
QVariant SettLoader4matilda::defValue4key(const int &key)
{
    QVariant defVal = QVariant();
    switch(key){
//peredavator  "main-zigbee"
    case SETT_ZIGBEE_PORT           : defVal = defUartName()            ; break;
    case SETT_ZIGBEE_PORT_SPEED     : defVal = defUartSpeed()           ; break;
    case SETT_ZIGBEE_READ_TO        : defVal = defUartReadTO()          ; break;
    case SETT_ZIGBEE_READ_TOB       : defVal = defUartReadTOB()         ; break;
    case SETT_ZIGBEE_AS_DATA        : defVal = defAsData()              ; break;

//tcp  "main-tcp"
    case SETT_TCP_READ_TO           : defVal = defTcpReadTO()           ; break;
    case SETT_TCP_READ_TOB          : defVal = defTcpReadTOB()          ; break;
    case SETT_TCP_PPPD_FIRST        : defVal = defTcpReadTOB()          ; break;
    case SETT_TCP_MNOZNYK           : defVal = defTcpMnznk()            ; break;


        //main-gprs
    case SETT_GPRS_PORT_NAME        : defVal = defGprsUartName()        ; break;
    case SETT_GPRS_BAUD_RATE        : defVal = defGprsUartSpeed()       ; break;
    case SETT_GPRS_APN              : defVal = defGprsApn()             ; break;
    case SETT_GPRS_USERNAME         : defVal = defGprsUserName()        ; break;
    case SETT_GPRS_PASSWORD         : defVal = defGprsPasswrd()         ; break;
    case SETT_GPRS_NUMBER           : defVal = defGprsNmbr()            ; break;
    case SETT_GPRS_PREFF_2G_3G      : defVal = defPref2g3g()            ; break;


        //about
    case SETT_ABOUT_MEMO            : defVal = defAboutMemo()           ; break;
    case SETT_ABOUT_COORDNTS        : defVal = defAboutCrdnts()         ; break;
    case SETT_ABOUT_MEMOLINE        : defVal = defAboutMemoLine()       ; break;

        //SlavaUkrajini
    case SETT_SOME_SETT             : defVal = defSomeSett()            ; break;
    case SETT_SVAHA_SYNC_ENABLE     : defVal = true                     ; break;

        //poll
    case SETT_POLL_FRWRD_TBL_4_PLL  : defVal = defPollFrwrd()           ; break;
    case SETT_POLL_METER_RETRY      : defVal = defPollMeterRetry()      ; break;
    case SETT_POLL_METER_RETRY_FA   : defVal = defPollMeterRetryFA()    ; break;
    case SETT_POLL_HARD_ADDRS       : defVal = defPollHardAddrs()       ; break;
    case SETT_POLL_WAIT4EMB         : defVal = defPollW4Emb()           ; break;
    case SETT_POLL_WAIT4EMB_RTRBFR  : defVal = defPollW4EmbRtrBfr()     ; break;
    case SETT_POLL_WAIT4EMB_RTRAFT  : defVal = defPollW4EmbRtrAft()     ; break;
    case SETT_POLL_TIME_CORR_ENBL   : defVal = defPollTimeCorrEn()      ; break;
    case SETT_POLL_DIFF_TIME_CORR   : defVal = defPollDiffTimeCorr()    ; break;
    case SETT_POLL_WAIT4POLL        : defVal = defPollWait4poll()       ; break;
    case SETT_POLL_AUTO_FRWRD_TBL   : defVal = defAdd2ForwardTable()    ; break;

//"matilda-active-client"
    case SETT_MAC_ENABLE            : defVal = defMacEnable()           ; break;
    case SETT_MAC_SVAHA_LIST        : defVal = defMacSvahaList()        ; break;
    case SETT_MAC_DAY_PROFILE       : defVal = defDayProfile()          ; break;


        //matilda vs peredavator   "matilda-server"
    case SETT_WHITE_IP_LIST         : defVal = defBlackIpList()         ; break;
    case SETT_BLACK_IP_LIST         : defVal = defWhiteIpList()         ; break;
    case SETT_TCP_PORT              : defVal = defServerPort()          ; break;
    case SETT_TCP_HOST_ADDR         : defVal = defServerName()          ; break;
    case SETT_ENABLE_DA             : defVal = defEnDA()                ; break;
    case SETT_PAC_LIST              : defVal = defPacList()             ; break;
    case SETT_PAC_DAY_PROFILE       : defVal = defDayProfile()          ; break;
    case SETT_PEREDAVATOR_MODE      : defVal = defPeredavatorMode()     ; break;
    case SETT_MAGIC_WORD            : defVal = defMagicSequence()       ; break;
    case SETT_MAGIC_WORDH           : defVal = defMagicInHex()          ; break;

    case SETT_DBG_PORT              : defVal = defDbgUargPort()         ; break;
    case SETT_MSERVER_PORT          : defVal = defMServerPort()         ; break;
    case SETT_UDP_BEACON_ENBL       : defVal = defUdpBeacon()           ; break;

        ///protocol v2
    case SETT_IP_ROUTE_TABLE        : defVal = defIpRoute()             ; break;
    case SETT_PPP_SUPERVISOR        : defVal = defPppSupervisor()       ; break;


        //"main-forward"
    case SETT_FORWARD_TABLE         : defVal = defForwardTbl()          ; break;

        //main-firefly
    case SETT_FIREFLY_SERVERPORT    : defVal = defFFtcpPort()           ; break;
    case SETT_FIREFLY_ACCESS_IP     : defVal = defFFallowAccess4thisIps(); break;
    case SETT_FIREFLY_REMOTE_HOST   : defVal = defFFremoteServer()      ; break;
    case SETT_FIREFLY_MODE          : defVal = defFFmode()              ; break;
    case SETT_FIREFLY_REALAY        : defVal = defFFrealaySett()        ; break;


    case SETT_SNTP_SERVICE          : defVal = defSntpBalans()          ; break;

    default: break;

    }
    return defVal;
}
//----------------------------------------------------------------------------------------------------------------
QVariant SettLoader4matilda::loadOneSett(const int key)
{

#ifdef VERSION_4_PC
    QSettings settings("kts-intek", "matilda");
#else
    QSettings settings(path2sett(key), QSettings::NativeFormat);
    if(settings.fileName().isEmpty()){
        qDebug() << "sett is empty " << settings.fileName();
        return false;
    }
#endif

    QString valName = valueName4key(key);

    QVariant value;
    if(!valName.isEmpty()){

        settings.beginGroup(groupName4key(key));
        value = settings.value(valName, defValue4key(key));
        settings.endGroup();
    }
    return value;
}
//----------------------------------------------------------------------------------------------------------------
bool SettLoader4matilda::saveOneSett(const int key, const QVariant data2save)
{
#ifdef VERSION_4_PC
    QSettings settings("kts-intek", "matilda");
#else
    QSettings settings(path2sett(key), QSettings::NativeFormat);
    if(settings.fileName().isEmpty()){
        qDebug() << "sett is empty " << settings.fileName();
        return false;
    }
#endif

    QString valKey = valueName4key(key);

    QString groupName = groupName4key(key);
    if(valKey.isEmpty() || groupName.isEmpty()){
        qDebug() << "saveOneSett = " << key << valKey << groupName << data2save;
        return false;
    }
    settings.beginGroup(groupName);
    settings.setValue(valKey , data2save);
    settings.endGroup();
    return true;

}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::path2sett(const int key)
{
    QDir dir(qApp->applicationDirPath());
    QString path2sett ;

    if(dir.cdUp()){
        dir.setPath(QString("%1/config").arg(dir.path()));
        path2sett = dir.path();
        if(!dir.exists())
            dir.mkpath(path2sett);

        path2sett.append( (key >= SETT_WHITE_IP_LIST) ? QString("/matilda") : QString("/peredavator"));

    }

    if(path2sett.isEmpty())
        path2sett = "/dev/null";

    return path2sett;
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::getRealPortName(const QString &portName, bool &isUsb)
{
#ifdef HASSERIALLIB
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        if(portName == info.portName()){
            qDebug() << info.serialNumber() << info.description() << info.manufacturer() << info.hasProductIdentifier() << info.hasVendorIdentifier();
            isUsb = (info.hasProductIdentifier() || portName.contains("ttyUSB") || portName.contains("ttyACM"));
            return portName;
        }
    }


    QDir dir(QString("%1/lnk2uart").arg(SettLoader4matilda::path2workDir()));
    if(dir.exists()){

        QFileInfoList listFi = dir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot);
        for(int i = 0, iMax = listFi.size(); i < iMax; i++){


            if((listFi.at(i).isSymLink() || listFi.at(i).isFile())){

                if(listFi.at(i).fileName() == portName || listFi.at(i).absolutePath() == portName){
                    QString realPortName = listFi.at(i).absoluteFilePath();
                    if(listFi.at(i).isSymLink())
                        realPortName = listFi.at(i).symLinkTarget();

                    QSerialPort port(realPortName);
                    QSerialPortInfo info(port.portName());
                    QFile f(realPortName);
                    if(f.exists()){
                        isUsb = (info.hasProductIdentifier() || portName.contains("ttyUSB") || portName.contains("ttyACM"));
                        return port.portName();
                    }

                }

            }
        }
    }

#else
    Q_UNUSED(portName);
    Q_UNUSED(isUsb);
    qDebug() << "serialport library not loaded";
#endif
    return "";
}
//----------------------------------------------------------------------------------------------------------------
bool SettLoader4matilda::createSymlinkForced(const QString &linkPath, const QString &targetPath)
{
    qDebug() << "createSymlinkForced " << linkPath << targetPath;
    QProcess proc;
    proc.start("bash");
    if(!proc.waitForStarted(2000))
        return false;

    proc.write("ln -sf \"" + QVariant(linkPath).toByteArray() + "\" \"" + QVariant(targetPath).toByteArray() + "\" && exit\n");//create symlint
    proc.waitForReadyRead(200);
    QByteArray arr = proc.readAll();
    proc.waitForReadyRead(60);
    arr.append(proc.readAll());
    proc.close();
    return true;
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::path2backups()
{
    QDir dir(qApp->applicationDirPath());
    QString path2sett("");

    if(dir.cdUp()){
        path2sett = dir.path() + "/backup";

        if(!dir.exists())
            dir.mkpath(path2sett);
    }
    if(path2sett.isEmpty())
        path2sett = "/dev/null";
    return path2sett;
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::path2config()
{
#ifdef VERSION_4_PC
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#else
    QDir dir(qApp->applicationDirPath());
    QString path2sett ;

    if(dir.cdUp()){
        dir.setPath(QString("%1/config").arg(dir.path()));
        path2sett = dir.path();
        if(!dir.exists())
            dir.mkpath(path2sett);

    }
    if(path2sett.isEmpty())
        path2sett = "/dev/null";
    return path2sett;
#endif
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::path2script()
{
    QString path2dir = path2workDir();
    path2dir.append("/script");
    QDir dir(path2dir);
    if(!dir.exists())
        dir.mkpath(path2dir);

    if(path2dir.isEmpty())
        path2dir = "/dev/null";

    return path2dir;
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::path2tmpDir()
{
    QString path2dir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    path2dir.append("/matilda");
    QDir dir(path2dir);
    if(!dir.exists())
        dir.mkpath(path2dir);

    if(path2dir.isEmpty())
        path2dir = "/dev/null";

    return path2dir;
}
//----------------------------------------------------------------------------------------------------------------

QString SettLoader4matilda::path2chatGprs() { return QString("/etc/ppp/chat/matilda") ; }

QString SettLoader4matilda::path2peersGprs(){ return QString("/etc/ppp/peers/matilda"); }

//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::path2database()
{
    QString path2dir = path2workDir();
    path2dir.append("/sqlite");
    QDir dir(path2dir);
    if(!dir.exists())
        dir.mkpath(path2dir);

    if(path2dir.isEmpty())
        path2dir = "/dev/null";
    else
        path2dir.append("/zbyrator.db");

    return path2dir;
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::path2workDir()
{
    QDir dir(qApp->applicationDirPath());
    QString path2sett("");

    if(dir.cdUp()){
        path2sett = dir.path();
        if(!dir.exists())
            dir.mkpath(path2sett);

    }
    if(path2sett.isEmpty())
        path2sett = "/dev/null";
    return path2sett;
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::path2myDir(const QString &myPath)
{
    QDir dir(qApp->applicationDirPath());
    QString path2sett("");

    if(dir.cdUp()){
        path2sett = dir.path();
        if(!dir.exists())
            dir.mkpath(path2sett);
        path2sett.append("/" + myPath);

    }
    if(path2sett.isEmpty())
        path2sett = "/dev/null";
    return path2sett;
}

//----------------------------------------------------------------------------------------------------------------

QString SettLoader4matilda::defLocalServerName()        { return QString("kts-intek.com.ua.matilda-bbb")                        ; }

QString SettLoader4matilda::defUdpLocalServerName()     { return QString("ua.ktsintek.matilda")                                 ; }

QString SettLoader4matilda::defSemaName()               { return QString("kts-intek.com.ua/sema")                               ; }

QString SettLoader4matilda::defSharedMemoName()         { return QString("kts-intek.com.ua/shared_memory")                      ; }

QString SettLoader4matilda::defProcManagerMemoName()    { return QString("%1/matilda/proc-manager").arg(defSharedMemoName())    ; }

QString SettLoader4matilda::defProcManagerSemaName()    { return QString("%1/matilda/proc-manager").arg(defSemaName())          ; }

QString SettLoader4matilda::defMatildaServerMemoName()  { return QString("%1/matilda/matilda-server").arg(defSharedMemoName())  ; }

QString SettLoader4matilda::defMatildaServerSemaName()  { return QString("%1/matilda/matilda-server").arg(defSemaName())        ; }


QString SettLoader4matilda::defPeredavatorMemoName()    { return QString("%1/peredavator").arg(defSharedMemoName())             ; }

QString SettLoader4matilda::defZbyratorMemoName()       { return QString("%1/zbyrator").arg(defSharedMemoName())                ; }

QString SettLoader4matilda::defZbyratorCounterMemoName(){ return QString("%1/zbyrator_counter").arg(defSharedMemoName())        ; }

QString SettLoader4matilda::defZbyratorQuickPollName() {  return QString("%1/zbyrator_quick_poll").arg(defSharedMemoName())     ; }


QString SettLoader4matilda::defPeredavatorSemaName()    { return QString("%1/peredavator").arg(defSemaName())                   ; }

QString SettLoader4matilda::defZbyratorSemaName()       { return QString("%1/zbyrator").arg(defSemaName())                      ; }

QString SettLoader4matilda::defZbyratorCounterSemaName(){ return QString("%1/zbyrator_counter").arg(defSemaName())              ; }

QString SettLoader4matilda::defZbyratorQuickPollSemaName(){return QString("%1/zbyrator_quick_poll").arg(defSemaName())          ; }

QString SettLoader4matilda::defAboutGsmModemMemoName()  { return QString("%1/about-gsm").arg(defSemaName())                     ; }

QString SettLoader4matilda::defAboutGsmModemSemaName()  { return QString("%1/about-gsm").arg(defSharedMemoName())               ; }

QString SettLoader4matilda::defAboutZbModemMemoName()   { return QString("%1/about-zb").arg(defSemaName())                      ; }

QString SettLoader4matilda::defAboutZbModemSemaName()   { return QString("%1/about-zb").arg(defSharedMemoName())                ; }


QString SettLoader4matilda::defSntpServerMemoName()     { return QString("%1/sntp-bbb").arg(defSemaName())                      ; }

QString SettLoader4matilda::defSntpServerSemaName()     { return QString("%1/sntp-bbb").arg(defSharedMemoName())                ; }


QString SettLoader4matilda::defSvahaServerMemoName()    { return QString("%1/svaha-bbb").arg(defSemaName())                     ; }

QString SettLoader4matilda::defSvahaServerSemaName()    { return QString("%1/svaha-bbb").arg(defSharedMemoName())               ; }



QString SettLoader4matilda::showNormalIP(const QHostAddress &hAddr) { return showNormalIP(hAddr.toString())                                          ; }

QString SettLoader4matilda::showNormalIP(const QString &ip)         { return (ip.left(7).toLower() == "::ffff:" && ip.contains(".")) ? ip.mid(7) : ip; }//Qt5 bug(((

QString SettLoader4matilda::defUartName()                           { return lnk2uartChecker("/dev/ttyS1", defLnkNameZigBee())                       ; }


qint32 SettLoader4matilda::defUartSpeed()  { return 9600    ; }

qint32 SettLoader4matilda::defUartReadTO() { return 900     ; }

qint32 SettLoader4matilda::defUartReadTOB(){ return 300     ; }


bool SettLoader4matilda::defAsData(){ return true; }


QString SettLoader4matilda::defLnkNameZigBee(){ return ("default-zigbee"); }

//----------------------------------------------------------------------------------------------------------------

QList<int> SettLoader4matilda::supportedUartSpeed()
{
    QList<qint32> listEmbSpeed;
    listEmbSpeed << 9600 << 2400 << 115200 << 57600 << 38400 << 19200 << 4800 << 1200;//EMB 250
#ifdef HASSERIALLIB
    QList<qint32> listPlatform = QSerialPortInfo::standardBaudRates();
#else
    QList<qint32> listPlatform = listEmbSpeed;
    qDebug() << "serialport library not loaded";
#endif
    QList<int> retList;
    for(int i = 0, iMax = listEmbSpeed.size(); i < iMax; i++){
        if(listPlatform.contains(listEmbSpeed.at(i)))
            retList.append(listEmbSpeed.at(i));
    }

    return retList ;
}
//----------------------------------------------------------------------------------------------------------------
QStringList SettLoader4matilda::supportedUartSpeedSL()
{
    QList<int> li = supportedUartSpeed();
    QStringList l;
    for(int i = 0, iMax = li.size(); i < iMax; i++)
        l.append(QString::number(li.at(i)));
    return l;
}
//----------------------------------------------------------------------------------------------------------------

qint32 SettLoader4matilda::defTcpReadTO()       { return 1000                   ; }

qint32 SettLoader4matilda::defTcpReadTOB()      { return 200                    ; }

bool SettLoader4matilda::defTcpPPPDfirst()      { return true                   ; }

qint32 SettLoader4matilda::defTcpMnznk()        { return 23                     ; }

bool SettLoader4matilda::defPollFrwrd()         { return false                  ; }

qint32 SettLoader4matilda::defPollMeterRetry()  { return 3                      ; }

qint32 SettLoader4matilda::defPollMeterRetryFA(){ return 2                      ; }

bool SettLoader4matilda::defPollHardAddrs()     { return false                  ; }

bool SettLoader4matilda::defPollW4Emb()         { return true                   ; }

qint32 SettLoader4matilda::defPollW4EmbRtrBfr() { return 11                     ; }

qint32 SettLoader4matilda::defPollW4EmbRtrAft() { return 55                     ; }

bool SettLoader4matilda::defPollTimeCorrEn()    { return true                   ; }

qint32 SettLoader4matilda::defPollDiffTimeCorr(){ return SETT_DT_DIFF_MAX_SECS  ; }

qint32 SettLoader4matilda::defPollWait4poll()   { return 33                     ; }

bool SettLoader4matilda::defAdd2ForwardTable()  { return true                   ; }

bool SettLoader4matilda::defMacEnable()         { return true                   ; }

//----------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader4matilda::defMacSvahaList()
{
    QStringList listHosts = QString("svaha.ddns.net:65000 svaha2.ddns.net:65000").split(" ");
    QString dayProf = "All day";

    QVariantHash hash;

    for(int i = 0, iMax = listHosts.size(); i < iMax; i++)
        hash.insert(listHosts.at(i), dayProf);

    return hash;
}

//----------------------------------------------------------------------------------------------------------------

QString SettLoader4matilda::defGprsUartName()   { return lnk2uartChecker(QString("/dev/ttyS4"), defLnkNameGsm()) ; }

qint32 SettLoader4matilda::defGprsUartSpeed()   { return 921600                 ; }

QString SettLoader4matilda::defGprsApn()        { return QString("")            ; }

QString SettLoader4matilda::defGprsUserName()   { return QString("")            ; }

QString SettLoader4matilda::defGprsPasswrd()    { return QString("")            ; }

QString SettLoader4matilda::defGprsNmbr()       { return QString("*99***1#")    ; }

int SettLoader4matilda::defPref2g3g()           { return 4                      ; } //3g preffered for HL85

QString SettLoader4matilda::defLnkNameGsm()     { return ("default-gsm")        ; }

QString SettLoader4matilda::defAboutMemo()      { return QString("")            ; }

QString SettLoader4matilda::defAboutCrdnts()    { return QString("")            ; }

QString SettLoader4matilda::defAboutMemoLine()  { return  QString("matilda")    ; }

QVariantHash SettLoader4matilda::defSomeSett()  { return QVariantHash()         ; }

QStringList SettLoader4matilda::defWhiteIpList(){ return QStringList()          ; }

QStringList SettLoader4matilda::defBlackIpList(){ return QStringList()          ; }

quint16 SettLoader4matilda::defServerPort()     { return SETT_DEFAULT_PEREDAVPORT;}

QString SettLoader4matilda::defServerName()     { return QString("0.0.0.0")     ; }

bool SettLoader4matilda::defEnDA()              { return true                   ; }

//----------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader4matilda::defPacList()
{
    QStringList listHosts = QStringList();// = QString("svaha.ddns.net:65000 svaha2.ddns.net:65000").split(" ");//192.168.88.202:65000").split(" ");
    QString dayProf = "All day";

    QVariantHash hash;

    for(int i = 0, iMax = listHosts.size(); i < iMax; i++)
        hash.insert(listHosts.at(i), dayProf);

    return hash;

}
//----------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader4matilda::defDayProfile()
{
    QVariantHash hash;
    hash.insert("All day", "[00:00]:[23:59]");
    return hash;
}

//----------------------------------------------------------------------------------------------------------------

int SettLoader4matilda::defPeredavatorMode()    { return DA_MODE_ON             ; }

QString SettLoader4matilda::defMagicSequence()  { return QString("hello")       ; }

bool SettLoader4matilda::defMagicInHex()        { return false                  ; }

quint16 SettLoader4matilda::defDbgUargPort()    { return SETT_DEFAULT_DBGUARTPORT;}

quint16 SettLoader4matilda::defMServerPort()    { return SETT_DEFAULT_MATILDAPORT;}

bool SettLoader4matilda::defUdpBeacon()         { return true                   ; }


//----------------------------------------------------------------------------------------------------------------

///protocol v2 start

QVariantHash SettLoader4matilda::defIpRoute()   { return QVariantHash()         ; }

//----------------------------------------------------------------------------------------------------------------

QVariantHash SettLoader4matilda::defPppSupervisor()
{
    QVariantHash h;
    h.insert("iplist", QStringList());
    h.insert("loc"  , DEF_VAL_LIFE_OF_CONNECTION);
    h.insert("ping" , DEF_VAL_PING_INTERVAL);
    h.insert("lcpi" , DEF_VAL_LCP_INTERVAL);
    h.insert("lcpe" , DEF_VAL_LCP_FAIL);
    h.insert("pingf", DEF_VAL_PING_FAIL);
    h.insert("ide"  , true);
    h.insert("dns"  , true);
    h.insert("ntp"  , true);
    h.insert("mtd"  , true);
    h.insert("da"   , true);
    return h;
}

//----------------------------------------------------------------------------------------------------------------

///led lamp
QString SettLoader4matilda::defFireflyLedListMemoName() { return QString("%1/fireflyList").arg(defSharedMemoName())    ; }

QString SettLoader4matilda::defFireflyLedListSemaName() { return QString("%1/fireflyList").arg(defSemaName())          ; }

QString SettLoader4matilda::defFireflyScheduleMemoName(){ return QString("%1/fireflySchedule").arg(defSharedMemoName()); }

QString SettLoader4matilda::defFireflyScheduleSemaName(){ return QString("%1/fireflySchedule").arg(defSemaName())      ; }

QString SettLoader4matilda::defFireflyTaskMemoName()    { return QString("%1/fireflyTask").arg(defSharedMemoName())    ; }

QString SettLoader4matilda::defFireflyTaskSemaName()    { return QString("%1/fireflyTask").arg(defSemaName())          ; }

QString SettLoader4matilda::defFireflyRelayMemoName()   { return QString("%1/fireflyRelay").arg(defSharedMemoName())   ; }

QString SettLoader4matilda::defFireflyRelaySemaName()   { return QString("%1/fireflyRelay").arg(defSemaName())         ; }

QString SettLoader4matilda::defFireflyStatusMemoName()  { return QString("%1/fireflyStatus").arg(defSharedMemoName())   ; }

QString SettLoader4matilda::defFireflyStatusSemaName()  { return QString("%1/fireflyStatus").arg(defSemaName())         ; }

QString SettLoader4matilda::defFFremoteServer()         { return QString("")                      ; }

quint16 SettLoader4matilda::defFFtcpPort()              { return (quint16)SETT_DEFAULT_FIREFLYPORT                     ; }

QStringList SettLoader4matilda::defFFallowAccess4thisIps(){return QString("").split(" ")                 ; }

quint16 SettLoader4matilda::defDbgFireflyPort()         { return (quint16)SETT_DEFAULT_DBGFFLYPORT                     ; }

//----------------------------------------------------------------------------------------------------------------

QVariantHash SettLoader4matilda::defFFmode()
{
    QVariantHash h;
    h.insert("enRemote", false);//використовувати віддалений серве або працювати автономно
    h.insert("remIp", "");//Адреса віддаленого серверу
    h.insert("remPort", 80);//Порт віддаленого серверу
    h.insert("pwrIntrvl", 10);//інтервал роздачі потужності по групам, хв
    h.insert("pollTime", 15);//період через який необхідно почати опитувати стан ламп, хв
    return h;
}

//----------------------------------------------------------------------------------------------------------------

QVariantList SettLoader4matilda::defFFrealaySett()
{
    return QVariantList();
//    QVariantHash h;
//    h.insert("type", "");//Тип <не використовувати ---><лічильник MTX>, <БУС>, <порт1>
//    h.insert("adr", "");//Адреса лічильника, або БУСа або порта
//    h.insert("asg", false);//пов'язати вимкнення реле з графіком групи
//    h.insert("asgg", -1);//ІД групи з якою пов'язати
    //    return h;
}
//----------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader4matilda::defSntpBalans()
{
    QVariantHash h;
    h.insert("queue", 5000);
    h.insert("secs", 30);
    h.insert("thrd", 60);
    return h;
}
//----------------------------------------------------------------------------------------------------------------
///protocol v2 end

QVariantHash SettLoader4matilda::defForwardTbl(){ return QVariantHash() ; }

bool SettLoader4matilda::defForwardTbl4poll()   { return false          ; }

//----------------------------------------------------------------------------------------------------------------
QStringList SettLoader4matilda::uartList()
{
    QStringList list;
#ifdef HASSERIALLIB
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        list.append(info.portName() );
    defUartName();
    defGprsUartName();
    QDir dir(qApp->applicationDirPath());
    if(dir.cdUp()){
        dir.setPath(QString("%1/lnk2uart").arg(dir.path()));
        QFileInfoList listFi = dir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot);
        for(int i = 0, iMax = listFi.size(); i < iMax; i++){
            if((listFi.at(i).isSymLink() || listFi.at(i).isFile())){
                QString realPortName = listFi.at(i).absoluteFilePath();
                if(listFi.at(i).isSymLink())
                    realPortName = listFi.at(i).symLinkTarget();

                bool ok = false;
                QSerialPort port(realPortName);
                if(port.open(QSerialPort::ReadOnly)){
                    ok = true;
                }else{
                    QFile f(realPortName);
                    if(f.exists())
                        ok = true;
                }
                port.close();
               if(ok)
                   list.append(listFi.at(i).fileName());
            }
        }
    }
#else
    qDebug() << "serialport library not loaded";
#endif
    return list;
}
//----------------------------------------------------------------------------------------------------------------
QString SettLoader4matilda::lnk2uartChecker(const QString &path2dev, const QString &lnkName)
{
    QString linkPath = QString(path2myDir("lnk2uart"));
    QDir dir(linkPath);
    if(!dir.exists())
        dir.mkpath(linkPath);

    linkPath.append("/" + lnkName);

    QFileInfo fi(linkPath);
    if(!fi.exists() || fi.symLinkTarget() != path2dev || !fi.isSymLink()){
        if(!createSymlinkForced(path2dev, linkPath))
            return path2dev;
    }
    return lnkName;
}
//----------------------------------------------------------------------------------------------------------------
QStringList SettLoader4matilda::isZigBeeOverUSB()
{
    bool isUsb = false;
    SettLoader4matilda().getRealPortName(SettLoader4matilda().loadOneSett(SETT_ZIGBEE_PORT).toString(), isUsb);
    return QString(isUsb ? "1" : "0").split(" ");
}
//----------------------------------------------------------------------------------------------------------------
QStringList SettLoader4matilda::isGsmOverUSB()
{
    bool isUsb = false;
    SettLoader4matilda().getRealPortName(SettLoader4matilda().loadOneSett(SETT_GPRS_PORT_NAME).toString(), isUsb);
    return QString(isUsb ? "1" : "0").split(" ");
}
//----------------------------------------------------------------------------------------------------------------
QList<int> SettLoader4matilda::strList2uint8list(const QStringList &sl, bool &ok)
{
    ok = true;
    QList<int> l;
    for(int i = 0, iMax = sl.size(); ok && i < iMax; i++)
        l.append(sl.at(i).toInt(&ok));
    if(!ok && !l.isEmpty())
        l.removeLast();
    return l;
}
//----------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader4matilda::checkIpRouteTable(const QVariantHash &h)
{

    QVariantHash saveHash;
    QList<QString> lk = h.keys();

    for(int i = 0, iMax = lk.size(); i < iMax; i++){
        QHostAddress haddr(lk.at(i));
        if(haddr.protocol() == QAbstractSocket::IPv4Protocol || haddr.protocol() == QAbstractSocket::IPv6Protocol){
            QStringList l = h.value(lk.at(i)).toStringList();
            if(l.size() == 2 && (QHostAddress(lk.first()).protocol() == QAbstractSocket::IPv4Protocol || QHostAddress(lk.first()).protocol() == QAbstractSocket::IPv6Protocol)){
                saveHash.insert(lk.at(i), l);
            }
        }
    }
    qDebug() << "checkIpRouteTable " << h << saveHash;
    return saveHash;
}
//----------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader4matilda::checkPppSupervisor(const QVariantHash &h)
{
    QVariantHash saveHash;
    QVariantHash savedHash = SettLoader4matilda().loadOneSett(SETT_PPP_SUPERVISOR).toHash();

    QStringList l = QString("iplist loc ping lcpi lcpe pingf ide dns ntp mtd da").split(" ", QString::SkipEmptyParts);

    for(int i = 0, iMax = l.size(); i< iMax; i++){
        QVariant val;
        if(h.contains(l.at(i))){
            val = h.value(l.at(i));
        }else{
            if(savedHash.contains(l.at(i)))
                val = savedHash.value(l.at(i));
        }

        if(val.isNull()){
            qDebug() << "checkPppSupervisor " << val.isNull() << val << i << l.at(i) << savedHash.value(l.at(i)) << savedHash;

            if(i == 0 || i >= 6){
                if(i == 0)
                    val = QStringList();
                else
                    val = true;
            }else{
                if(i > 0 && i < 6){
                    switch(i){
                    case 1: val = DEF_VAL_LIFE_OF_CONNECTION; break;
                    case 2: val = DEF_VAL_PING_INTERVAL     ; break;
                    case 3: val = DEF_VAL_LCP_INTERVAL      ; break;
                    case 4: val = DEF_VAL_LCP_FAIL          ; break;
                    case 5: val = DEF_VAL_PING_FAIL         ; break;
                    }
                }
            }
        }

        if(i == 0 || i >= 6){
            if(i == 0)
                saveHash.insert(l.at(i), val.toStringList());
            else
                saveHash.insert(l.at(i), val.toBool());
        }else{
            if(i > 0 && i < 6){
                bool ok;
                quint32 uval = val.toUInt(&ok);
                if(ok && uval <= MAX_TIME_4_PPP_SUPERVISOR)
                    saveHash.insert(l.at(i), uval);
            }
        }

    }
    return saveHash;
}
//----------------------------------------------------------------------------------------------------------------
QPair<QString, quint16> SettLoader4matilda::getHostAndPortFromLine(const QString &line, const quint16 &defPort)
{
    QString host = line;
    quint16 port = defPort;

    if(line.contains(":")){
        if(line.contains("]:")){
            if(!line.split("]:").last().isEmpty() && line.split("]:").last() != "0")
                port = line.split("]:").last().toUInt();

            host = line.split("]:").first();
            if(host.left(1) == "[")
                host = host.mid(1);
        }else{
            if(!line.split(":").last().isEmpty() && line.split(":").last() != "0")
                port = line.split(":").last().toUInt();

            host = line.split(":").first();
        }
    }

    return QPair<QString,quint16>(host,port);

}
//----------------------------------------------------------------------------------------------------------------
QByteArray SettLoader4matilda::readSmplTextFile(const QString &path2file)
{
    QFile file(path2file);
    if(file.open(QFile::ReadOnly))
        return file.readAll();
    return "";
}
//----------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader4matilda::readVarHashFile(const QString &path2file)
{
    QVariantHash h;
    QFile file(path2file);
    if(file.open(QFile::ReadOnly)){
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_6);
        stream >> h;
        file.close();

    }
    return h;
}
//----------------------------------------------------------------------------------------------------------------
QVariantHash SettLoader4matilda::aboutZigBeeModem2humanReadable(const QVariantHash &aboutModem)
{
    QVariantHash h;
    bool ok;
    int val;
    //"ATVR" "0206"
    if(aboutModem.contains("ATVR") && aboutModem.value("ATVR").toString().length() > 3)
        h.insert("VR", aboutModem.value("ATVR").toString());

    //"ATSL" "007A8E23"
    //"ATSH" "000D6F00"
    if(aboutModem.contains("ATSL") && aboutModem.contains("ATSH")
            && aboutModem.value("ATSL").toString().length() == 8 && aboutModem.value("ATSH").toString().length() == 8
            && aboutModem.value("ATSL").toString().toULongLong(&ok, 16) > 0 && ok
            && aboutModem.value("ATSH").toString().toULongLong(&ok, 16) > 0 && ok)
        h.insert("SN (EUI64)", aboutModem.value("ATSH").toString() + aboutModem.value("ATSL").toString());

    //"ATDB" "000 RSSI:-42 LQI:255"
    if(aboutModem.contains("ATDB") && aboutModem.value("ATDB").toString().contains("RSSI:") && aboutModem.value("ATDB").toString().contains("LQI:")){
        QString s = aboutModem.value("ATDB").toString();
        s = s.left( s.indexOf("LQI:") + 7 );
        QStringList l = s.split("LQI:", QString::SkipEmptyParts);
        s = l.first();
        s = s.mid(s.indexOf("RSSI:") + 5).simplified().trimmed();
        val = s.toInt(&ok);
        if(val > (-99) && val < (-1) && ok)
            h.insert("RSSI", QString("%1 dBm").arg(val) );
        val = l.last().toInt(&ok);
        if(val >= 0 && val < 256 && ok)
            h.insert("LQI", QString::number(val));
    }

    if(aboutModem.contains("ATAD") && QString("C R E M").split(" ").contains(aboutModem.value("ATAD").toString().toUpper())){
        switch(QString("C R E M").split(" ").indexOf(aboutModem.value("ATAD").toString().toUpper())){
        case 0: h.insert("Type", "Coordinator"); break;
        case 1: h.insert("Type", "Router"); break;
        case 2: h.insert("Type", "End Device"); break;
        case 3: h.insert("Type", "Mobile Device"); break;
        }
    }

    if(aboutModem.contains("ATHP") && aboutModem.value("ATHP").toString().toInt(&ok, 16) > 0 && ok)
        h.insert("Hops", QString::number(aboutModem.value("ATHP").toString().toInt(&ok, 16)));

    if(aboutModem.contains("ATID") && aboutModem.value("ATID").toString().toInt(&ok, 16) >= 0 && ok && aboutModem.value("ATID").toString().toInt(&ok, 16) <= 0x3FFF)
        h.insert("Network ID", "0x" + aboutModem.value("ATID").toString() );


    if(aboutModem.contains("ATHV") && aboutModem.value("ATHV").toString().length() > 3)
        h.insert("HV", aboutModem.value("ATHV").toString());

    if(aboutModem.contains("ATCH") && aboutModem.value("ATCH").toString().toInt(&ok, 16) >= 0x0B && ok && aboutModem.value("ATCH").toString().toInt(&ok, 16) <= 0x1A){
        QStringList listChannels = QString("11 | 2405MHz | 0x0B\n12 | 2410MHz | 0x0C\n13 | 2415MHz | 0x0D\n14 | 2420MHz | 0x0E\n15 | 2425MHz | 0x0F\n"
                                           "16 | 2430MHz | 0x10\n17 | 2435MHz | 0x11\n18 | 2440MHz | 0x12\n19 | 2445MHz | 0x13\n20 | 2450MHz | 0x14\n"
                                           "21 | 2455MHz | 0x15\n22 | 2460MHz | 0x16\n23 | 2465MHz | 0x17\n24 | 2470MHz | 0x18\n25 | 2475MHz | 0x19\n"
                                           "26 | 2480MHz | 0x1A").split("\n");
        val = aboutModem.value("ATCH").toString().toInt(&ok, 16) - 0x0B;
        if(val >= 0 && val < listChannels.size() && ok)
            h.insert("Channel", listChannels.at(val));
    }

    if(aboutModem.contains("ATPL") && aboutModem.value("ATPL").toString().toInt(&ok) >= (-43) && ok && aboutModem.value("ATPL").toString().toInt(&ok) <= 3){
        QStringList listPowerLevel ;

        val = h.value("HV").toString().mid(1).toInt(&ok);
        if(ok){
            if((val%2) > 0 )     //1 100mW  or  0 2mW
                listPowerLevel = QString(" 003 dBm | 100.0 mW MAX\n 002 dBm |  80.0 mW\n 001 dBm |  63.0 mW\n 000 dBm |  50.0 mW\n"
                    "-001 dBm | 40.0 mW\n-002 dBm | 32.0 mW\n-003 dBm | 25.0 mW\n-004 dBm | 20.0 mW\n"
                    "-005 dBm | 16.0 mW\n-006 dBm | 12.5 mW\n-007 dBm | 10.0 mW\n-008 dBm |  8.1 mW\n"
                    "-009 dBm |  7.9 mW\n-011 dBm |  5.0 mW\n-012 dBm |  4.0 mW\n-014 dBm |  2.5 mW\n"
                    "-017 dBm |  1.0 mW\n-020 dBm |  630 uW\n-026 dBm |  160 uW\n-043 dBm |  3 uW MIN").split("\n");
            else
                listPowerLevel = QString(" 003 dBm | 2.00 mW MAX\n 002 dBm | 1.60 mW\n 001 dBm | 1.30 mW\n 000 dBm | 1.00 mW\n"
                    "-001 dBm | 0.79 mW\n-002 dBm | 0.63 mW\n-003 dBm | 0.50 mW\n-004 dBm | 0.39 mW\n"
                    "-005 dBm | 0.31 mW\n-006 dBm | 0.25 mW\n-007 dBm | 0.20 mW\n-008 dBm | 0.16 mW\n"
                    "-009 dBm | 0.13 mW\n-011 dBm | 0.08 mW\n-012 dBm | 0.06 mW\n-014 dBm | 0.04 mW\n"
                    "-017 dBm | 0.02 mW\n-020 dBm | 10 uW\n-026 dBm | 2.5 uW\n-043 dBm | 50nW MIN").split("\n");
        }else{
            listPowerLevel = QString(" 003 dBm | 100.0 (2.00) mW MAX\n 002 dBm |  80.0 (1.60) mW\n 001 dBm |  63.0 (1.30) mW\n 000 dBm |  50.0 (1.00) mW\n"
                                                         "-001 dBm | 40.0 (0.79) mW\n-002 dBm | 32.0 (0.63) mW\n-003 dBm | 25.0 (0.50) mW\n-004 dBm | 20.0 (0.39) mW\n"
                                                         "-005 dBm | 16.0 (0.31) mW\n-006 dBm | 12.5 (0.25) mW\n-007 dBm | 10.0 (0.20) mW\n-008 dBm |  8.1 (0.16) mW\n"
                                                         "-009 dBm |  7.9 (0.13) mW\n-011 dBm |  5.0 (0.08) mW\n-012 dBm |  4.0 (0.06) mW\n-014 dBm |  2.5 (0.04) mW\n"
                                                         "-017 dBm |  1.0 (0.02) mW\n-020 dBm |  630 (10) uW\n-026 dBm |  160 (2.5) uW\n-043 dBm |  3 uW (50nW) MIN").split("\n");
        }



        int power = aboutModem.value("ATPL").toString().toInt(&ok, 10);

        if(ok){
            if(power > (-44) && power < (-26)) //-43...-25  pl -43
                power = 19;
            else{
                if(power > (-27) && power < (-20)){ //-26...-21
                    power = 18;
                }else{
                    switch(power){
                    case   3: { power =  0; break; }
                    case   2: { power =  1; break; }
                    case   1: { power =  2; break; }
                    case   0: { power =  3; break; }
                    case  -1: { power =  4; break; }
                    case  -2: { power =  5; break; }
                    case  -3: { power =  6; break; }
                    case  -4: { power =  7; break; }
                    case  -5: { power =  8; break; }
                    case  -6: { power =  9; break; }
                    case  -7: { power = 10; break; }
                    case  -8: { power = 11; break; }
                    case  -9: { power = 12; break; }
                    case -10: { power = 13; break; }
                    case -11: { power = 13; break; }
                    case -12: { power = 14; break; }
                    case -13: { power = 15; break; }
                    case -14: { power = 15; break; }
                    case -15: { power = 16; break; }
                    case -16: { power = 16; break; }
                    case -17: { power = 16; break; }
                    case -18: { power = 17; break; }
                    case -19: { power = 17; break; }
                    case -20: { power = 17; break; }
                    default : { power = -1; break; }
                    }
                }
            }
        }

        if(ok && power >= 0 && power < listPowerLevel.size())
            h.insert("Power level", listPowerLevel.at(power).trimmed());
    }


    return h;
}

//----------------------------------------------------------------------------------------------------------------

QVariantHash SettLoader4matilda::validFFrealySett(const QVariantHash &h, bool &allBad)
{
    allBad = true;
    QVariantHash hh;

    if(!QString("MTX BUS Port").split(" ").contains(h.value("type").toString()) || h.value("adr").toString().isEmpty())
        return hh;

    if(h.value("type").toString() == "MTX" && (h.value("adr").toInt() < 0 || h.value("adr").toInt() >= 0xFFFF))
        return hh;

    if(h.value("type").toString() == "BUS" && (h.value("adr").toString().length() > 8))
        return hh;

    if(h.value("type").toString() == "Port")
        return hh;
    allBad = false;

    hh.insert("type", h.value("type"));
    hh.insert("adr", h.value("adr"));
    hh.insert("asg", h.value("asg"));
    hh.insert("asgg", h.value("asgg"));
    return hh;

}

//----------------------------------------------------------------------------------------------------------------

QVariantHash SettLoader4matilda::validFFmodeSett(const QVariantHash &h, bool &allBad)
{
    allBad = true;
    QList<QString> lk = defFFmode().keys();
    for(int i = 0, iMax = lk.size(); i < iMax; i++){
        if(!h.contains(lk.at(i)))
            return defFFmode();
    }

    if(h.value("enRemote").toBool() && (h.value("remIp").toString().isEmpty() || h.value("remPort").toInt() < 80 || h.value("remPort").toInt() >= 0xFFFF))
        return defFFmode();

    if(h.value("pwrIntrvl").toInt() < 5 || h.value("pwrIntrvl").toInt() > 1080 || h.value("pollTime").toInt() < 5 || h.value("pollTime").toInt() > 1080)//17 hours
        return defFFmode();

    QVariantHash hh;
    for(int i = 0, iMax = lk.size(); i < iMax; i++)
        hh.insert(lk.at(i), h.value(lk.at(i)));
    allBad = false;
    return hh;

}
//----------------------------------------------------------------------------------------------------------------
quint64 SettLoader4matilda::integerValidator(const quint64 &val, const quint64 &minVal, const quint64 &maxVal)
{
    if(val < minVal)
        return minVal;
    if(val > maxVal)
        return maxVal;
    return val;
}

//----------------------------------------------------------------------------------------------------------------

