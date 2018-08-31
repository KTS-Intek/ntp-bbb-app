#ifndef METERPLUGIN_H
#define METERPLUGIN_H

#include <QList>
#include <QStringList>
#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QVariant>
#include <QHash>
#include <QPair>

#define PLG_VER_1_0     10

class MeterPlugin
{
public:
    virtual ~MeterPlugin() {}

    virtual QString getMeterType() = 0;    

    virtual quint16 getPluginVersion() = 0;

    virtual quint8 getPasswdType() = 0;

    virtual QByteArray getDefPasswd() = 0;

    virtual QString getVersion() = 0;

    virtual QString getMeterAddrsAndPsswdRegExp() = 0;

    virtual QString getSupportedMeterList() = 0;

    virtual quint8 getMaxTariffNumber(const QString &version) = 0;

    virtual QStringList getEnrgList4thisMeter(const quint8 &pollCode, const QString &version) = 0;

    virtual QPair< QVariantHash, QVariantHash > messToMeter(const QPair< QVariantHash, QVariantHash > &pairAboutMeter) = 0;

    virtual QVariantHash decodeMeterData(const QHash<QString, QVariant> &threeHash) = 0;

    virtual QVariantHash helloMeter(const QVariantHash &hashMeterConstData) = 0;

    virtual QString meterTypeFromMessage(QByteArray &readArr) = 0;

    virtual QVariantHash isItYour(const QByteArray &arr) = 0;

    virtual QVariantHash isItYourRead(const QByteArray &arr) = 0;

    virtual QByteArray niChanged(const QByteArray &arr) = 0;

    virtual QVariantHash meterSn2NI(const QString &meterSn) = 0;


    //parametryzatsiya
    virtual QPair< QVariantHash, QVariantHash > messParamPamPam(const QPair< QVariantHash, QVariantHash > &pairAboutMeter) = 0;
    virtual QVariantHash decodeParamPamPam(const QHash<QString, QVariant> &threeHash) = 0;

//    virtual QPair<QString,QString> niChandedFixedNI() = 0;



//    virtual int getPasswdType() = 0;

//    virtual QByteArray getDefaultPasswd() = 0;

//    virtual QByteArray getSupportedMeterType() = 0;

//    virtual QStringList getListOfEnergy() = 0;

////    virtual QList<QVariant> meterPollValue(const quint8 &pollStatus, const QByteArray &value, const QDateTime &dateTime, const QByteArray &passwd, const QByteArray &nodeID, const quint16 &command) = 0;

////    virtual QHash<QString, QVariant> meterPollValue(const QHash<QString, QVariant> &hashData) = 0;

//    virtual QPair<QVariant, QVariant> meterPollValue(const QHash<QString, QVariant> &hashData, const QHash<QString, QVariant> &hashTmpValues) = 0;


//    virtual QList<QVariant> detectMeterTypeMessage(const QByteArray &nodeID) = 0;

//    virtual QString detectMeter(const QByteArray &readArr) = 0;

//    virtual bool isAllowed(const QByteArray &byteArr) = 0;

//    virtual bool iHaveThreePhase() = 0;


};

#if QT_VERSION < 0x050000
Q_DECLARE_INTERFACE(MeterPlugin, "ua.zbyralko.hello_zb.MeterPlugin")
#else
QT_BEGIN_NAMESPACE

#define PluginInterface_iid "ua.zbyralko.hello_zb.MeterPlugin"

Q_DECLARE_INTERFACE(MeterPlugin, PluginInterface_iid)
QT_END_NAMESPACE

#endif
#endif // METERPLUGIN_H
