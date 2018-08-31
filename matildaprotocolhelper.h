#ifndef MATILDAPROTOCOLHELPER_H
#define MATILDAPROTOCOLHELPER_H

#include <QtCore>
#include <QJsonObject>

class MatildaProtocolHelper
{

public:
    MatildaProtocolHelper();

    static QVariantHash errCodeLastOperation(quint16 &s_command, const quint16 &command, const int &errCode);

    static QJsonObject errCodeLastOperationJson(quint16 &s_command, const quint16 &command, const int &errCode);

    static QString getSemaNameAndSharedMemoryName(const qint8 &mtdExtNameIndx, QString &sharedMemoKey);

    static QVariantHash readFromSharedMemory(const qint8 &mtdExtNameIndx);

    static QVariantList readFromSharedMemoryFFledListFormat(qint8 mtdExtNameIndx);

    static QVariantList readFromSharedMemoryFFtaskFormat(qint8 mtdExtNameIndx);

    static QVariantList readFromSharedMemoryFFscheduleFormat(qint8 mtdExtNameIndx);


    static bool write2sharedMemory(const QVariantHash &h, QSharedMemory &shmem, const QString &semaKey);


    static QStringList listNiFromLine(const QString &aLine) ;


    static int checkMessSize(const QVariant s_data, const quint16 s_command);

    static qint64 chkMessSize(const QVariant &s_data);


    static QVariantHash preparyExtError(quint16 &s_command, const QString &mess, const quint16 &errComment, const quint16 &command) ;

    static QJsonObject preparyExtErrorJson(quint16 &s_command, const QString &mess, const quint16 &errComment, const quint16 &command) ;


    static QByteArray wrtHashAsJsonDoc(const QVariantHash &h);


    static int hshSummVal(const QString &arg) ;

    static QString hshSummName(const int &indx);

    static QStringList getHshNames();

    static QStringList defValEnergyKeys(const quint8 &code) ;

    static QStringList defValEnergy(const quint8 &code);

    static QDateTime dateTimeFromStr(const QString &str);

    static QStringList varList2strList(const QVariantList &list);

    static QVariantList strList2VarList(const QStringList &list);

    static QVariant strList2Var(const QStringList &list);

    static QJsonArray arrFromList(const QVariantList &list);

    static QJsonArray arrFromList(const QStringList &list);

    static bool messHshIsValid(const QJsonObject &jObj, QByteArray readArr, int &lastHashSumm);


    static QStringList codeList4hash(const QString &code, bool &ok);

    static quint64 calcMaxDataLen(const quint64 &dataLen, quint64 &lastMaxDataLen, bool &buvIneedMoreTime, const qint64 &mynulo4asy, const qint64 &chasovyjMnoznykPereda4i, const qint64 &connSpeed, const qint64 &lastByteWrt);

    static qint64 calcMaxDataLenSigned(const qint64 &dataLen, qint64 &lastMaxDataLenSigned, bool &buvIneedMoreTime, const qint64 &mynulo4asy, const qint64 &chasovyjMnoznykPereda4i, const qint64 &connSpeed, const qint64 &lastByteWrt);

    static bool fileIsExists(const QString &fileName);

    static bool urlIsValid(const QString &strUrl);

    static QVariantHash loadAboutPlugin();

    static bool readMeterListHashSumm(QByteArray &hsh, QString &errStr, const int &alg, const bool &isJson, QStringList listColmns = QStringList() );

    static QByteArray runProc(const QString &progName, const QStringList args = QStringList(), const int timeLimitMsec = 5000, const QByteArray endArr = "", const bool chopEndArr = true, const bool mergedChannels = false);

    static QByteArray runBashProc(const QByteArray &bashCommand, const int timeLimitMsec = 5000);

    static QString getInfoAboutSystem();

    static QByteArray getTaskInfo(bool &ok);

    static QByteArray getNtpServices(bool &ok);

    static bool isIpAddress(const QString &s);

    static void showDebugOut4varHash(const QVariantHash &h, const QString &firstLine);

    static int calcTMsecCheck(int tMSec, bool &tMSecCheck);

    static void syncRtcWithSystemcClock();
    
    static bool checkCreateDir(const QString &path2dir);

    ///protocol v2
    static QString getDevSerialNumber();

    static QByteArray compressVarList(const QVariantList &varList, const bool addSize = true);

    static QVariantList uncompress2varList(const QByteArray &inArr, bool &ok, const bool hasSize = true);

    static QByteArray compressVarHash(const QVariantHash &hash, const bool addSize = true);

    static QVariantHash uncompress2varHash(const QByteArray &inArr, bool &ok, const bool hasSize = true);

    static QString getKeysLedLamp();
    static QString getKeysLedLampCol();

    static QString getKeysLedLampRW();

    static QString getKeysLedLampWriteOnly();

    static QString getKeysMeterList();


    static QHash<QString,QString> strHashFromQsl(const QStringList &l, const QString &separ, QList<QString> &lk, const int key = 0, const int val = 1, const bool oneMode = true);

    static QStringList qslFromHash(const QHash<QString,QString> &h, const QString &separ, QList<QString> lk = QList<QString>());

    static QString evcodeV2toV1(QString evLine, QString &lastModel, const QString defModel = "UCM");

    static QHash<QString,QString> getMeterSn2meterModel();

    static QString validateCoordinates(const QString &instr, bool &ok);

    static QStringList addLine2list(QString line, QStringList log, const int &maxLogSize, const QString timeStampMask = "yyyy-MM-dd hh:mm:ss");

    static QStringList addLines2list(QStringList lines, QStringList log, const int &maxLogSize, const QString timeStampMask = "yyyy-MM-dd hh:mm:ss");

    static QByteArray calcFileSha1(const QString &fileName, bool &ok);

    static QVariantHash getLastAboutObject(const QString &version, const QString &devId, const int &devType);

    static void addData2hash(QVariantHash &mainHash, const QVariantHash &h, const QStringList &lk, const QStringList &lk4maint);

    static void checkHostName();

    static QString devType2str(const int &devType);

    static QStringList getSyncDirPaths(const QString &workDir);

    static QHash<QString, QStringList> getSyncDirPathsFiles(const QString &workDir, QStringList &lk);

    static bool isCommandAllowed4thisDev(const quint16 &command, const int &devType);

    static QList<quint16> getCommand4devSvaha();

    static QList<quint16> getCommand4devPoll();

    static QVariantHash aboutObjLine2hashObj(const QString &aboutObjLine);

    static QVariantHash backupFileName2hashObj(QString fileName);

    static QString getValFromFileName(const QString &fileName, const QString &key);


};

#endif // MATILDAPROTOCOLHELPER_H
