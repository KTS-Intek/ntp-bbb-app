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


#ifndef SETTLOADER4MATILDA_H
#define SETTLOADER4MATILDA_H

#include <QObject>
#include <QHostAddress>

//peredavator settt
//zigbee group
#define SETT_ZIGBEE_PORT            1
#define SETT_ZIGBEE_PORT_SPEED      2
#define SETT_ZIGBEE_READ_TO         3
#define SETT_ZIGBEE_READ_TOB        4
#define SETT_ZIGBEE_AS_DATA         5

//tcp group
#define SETT_TCP_READ_TO            50
#define SETT_TCP_READ_TOB           51
#define SETT_TCP_PPPD_FIRST         52
#define SETT_TCP_MNOZNYK            53


//poll group
#define SETT_POLL_FRWRD_TBL_4_PLL   200 // використовувати при опитуванні таблицю переадресації модемів
#define SETT_POLL_METER_RETRY       201 // к-ть повторів при опитуванні лічильника (к-ть невдалих підряд спроб зв'язатись з лічильником)
#define SETT_POLL_METER_RETRY_FA    202 // к-ть повторів при опитуванні лічильника до першої відповіді (к-ть невдалих підряд спроб зв'язатись з лічильником до першої відповіді)
#define SETT_POLL_HARD_ADDRS        203 // якщо Правда то в протоколі лічильника буде використано вказана адреса,  в іншому випадку: по можливості широкомовна адреса
#define SETT_POLL_WAIT4EMB          204 // очікування на готовність мережі
#define SETT_POLL_WAIT4EMB_RTRBFR   205 // к-ть підряд неопитаних лічильників, до першого спрацьовування очікування на готовність мережі
#define SETT_POLL_WAIT4EMB_RTRAFT   206 // к-ть підряд неопитаних лічильників, повторного спрацьовування очікування на готовність мережі
#define SETT_POLL_TIME_CORR_ENBL    207 // увімкнути корекцію часу
#define SETT_POLL_DIFF_TIME_CORR    208 // різниця в часі
#define SETT_POLL_WAIT4POLL         209 // затримка до опитування
#define SETT_POLL_AUTO_FRWRD_TBL    210 // автоматично наповнювати при опитуванні таблицю переадресації модемів (тільки якщо адреса модема не співпадає з адресою лічильника)

//"main-gprs"
#define SETT_GPRS_PORT_NAME         300
#define SETT_GPRS_BAUD_RATE         301
#define SETT_GPRS_APN               302
#define SETT_GPRS_USERNAME          303
#define SETT_GPRS_PASSWORD          304
#define SETT_GPRS_NUMBER            305
#define SETT_GPRS_PREFF_2G_3G       306

//"about"
#define SETT_ABOUT_MEMO             350
#define SETT_ABOUT_COORDNTS         351
#define SETT_ABOUT_MEMOLINE         352


//"SlavaUkrajini"
#define SETT_SOME_SETT              400
#define SETT_SVAHA_SYNC_ENABLE      401 //protocol v2


//"matilda-active-client"
#define SETT_MAC_ENABLE             450
#define SETT_MAC_SVAHA_LIST         451
#define SETT_MAC_DAY_PROFILE        452


//matilda vs peredavator sett
#define SETT_WHITE_IP_LIST          1000 //allow only for this IPs
#define SETT_BLACK_IP_LIST          1001 //block this IPs
#define SETT_TCP_PORT               1002 //listening port
#define SETT_TCP_HOST_ADDR          1003 //listen on this HostName
#define SETT_ENABLE_DA              1004 //вкл/викл вихідних з'єданнь
#define SETT_PAC_LIST               1005 //PAC list (peredavator active client)
#define SETT_PAC_DAY_PROFILE        1006 //day profile for PAC
#define SETT_PEREDAVATOR_MODE       1007 //1 - always on, 2 magic sequence,
#define SETT_MAGIC_WORD             1008 //
#define SETT_MAGIC_WORDH            1009 //
#define SETT_DBG_PORT               1010
#define SETT_MSERVER_PORT           1011 //матільда-сервер порт
#define SETT_UDP_BEACON_ENBL        1012 //увімкнути ЮДіПі  маячок

///protocol v2
#define SETT_IP_ROUTE_TABLE         1013 //custom ip route table
#define SETT_PPP_SUPERVISOR         1014 // наглядач за PPP, пінг && тривалість з'єднання

#define SETT_FORWARD_TABLE          1510 //


#define SETT_FIREFLY_SERVERPORT     2000
#define SETT_FIREFLY_ACCESS_IP      2001
#define SETT_FIREFLY_REMOTE_HOST    2002
#define SETT_FIREFLY_MODE           2003
#define SETT_FIREFLY_REALAY         2004


#define SETT_SNTP_SERVICE           3000


#define DA_MODE_ON      1 //always on
#define DA_MODE_MGC     2 //always on + magic sequence
#define DA_MODE_OFF     3 //always off (only local IPv4)


#define MIN_READ_TO 1
#define MAX_READ_TO 120000



#define EXT_CODE_PORT_NOT_VALID     11100
#define EXT_CODE_PEREDAVATOR_DONE   11101

#define SETT_DEFAULT_DBGUARTPORT    5656
#define SETT_DEFAULT_MATILDAPORT    9090
#define SETT_DEFAULT_PEREDAVPORT    8989

#define SETT_DEFAULT_FIREFLYPORT    3434
#define SETT_DEFAULT_DBGFFLYPORT    5757


class SettLoader4matilda : public QObject
{
    Q_OBJECT
public:
    explicit SettLoader4matilda(QObject *parent = 0);

    QHash<int,QVariant> loadAllSett();

    QHash<int,QVariant> loadAllSettPeredavator();


    QString groupName4key(const int &key);

    QString valueName4key(const int &key);

    QVariant defValue4key(const int &key);


    QVariant loadOneSett(const int key);

    bool saveOneSett(const int key, const QVariant data2save);

    QString path2sett(const int key);

    QString getRealPortName(const QString &portName, bool &isUsb);

    static bool createSymlinkForced(const QString &linkPath, const QString &targetPath);

    static QString path2backups();

    static QString path2config();

    static QString path2script();

    static QString path2tmpDir();


    static QString path2chatGprs();   

    static QString path2peersGprs();

    static QString path2database();


    static QString path2workDir();

    static QString path2myDir(const QString &myPath);

    static QString defLocalServerName();

    static QString defUdpLocalServerName();

    static QString defSemaName();

    static QString defSharedMemoName();



//procManager
    static QString defProcManagerMemoName();

    static QString defProcManagerSemaName();



    //matilda-server
    static QString defMatildaServerMemoName();

    static QString defMatildaServerSemaName();

    static QString defPeredavatorMemoName();

    static QString defZbyratorMemoName();

    static QString defZbyratorCounterMemoName();

    static QString defZbyratorQuickPollName();


    static QString defPeredavatorSemaName();

    static QString defZbyratorSemaName();

    static QString defZbyratorCounterSemaName();

    static QString defZbyratorQuickPollSemaName();


    static QString defAboutGsmModemMemoName();

    static QString defAboutGsmModemSemaName();

    static QString defAboutZbModemMemoName();

    static QString defAboutZbModemSemaName();


    static QString defSntpServerMemoName();

    static QString defSntpServerSemaName();


    static QString defSvahaServerMemoName();

    static QString defSvahaServerSemaName();


    static QString showNormalIP(const QHostAddress &hAddr);

    static QString showNormalIP(const QString &ip);

    //zigbee
    static QString defUartName() ;

    static qint32 defUartSpeed() ;

    static qint32 defUartReadTO() ;

    static qint32 defUartReadTOB() ;

    static bool defAsData() ;

    static QString defLnkNameZigBee();


    static QList<int> supportedUartSpeed() ;

    static QStringList supportedUartSpeedSL();


//tcp
    static  qint32 defTcpReadTO() ;

    static qint32 defTcpReadTOB() ;

    static bool defTcpPPPDfirst() ;

    static qint32 defTcpMnznk();

    //poll
    static bool defPollFrwrd();

    static qint32 defPollMeterRetry();

    static qint32 defPollMeterRetryFA();

    static bool defPollHardAddrs();

    static bool  defPollW4Emb();

    static qint32  defPollW4EmbRtrBfr();

    static qint32 defPollW4EmbRtrAft();

    static bool defPollTimeCorrEn();

    static qint32 defPollDiffTimeCorr();

    static qint32 defPollWait4poll();

    static bool defAdd2ForwardTable();


//"matilda-active-client"
    static bool defMacEnable();

    static QVariantHash defMacSvahaList();

    static QVariantHash defDayProfile();



    //main-gprs
    static QString defGprsUartName() ;

    static qint32 defGprsUartSpeed() ;

    static QString defGprsApn() ;

    static QString defGprsUserName() ;

    static QString defGprsPasswrd() ;

    static QString defGprsNmbr() ;

    static int defPref2g3g() ;

    static QString defLnkNameGsm();


    //about
    static QString defAboutMemo() ;

    static QString defAboutCrdnts() ;

    static QString defAboutMemoLine() ;


    //Слава Україні
    static QVariantHash defSomeSett() ;


    //peredavator vs matilda  matilda-server
    static QStringList defWhiteIpList();

    static QStringList defBlackIpList();

    static quint16 defServerPort() ;

    static QString defServerName() ;

    static bool defEnDA();

    static QVariantHash defPacList();

    static int defPeredavatorMode();

    static QString defMagicSequence();

    static bool defMagicInHex();


    static quint16 defDbgUargPort();

    static quint16 defMServerPort();

    static bool defUdpBeacon();

    ///protocol v2 start
    static QVariantHash defIpRoute();

    static QVariantHash defPppSupervisor();

    ///led lamp
    static QString defFireflyLedListMemoName();

    static QString defFireflyLedListSemaName();

    static QString defFireflyScheduleMemoName();

    static QString defFireflyScheduleSemaName();

    static QString defFireflyTaskMemoName();

    static QString defFireflyTaskSemaName();

    static QString defFireflyRelayMemoName();

    static QString defFireflyRelaySemaName();

    static QString defFireflyStatusMemoName();

    static QString defFireflyStatusSemaName();

    //main-firefly
    static QString defFFremoteServer();

    static quint16 defFFtcpPort();

    static QStringList defFFallowAccess4thisIps();

    static quint16 defDbgFireflyPort();

    static QVariantHash defFFmode();

    static QVariantList defFFrealaySett();

    static QVariantHash defSntpBalans();

    ///protocol v2 end

    static QVariantHash defForwardTbl();

    static bool defForwardTbl4poll();

    //helper
    static QStringList uartList();

    static QString lnk2uartChecker(const QString &path2dev, const QString &lnkName);


    static QStringList isZigBeeOverUSB();

    static QStringList isGsmOverUSB();

    static QList<int> strList2uint8list(const QStringList &sl, bool &ok);

    static QVariantHash checkIpRouteTable(const QVariantHash &h);

    static QVariantHash checkPppSupervisor(const QVariantHash &h);

    static QPair<QString,quint16> getHostAndPortFromLine(const QString &line, const quint16 &defPort);

    static QByteArray readSmplTextFile(const QString &path2file);

    static QVariantHash readVarHashFile(const QString &path2file);

    static QVariantHash aboutZigBeeModem2humanReadable(const QVariantHash &aboutModem);


    //settings validators
    static QVariantHash validFFrealySett(const QVariantHash &h, bool &allBad);

    static QVariantHash validFFmodeSett(const QVariantHash &h, bool &allBad);

    static quint64 integerValidator(const quint64 &val, const quint64 &minVal, const quint64 &maxVal);


signals:

public slots:

private:

};

#endif // SETTLOADER4MATILDA_H
