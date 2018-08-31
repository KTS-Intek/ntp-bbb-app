#include "matildaprotocolhelper.h"

#include "moji_defy.h"
#include "settloader4matilda.h"
#include "src/meter/meterplugin.h"
#include "globalhelper.h"
#include <QHostAddress>
#include "src/meter/ucmetereventcodes.h"
#include <QNetworkInterface>

#define TIME_COEFITIENT         100

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include "settloader.h"
#endif

//-----------------------------------------------------------------------------------------
MatildaProtocolHelper::MatildaProtocolHelper()
{

}
//-----------------------------------------------------------------------------------------
QVariantHash MatildaProtocolHelper::errCodeLastOperation(quint16 &s_command, const quint16 &command, const int &errCode)
{
    QVariantHash h;
    h.insert("lcmd", command);
    h.insert("e", errCode);
    s_command = COMMAND_ERROR_CODE;
    return h;
}
//-----------------------------------------------------------------------------------------
QJsonObject MatildaProtocolHelper::errCodeLastOperationJson(quint16 &s_command, const quint16 &command, const int &errCode)
{
    QJsonObject j;
    j.insert("lcmd", command);
    j.insert("e", errCode);
    s_command = COMMAND_ERROR_CODE;
    return j;
}
//-----------------------------------------------------------------------------------------
QString MatildaProtocolHelper::getSemaNameAndSharedMemoryName(const qint8 &mtdExtNameIndx, QString &sharedMemoKey)
{
    QString semaName("");
    switch(mtdExtNameIndx){
    case MTD_EXT_NAME_ZBYRATOR            : sharedMemoKey = SettLoader4matilda::defZbyratorMemoName()     ; semaName = SettLoader4matilda::defZbyratorSemaName()     ; break;
    case MTD_EXT_NAME_PEREDAVATOR         : sharedMemoKey = SettLoader4matilda::defPeredavatorMemoName()  ; semaName = SettLoader4matilda::defPeredavatorSemaName()  ; break;
    case MTD_EXT_NAME_MATILDA_PROC_MANAGER: sharedMemoKey = SettLoader4matilda::defProcManagerMemoName()  ; semaName = SettLoader4matilda::defProcManagerSemaName()  ; break;
    case MTD_EXT_NAME_MATILDA_SERVER      : sharedMemoKey = SettLoader4matilda::defMatildaServerMemoName(); semaName = SettLoader4matilda::defMatildaServerSemaName(); break;
    case MTD_EXT_NAME_ABOUT_GSM           : sharedMemoKey = SettLoader4matilda::defAboutGsmModemMemoName(); semaName = SettLoader4matilda::defAboutGsmModemSemaName(); break;
    case MTD_EXT_NAME_ABOUT_ZB            : sharedMemoKey = SettLoader4matilda::defAboutZbModemMemoName() ; semaName = SettLoader4matilda::defAboutZbModemSemaName() ; break;

    //PROTOCOL V2 led lamps
    case MTD_EXT_NAME_FIREFLY_LEDLIST     : sharedMemoKey = SettLoader4matilda::defFireflyLedListMemoName() ; semaName = SettLoader4matilda::defFireflyLedListSemaName() ; break;
    case MTD_EXT_NAME_FIREFLY_SCHEDULE    : sharedMemoKey = SettLoader4matilda::defFireflyScheduleMemoName(); semaName = SettLoader4matilda::defFireflyScheduleSemaName(); break;
    case MTD_EXT_NAME_FIREFLY_TASK        : sharedMemoKey = SettLoader4matilda::defFireflyTaskMemoName()    ; semaName = SettLoader4matilda::defFireflyTaskSemaName()    ; break;
    case MTD_EXT_NAME_FIREFLY_RELAY       : sharedMemoKey = SettLoader4matilda::defFireflyRelayMemoName()   ; semaName = SettLoader4matilda::defFireflyRelaySemaName()   ; break;

    case MTD_EXT_NAME_ZBYRATOR_QUICK_POLL : sharedMemoKey = SettLoader4matilda::defZbyratorQuickPollName()  ; semaName = SettLoader4matilda::defZbyratorQuickPollSemaName(); break;

    case MTD_EXT_NAME_FIREFLY_STATUS      : sharedMemoKey = SettLoader4matilda::defFireflyStatusMemoName()  ; semaName = SettLoader4matilda::defFireflyStatusSemaName()    ; break;

    case MTD_EXT_NAME_SNTP_SERVICE        : sharedMemoKey = SettLoader4matilda::defSntpServerMemoName()     ; semaName = SettLoader4matilda::defSntpServerSemaName()       ; break;
    case MTD_EXT_NAME_SVAHA_SERVICE       : sharedMemoKey = SettLoader4matilda::defSvahaServerMemoName()    ; semaName = SettLoader4matilda::defSvahaServerSemaName()      ; break;

    default: qDebug() << "LocalSocket::readFromSharedMemory unknown index = " << mtdExtNameIndx ; break;
    }
    return semaName;
}
//-----------------------------------------------------------------------------------------
QVariantHash MatildaProtocolHelper::readFromSharedMemory(const qint8 &mtdExtNameIndx)
{
    QVariantHash hash;
    QString sharedMemoKey("");
    QString semaName = getSemaNameAndSharedMemoryName(mtdExtNameIndx, sharedMemoKey);

    if(!sharedMemoKey.isEmpty() && !semaName.isEmpty()){


        QSharedMemory memory(sharedMemoKey);

        QSystemSemaphore sema(semaName, 1);
        sema.acquire();

        if(memory.attach(QSharedMemory::ReadOnly)){
            QBuffer buffer;

            memory.lock();
            buffer.setData((char *)memory.constData(), memory.size());
            buffer.open(QBuffer::ReadOnly);
            QDataStream in(&buffer);
            in.setVersion(QDataStream::Qt_5_6);
            in >> hash;
            memory.unlock();
            memory.detach();


        }else{
            qDebug() << "can't attach error = " << memory.errorString() << memory.key();
            memory.detach();
        }
        sema.release();


    }
    return hash;
}
//-----------------------------------------------------------------------------------------
QVariantList MatildaProtocolHelper::readFromSharedMemoryFFledListFormat(qint8 mtdExtNameIndx)
{
    QVariantList list;
    QString sharedMemoKey("");
    QString semaName = getSemaNameAndSharedMemoryName(mtdExtNameIndx, sharedMemoKey);

    if(!sharedMemoKey.isEmpty() && !semaName.isEmpty()){


        QSharedMemory memory(sharedMemoKey);

        QSystemSemaphore sema(semaName, 1);
        sema.acquire();

        if(memory.attach(QSharedMemory::ReadOnly)){

            QByteArray bufArrCompressed;
            if(true){
                QBuffer buffer;
                memory.lock();
                buffer.setData((char *)memory.constData(), memory.size());
                buffer.open(QBuffer::ReadOnly);
                QDataStream in(&buffer);
                in.setVersion(QDataStream::Qt_5_6);
                in >> bufArrCompressed;

                memory.unlock();
                memory.detach();
            }

            if(!bufArrCompressed.isEmpty()){
                bufArrCompressed = qUncompress(bufArrCompressed);
                if(bufArrCompressed.isEmpty())
                    qDebug() << "uncompresss error ";
            }

            if(!bufArrCompressed.isEmpty()){
                QStringList keysList;
                QHash<QString, QHash<QString,QString> > hash;

                QDataStream in(&bufArrCompressed, QIODevice::ReadOnly);
                in >> keysList >> hash;

                bufArrCompressed.clear();//free memory
                QList<QString> lk;
                for(int i = 0, iMax = keysList.size(); i < iMax; i++){
                    QHash<QString,QString> h = hash.value(keysList.at(i));
                    lk = h.keys();
                    QVariantHash vh;
                    for(int j = 0, jMax = lk.size(); j < jMax; j++ )
                        vh.insert(lk.at(j), h.value(lk.at(j)));
                    if(!vh.isEmpty())
                        list.append(vh);
                }
            }

        }else{
            qDebug() << "can't attach error = " << memory.errorString()<< memory.key();
            memory.detach();
        }
        sema.release();


    }
    return list;
}
//-----------------------------------------------------------------------------------------
QVariantList MatildaProtocolHelper::readFromSharedMemoryFFtaskFormat(qint8 mtdExtNameIndx)
{
    QVariantList list;
    QString sharedMemoKey("");
    QString semaName = getSemaNameAndSharedMemoryName(mtdExtNameIndx, sharedMemoKey);

    if(!sharedMemoKey.isEmpty() && !semaName.isEmpty()){


        QSharedMemory memory(sharedMemoKey);

        QSystemSemaphore sema(semaName, 1);
        sema.acquire();

        if(memory.attach(QSharedMemory::ReadOnly)){

            QList<int> lTaskQueue;
            QHash<int, QVariantHash> hashTaskTable;

            if(true){
                QBuffer buffer;
                memory.lock();
                buffer.setData((char *)memory.constData(), memory.size());
                buffer.open(QBuffer::ReadOnly);
                QDataStream in(&buffer);
                in.setVersion(QDataStream::Qt_5_6);
                in >> lTaskQueue >> hashTaskTable;

                memory.unlock();
                memory.detach();
            }

            for(int i = 0, iMax = lTaskQueue.size(); i < iMax; i++){
                QVariantHash vh = hashTaskTable.value(lTaskQueue.at(i));
                if(!vh.isEmpty())
                    list.append(vh);
            }


        }else{
            qDebug() << "can't attach error = " << memory.errorString()<< memory.key();
            memory.detach();
        }
        sema.release();


    }
    return list;
}
//-----------------------------------------------------------------------------------------
QVariantList MatildaProtocolHelper::readFromSharedMemoryFFscheduleFormat(qint8 mtdExtNameIndx)
{
    QVariantList list;
    QString sharedMemoKey("");
    QString semaName = getSemaNameAndSharedMemoryName(mtdExtNameIndx, sharedMemoKey);

    if(!sharedMemoKey.isEmpty() && !semaName.isEmpty()){
        QSharedMemory memory(sharedMemoKey);
        QSystemSemaphore sema(semaName, 1);
        sema.acquire();
        if(memory.attach(QSharedMemory::ReadOnly)){
            QHash<QString, QString> h;
            if(true){
                QBuffer buffer;
                memory.lock();
                buffer.setData((char *)memory.constData(), memory.size());
                buffer.open(QBuffer::ReadOnly);
                QDataStream in(&buffer);
                in.setVersion(QDataStream::Qt_5_6);
                in >> h;
                memory.unlock();
                memory.detach();
            }
            QList<QString> lk = h.keys();
            GlobalHelper::lSort(lk);
            for(int i = 0, iMax = lk.size(); i < iMax; i++){
                if(h.value(lk.at(i)).isEmpty())
                    continue;
                QVariantHash vh;
                vh.insert(lk.at(i), h.value(lk.at(i)));
                list.append(vh);
            }
        }else{
            qDebug() << "can't attach error = " << memory.errorString()<< memory.key();
            memory.detach();
        }
        sema.release();
    }
    return list;
}
//-----------------------------------------------------------------------------------------
bool MatildaProtocolHelper::write2sharedMemory(const QVariantHash &h, QSharedMemory &shmem, const QString &semaKey)
{
    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);
    QDataStream out(&buffer);
    out.setVersion(QDataStream::Qt_5_6);
    out << h;
    int size = buffer.size();

    QSystemSemaphore sema(semaKey, 1);// SemaName(), 1);
    sema.acquire();


    if(shmem.isAttached())
        shmem.detach();

    bool memoReady = false;

    for(int i = 0; i < 3 && !memoReady; i++){
        memoReady = shmem.create(size);
        if(!memoReady){
            shmem.attach();
            shmem.detach();
            QThread::msleep(300);
        }
    }

    if(!memoReady){
        shmem.attach();
        shmem.detach();
        sema.release();
        return memoReady;
    }

    shmem.lock();
    char *to = (char *)shmem.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(shmem.size(), size));
    shmem.unlock();

    sema.release();
    return memoReady;
}
//-----------------------------------------------------------------------------------------
QStringList MatildaProtocolHelper::listNiFromLine(const QString &aLine)
{
    QStringList listNI;
    if(!aLine.isEmpty()){

            QStringList listComma ;
            // QStringList listNI;

            listComma = aLine.split(",");

            int niCounter = 0;
            foreach(QString strComma, listComma){

                if(strComma.indexOf('-') >= 0){
                    QStringList listDefis = strComma.split("-");

                    if(listDefis.size() > 1){
                        bool ok;
                        quint64 poch = listDefis.first().toInt(&ok, 10);

                        if(ok){
                            quint64 endNI=listDefis.last().toInt(&ok, 10);
                            if(ok){
                                if(poch <= endNI){
                                    for( ; poch <= endNI; poch++){
                                        listNI.append(QString::number(poch));
                                        niCounter++;
                                    }
                                }else{
                                    for( ; endNI <= poch; poch--){
                                        listNI.append(QString::number(poch));
                                        niCounter++;
                                    }
                                }

                            }else{
                                listNI.append(listDefis.first());
                                listNI.append(listDefis.last());
                                niCounter++;
                                niCounter++;
                            }
                        }else{
                            listNI.append(listDefis.first());
                            niCounter++;
                        }


                    }else{
                        listNI.append(strComma);
                        niCounter++;
                    }

                }else{
                    listNI.append(strComma);
                    niCounter++;
                }
                if(niCounter > 300)
                    break;
            }
            listNI.removeDuplicates();



            for(int i = 0, iMax = listNI.size(); i < iMax; i++){
                if(listNI.at(i).isEmpty()){
                    listNI.removeAt(i);
                    i--;
                    iMax--;
                }
            }

            listNI = listNI.mid(0,300);


    }
    return listNI;
}
//-----------------------------------------------------------------------------------------
int MatildaProtocolHelper::checkMessSize(const QVariant s_data, const quint16 s_command)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //Qt_4_0);
    out << (quint32)0;
    out << s_command << s_data;

    out.device()->seek(0);

    quint32 blSize = block.size();
    quint32 blSizeOld = blSize;

    if(blSize < SETT_MAX_UNCOMPRSS_PACkET_SIZE || s_command == COMMAND_WRITE_UPGRADE || s_command == COMMAND_COMPRESSED_STREAM){
        out << (quint32)(blSize - sizeof(quint32));
        qDebug() << "===blSize " << blSize << blSizeOld << s_command;
        return block.size();

    }else{
         blSizeOld = blSize;
        qDebug() << "===blSize " << block.size() / 6   << blSizeOld << s_command;

        return block.size() / 6;
    }

}
//-----------------------------------------------------------------------------------------
qint64 MatildaProtocolHelper::chkMessSize(const QVariant &s_data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //Qt_4_0);
    out <<  s_data;
    return (qint64)block.size();
}
//-----------------------------------------------------------------------------------------
QVariantHash MatildaProtocolHelper::preparyExtError(quint16 &s_command, const QString &mess, const quint16 &errComment, const quint16 &command)
{
    QVariantHash h;
    h.insert("lcmd", command);
    h.insert("e", errComment);
    h.insert("em", mess);
    s_command = COMMAND_ERROR_CODE_EXT;
    return h;
}
//-----------------------------------------------------------------------------------------
QJsonObject MatildaProtocolHelper::preparyExtErrorJson(quint16 &s_command, const QString &mess, const quint16 &errComment, const quint16 &command)
{
    QJsonObject j;
    j.insert("lcmd", command);
    j.insert("e", errComment);
    j.insert("em", mess);
    s_command = COMMAND_ERROR_CODE_EXT;
    return j;
}
//-----------------------------------------------------------------------------------------
QByteArray MatildaProtocolHelper::wrtHashAsJsonDoc(const QVariantHash &h)
{
    QJsonObject jObj;
    foreach (QString key, h.keys()) {
        jObj.insert(key, h.value(key).toString());
    }
     QJsonDocument jDoc2DST(jObj);
     return jDoc2DST.toJson(QJsonDocument::Compact);
}
//-----------------------------------------------------------------------------------------
int MatildaProtocolHelper::hshSummVal(const QString &arg)
{
    int alg;

    QStringList hashList = QString("Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512").split(",");

    switch(hashList.indexOf(arg)){
    case 0: alg = QCryptographicHash::Md4; break;

    case 2: alg = QCryptographicHash::Sha1; break;

    case 3: alg = QCryptographicHash::Sha224; break;
    case 4: alg = QCryptographicHash::Sha256; break;
    case 5: alg = QCryptographicHash::Sha384; break;
    case 6: alg = QCryptographicHash::Sha512; break;

    case 7: alg = QCryptographicHash::Sha3_224; break;
    case 8: alg = QCryptographicHash::Sha3_256; break;
    case 9: alg = QCryptographicHash::Sha3_256; break;
    case 10: alg = QCryptographicHash::Sha3_512; break;


    default: alg = QCryptographicHash::Md5; break;

    }
    return alg;

    /*Md4,
        Md5,
        Sha1,
        Sha224,
        Sha256,
        Sha384,
        Sha512,
        Sha3_224,
        Sha3_256,
        Sha3_384,
        Sha3_512*/
}
//-----------------------------------------------------------------------------------------
QString MatildaProtocolHelper::hshSummName(const int &indx)
{
    QStringList hashList = QString("Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512").split(",");
    if(indx < 0 || indx >= hashList.size())
        return "Md5";

    return hashList.at(indx);
}
//-----------------------------------------------------------------------------------------

QStringList MatildaProtocolHelper::getHshNames(){ return QString("Md4,Md5,Sha1,Sha224,Sha256,Sha384,Sha512,Sha3_224,Sha3_256,Sha3_384,Sha3_512").split(","); }

//-----------------------------------------------------------------------------------------
QStringList MatildaProtocolHelper::defValEnergyKeys(const quint8 &code)
{
    if(code == POLL_CODE_READ_VOLTAGE || code == POLL_CODE_READ_POWER || code == POLL_CODE_METER_STATUS)
        return defValEnergy(code);
    else{
        QStringList listEnrg = defValEnergy(code);
        QStringList list;
        for(int i = 0, iMax = listEnrg.size(); i < iMax; i++){
            for(int j = 0; j < MAX_TARIFF_COUNT; j++){
                list.append(QString("T%1_%2")
                            .arg(QString::number(j))
                            .arg(listEnrg.at(i) ));
            }
        }
        return list;
    }
}
//-----------------------------------------------------------------------------------------
QStringList MatildaProtocolHelper::defValEnergy(const quint8 &code)
{
    if(code == POLL_CODE_READ_VOLTAGE){
        return QString("UA,UB,UC,IA,IB,IC,PA,PB,PC,QA,QB,QC,cos_fA,cos_fB,cos_fC,F").split(',');
    }else{
        if(code == POLL_CODE_METER_STATUS)
            return QString("evnt_code,comment").split(',');
        //return QString("evnt_code,comment").split(',');

        return QString("A+,A-,R+,R-").split(',');
    }
}
//-----------------------------------------------------------------------------------------
QDateTime MatildaProtocolHelper::dateTimeFromStr(const QString &str)
{
    //ISO8601 strings ("YYYY-MM-DD HH:MM:SS").
        return QDateTime( QDate::fromString(str.left(10), "yyyy-MM-dd"), QTime::fromString(str.right(8), "hh:mm:ss"), Qt::UTC );
}
//-----------------------------------------------------------------------------------------
QStringList MatildaProtocolHelper::varList2strList(const QVariantList &list)
{
    QStringList l;
    for(int i = 0, iMax = list.size(); i < iMax; i++)
        l.append(list.at(i).toString());
    return l;
}
//-----------------------------------------------------------------------------------------
QVariantList MatildaProtocolHelper::strList2VarList(const QStringList &list)
{
    QVariantList l;
    for(int i = 0, iMax = list.size(); i < iMax; i++)
        l.append(list.at(i));
    return l;
}
//-----------------------------------------------------------------------------------------
QVariant MatildaProtocolHelper::strList2Var(const QStringList &list)
{
    QVariantList l;
    for(int i = 0, iMax = list.size(); i < iMax; i++)
        l.append(list.at(i));
    return QVariant(l);
}
//-----------------------------------------------------------------------------------------

QJsonArray MatildaProtocolHelper::arrFromList(const QVariantList &list){ return QJsonArray::fromVariantList(list); }

//-----------------------------------------------------------------------------------------
QJsonArray MatildaProtocolHelper::arrFromList(const QStringList &list){ return QJsonArray::fromStringList(list); }
//-----------------------------------------------------------------------------------------
bool MatildaProtocolHelper::messHshIsValid(const QJsonObject &jObj, QByteArray readArr, int &lastHashSumm)
{
    QStringList lh = MatildaProtocolHelper::getHshNames();
    int hshIndx = -1;


    QString hshName("");
    for(int i = 0, iMax = MatildaProtocolHelper::getHshNames().size(); i < iMax && hshIndx < 0; i++){
        if(jObj.contains(lh.at(i))){
            hshIndx = i;
            hshName = lh.at(i);
            qDebug() << "MatildaProtocolHelper hash is " << hshName;
        }
    }

    if(hshIndx < 0){        
        qDebug() << "MatildaProtocolHelper if(hshIndx < 0 " << hshIndx << readArr;
        return false;
    }else{
        lastHashSumm = hshIndx;
        int startIndx = readArr.indexOf(QString("\"%1\":").arg(hshName));
        QByteArray hshBase64;
        if(startIndx > 0){
            startIndx += hshName.length() + 4;

            int endIndx = readArr.indexOf("\"", startIndx + 1);

            hshBase64 = readArr.mid(startIndx , endIndx - startIndx);
            readArr = readArr.left(startIndx ) + "0" + readArr.mid(endIndx);

        }
        if(hshBase64.isEmpty())
            return false;

        hshBase64 = QByteArray::fromBase64(hshBase64);

        QByteArray myHash = QCryptographicHash::hash(readArr, static_cast<QCryptographicHash::Algorithm>(lastHashSumm));
        if(myHash == hshBase64){
            return true;
        }else{
            qDebug() << "MatildaProtocolHelper if(myHash != hshBase64 " << myHash.toBase64(QByteArray::OmitTrailingEquals) << hshBase64.toBase64(QByteArray::OmitTrailingEquals);
            return false;
        }
    }
}
//-----------------------------------------------------------------------------------------
QStringList MatildaProtocolHelper::codeList4hash(const QString &code, bool &ok)
{


    qDebug() << "codeList4hash " << code ;
    QStringList codeL = code.split(",", QString::SkipEmptyParts);
    ok = true;

    code.toUInt(&ok);
    if(ok){
        codeL.clear();
        if(code.toUInt() != 0)
            codeL.append(code);

        qDebug() << "codeList4hash 9 " << codeL;

        return codeL;
    }
    ok = true;

    if(code.contains(",") || code.contains("-")){ // codeL.isEmpty()){
        codeL = MatildaProtocolHelper::listNiFromLine(code);

        if(codeL.isEmpty())
            codeL = code.split(", ", QString::SkipEmptyParts);

        if(codeL.isEmpty())
            codeL.append(code);

        QList<int> l = SettLoader4matilda::strList2uint8list(codeL, ok);

        qDebug() << codeL << l;

        if(l.contains(0)){
            codeL.clear();
        }else{
            if(ok){

                GlobalHelper::lSort(l);

                if(qMin(l.first(), ZBR_EVENT_OTHER) != ZBR_EVENT_OTHER || qMax(l.last(), ZBR_EVENT_MAX_EVNT_CODE) > ZBR_EVENT_DAY_DONE){
                    ok = false;
                    qDebug() << "lqMin(l.first(), ZBR_EVENT_OTHE" << l.first() << l.last() << qMin(l.first(), ZBR_EVENT_OTHER) << qMax(l.last(), ZBR_EVENT_MAX_EVNT_CODE);

                }

            }else{
                qDebug() << "!ok codeL " << codeL << l;
            }
        }
    }
    qDebug() << "codeList4hash " << codeL;
    return codeL;
}
//-----------------------------------------------------------------------------------------
quint64 MatildaProtocolHelper::calcMaxDataLen(const quint64 &dataLen, quint64 &lastMaxDataLen, bool &buvIneedMoreTime, const qint64 &mynulo4asy, const qint64 &chasovyjMnoznykPereda4i, const qint64 &connSpeed, const qint64 &lastByteWrt)
{


    if(dataLen < 700 && dataLen != 0)
        lastMaxDataLen = 700;
    //        connSpeed = lastByteWrt/connSpeed;
    if(dataLen == 0){

        quint64 dataLen = lastMaxDataLen + MAX_PACKET_LEN_RECOMENDATION;
        if(mynulo4asy <= (chasovyjMnoznykPereda4i * TIME_COEFITIENT) && !buvIneedMoreTime){
            //минуло часу менше ніж закладено, збільшую розмір пакунку
            if( dataLen <= MAX_PACKET_LEN){
                lastMaxDataLen = dataLen;
            }
            if(mynulo4asy <= (chasovyjMnoznykPereda4i * TIME_COEFITIENT * 0.3) && lastMaxDataLen < (100 * MAX_PACKET_LEN_RECOMENDATION)){
                if((lastMaxDataLen * 3) < MAX_PACKET_LEN){
                    lastMaxDataLen *= 3;
                }else{
                    if((lastMaxDataLen + MAX_PACKET_LEN_RECOMENDATION * 3) < MAX_PACKET_LEN)
                        lastMaxDataLen += (MAX_PACKET_LEN_RECOMENDATION * 3);
                }
            }else{
                if(mynulo4asy <= (chasovyjMnoznykPereda4i * TIME_COEFITIENT * 0.5) ){
                    if((lastMaxDataLen * 2) < MAX_PACKET_LEN){
                        lastMaxDataLen *= 2;
                    }else{
                        if((lastMaxDataLen + MAX_PACKET_LEN_RECOMENDATION * 2) < MAX_PACKET_LEN)
                            lastMaxDataLen += (MAX_PACKET_LEN_RECOMENDATION * 2);
                    }
                }
            }


        }else{

            if( (mynulo4asy > (2 * chasovyjMnoznykPereda4i * TIME_COEFITIENT)) ){//якщо на передачу пакунку пішло в двічі більше часу ніж розраховано
                lastMaxDataLen = MAX_PACKET_LEN_RECOMENDATION;
            }else{
                if( lastMaxDataLen > MAX_PACKET_LEN_RECOMENDATION){//якщо на передачу пакунку пішло більше часу ніж розраховано
                    lastMaxDataLen -= MAX_PACKET_LEN_RECOMENDATION;
                }
            }
        }
        qDebug() << "calcMaxDataLen " << connSpeed << mynulo4asy << lastByteWrt << lastMaxDataLen << chasovyjMnoznykPereda4i;

    }
    buvIneedMoreTime = false;

    if(lastMaxDataLen > MAX_PACKET_LEN)
        lastMaxDataLen = MAX_PACKET_LEN;
    else if(lastMaxDataLen < MIN_AUTO_PACKET_LEN){
        lastMaxDataLen = MIN_AUTO_PACKET_LEN;
    }
    return lastMaxDataLen;
}
//-----------------------------------------------------------------------------------------
qint64 MatildaProtocolHelper::calcMaxDataLenSigned(const qint64 &dataLen, qint64 &lastMaxDataLenSigned, bool &buvIneedMoreTime, const qint64 &mynulo4asy, const qint64 &chasovyjMnoznykPereda4i, const qint64 &connSpeed, const qint64 &lastByteWrt)
{


    if(dataLen < 700 && dataLen != 0)
        lastMaxDataLenSigned = 700;
    //        connSpeed = lastByteWrt/connSpeed;
    if(dataLen == 0){

        qint64 dataLen = lastMaxDataLenSigned + MAX_PACKET_LEN_RECOMENDATION;
        if(mynulo4asy <= (chasovyjMnoznykPereda4i * TIME_COEFITIENT) && !buvIneedMoreTime){
            //минуло часу менше ніж закладено, збільшую розмір пакунку
            if( dataLen <= MAX_PACKET_LEN){
                lastMaxDataLenSigned = dataLen;
            }

            if(mynulo4asy <= (chasovyjMnoznykPereda4i * TIME_COEFITIENT * 0.3) && lastMaxDataLenSigned < (100 * MAX_PACKET_LEN_RECOMENDATION)){
                if((lastMaxDataLenSigned * 3) < MAX_PACKET_LEN){
                    lastMaxDataLenSigned *= 2.7;
                }else{
                    if((lastMaxDataLenSigned + MAX_PACKET_LEN_RECOMENDATION * 3) < MAX_PACKET_LEN)
                        lastMaxDataLenSigned += (MAX_PACKET_LEN_RECOMENDATION * 3);
                }
            }else{
                if(mynulo4asy <= (chasovyjMnoznykPereda4i * TIME_COEFITIENT * 0.5)){
                    if((lastMaxDataLenSigned * 2) < MAX_PACKET_LEN){
                        lastMaxDataLenSigned *= 1.8;
                    }else{
                        if((lastMaxDataLenSigned + MAX_PACKET_LEN_RECOMENDATION * 2) < MAX_PACKET_LEN)
                            lastMaxDataLenSigned += (MAX_PACKET_LEN_RECOMENDATION * 2);
                    }
                }
            }

        }else{

            if( (mynulo4asy > (2 * chasovyjMnoznykPereda4i * TIME_COEFITIENT)) ){//якщо на передачу пакунку пішло в двічі більше часу ніж розраховано
                lastMaxDataLenSigned = MAX_PACKET_LEN_RECOMENDATION;
            }else{
                if( lastMaxDataLenSigned > MAX_PACKET_LEN_RECOMENDATION){//якщо на передачу пакунку пішло більше часу ніж розраховано
                    lastMaxDataLenSigned -= MAX_PACKET_LEN_RECOMENDATION;
                }
            }
        }
        qDebug() << "calcMaxDataLen " << connSpeed << mynulo4asy << lastByteWrt << lastMaxDataLenSigned << chasovyjMnoznykPereda4i;

    }
    buvIneedMoreTime = false;
    if(lastMaxDataLenSigned > MAX_PACKET_LEN)
        lastMaxDataLenSigned = MAX_PACKET_LEN;
    else if(lastMaxDataLenSigned < MIN_AUTO_PACKET_LEN){
        lastMaxDataLenSigned = MIN_AUTO_PACKET_LEN;
    }
    return lastMaxDataLenSigned;
}
//-----------------------------------------------------------------------------------------
bool MatildaProtocolHelper::fileIsExists(const QString &fileName)
{
    QFileInfo fi(fileName);
    return fi.exists();
}
//-----------------------------------------------------------------------------------------
bool MatildaProtocolHelper::urlIsValid(const QString &strUrl)
{
    QUrl url(strUrl, QUrl::StrictMode);
    return (url.isValid() && !strUrl.contains(" ") && !strUrl.contains(">") && !strUrl.contains("{"));
}
//-----------------------------------------------------------------------------------------
QVariantHash MatildaProtocolHelper::loadAboutPlugin()
{
    QVariantHash hashAboutPlugin;

    QDir pluginsDir(QString("%1/plugin").arg(SettLoader4matilda::path2workDir()));



    if(!pluginsDir.exists()){
        qDebug() << "plugin dir not found" << qApp->applicationDirPath();
         QStringList l;
         l << "-" << "-" << "-";
         hashAboutPlugin.insert("not plugin", l);

        return hashAboutPlugin;
    }


    QString plgNames;
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (pluginLoader.isLoaded() && plugin != nullptr) {
            MeterPlugin *pluginsInterface = qobject_cast<MeterPlugin *>(plugin);
            if (pluginsInterface != nullptr){

                if(pluginsInterface->getPluginVersion() != PLG_VER_1_0){
                    pluginLoader.unload();
                    qDebug() << "unknown plg" << fileName;
                    continue;

                }

                QString keyName = pluginsInterface->getMeterType();
                //                QByteArray defPasswd = pluginsInterface->getDefaultPasswd();
//                quint8 passwdType = pluginsInterface->getPasswdType();
                //0 - unknown
                //1 - text
                //2 - digit
                //3 - hex


                QStringList l;

                QFileInfo fi(pluginsDir.absoluteFilePath(fileName));
                l.append(fi.birthTime().toString("yyyy-MM-dd hh:mm:ss"));
                l.append(pluginsInterface->getVersion());
                l.append(pluginsInterface->getSupportedMeterList());
                l.append(pluginsInterface->getMeterAddrsAndPsswdRegExp());//^(xxx)$^(xxx)$
//                qDebug() << "load plugin " << keyName << l;
                hashAboutPlugin.insert(keyName, l);

                plgNames.append(keyName + " ");
            }

        }

        if(plugin != nullptr){
            try{
                pluginLoader.unload();
            }catch(...){
                qDebug() << "err unload plugin";
            }
        }

    }
    return hashAboutPlugin;
}
//-----------------------------------------------------------------------------------------
bool MatildaProtocolHelper::readMeterListHashSumm(QByteArray &hsh, QString &errStr, const int &alg, const bool &isJson, QStringList listColmns)
{
    //зафіксувати цей порядок ключів "model_NI_SN_memo_passwd_on_politic_trff_vrsn"
    QStringList listKeys = QString("model,NI,SN,memo,passwd,on,politic,trff,vrsn,crdnts").split(",");

    QStringList listCol4hsh;

    if(listColmns.isEmpty()){
        listCol4hsh = listKeys;
    }else{
        for(int i = 0, iMax = listKeys.size(); i < iMax; i++){
            if(listColmns.contains(listKeys.at(i)))
                listCol4hsh.append(listKeys.at(i));
        }
    }


    int colSize = listCol4hsh.size();

    if(listCol4hsh.isEmpty() || (colSize != listColmns.size() && !listColmns.isEmpty())){
//        s_command = COMMAND_ERROR_CODE;
//        s_data = MatildaProtocolHelper::errCodeLastOperation(command, ERR_INCORRECT_REQUEST);
        errStr = "";
        return false;
    }

    QVariantList listMeter;
    if(true){
        QFile file(QString("%1/meter").arg(SettLoader4matilda::path2config()));
        if(file.open(QFile::ReadOnly)){
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Qt_5_6);

            while(!stream.atEnd()){
                QVariantHash hashOneMeter;
                stream >> hashOneMeter;

                QVariantList listhOneMeterTmp;

                for(int j = 0; j < colSize; j++)
                    listhOneMeterTmp.append(hashOneMeter.value(listCol4hsh.at(j)));

                listMeter.append(listhOneMeterTmp);
            }
            file.close();
        }else{
//            s_command = COMMAND_ERROR_CODE_EXT;
//            s_data = MatildaProtocolHelper::preparyExtError(tr("Can't open file. Error: %1").arg(file.errorString()), ERR_INTERNAL_ERROR, command);
            errStr = file.errorString();
            return false;
        }
    }

    QByteArray arr;
    if(true){
        QBuffer file(&arr);
        file.open(QIODevice::WriteOnly);
        QDataStream stream(&file);

        stream.setVersion(QDataStream::Qt_5_6);
        stream << listMeter;

        file.close();
    }

    if(true){
        QBuffer file(&arr);
        file.open(QIODevice::ReadOnly);

        hsh = QCryptographicHash::hash(file.readAll(),static_cast<QCryptographicHash::Algorithm>(alg) );
        if(isJson)
            hsh = hsh.toBase64(QByteArray::OmitTrailingEquals);
        if(!listColmns.isEmpty())
            errStr = listCol4hsh.join("\n");
        file.close();
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------------------
QByteArray MatildaProtocolHelper::runProc(const QString &progName, const QStringList args, const int timeLimitMsec, const QByteArray endArr, const bool chopEndArr, const bool mergedChannels)
{
    QTime time;
    time.start();

    QProcess proc;
    if(mergedChannels)
        proc.setProcessChannelMode(QProcess::MergedChannels);

    proc.start( QVariant(progName).toByteArray() ,args);


    if(!proc.waitForStarted(2000)){
        qDebug() << "MatildaProtocolHelper runProc " << progName << args << timeLimitMsec << endArr << chopEndArr;
        qDebug() << "MatildaProtocolHelper procErr=" << proc.errorString();
    }

   proc.waitForReadyRead(300);

    QByteArray readArr = proc.readAll();

    while(time.elapsed() < timeLimitMsec && (proc.waitForReadyRead(200) || readArr.isEmpty()) && (endArr.isEmpty() || (!endArr.isEmpty() && endArr != readArr.right(endArr.length()))) ){
        proc.waitForReadyRead(200);
        readArr.append(proc.readAll());

        if(proc.state() != QProcess::Running)
            break;
    }

    proc.close();

    if(chopEndArr && !endArr.isEmpty() && endArr == readArr.right(endArr.length()))
        readArr.chop(endArr.length());

    return readArr;
}
//-----------------------------------------------------------------------------------------
QByteArray MatildaProtocolHelper::runBashProc(const QByteArray &bashCommand, const int timeLimitMsec)
{


    qDebug() << "runBashProc " << bashCommand << timeLimitMsec;
    QTime time;
    time.start();

    QProcess proc;
    proc.setStandardErrorFile(QProcess::nullDevice());


    proc.start("bash");
    proc.waitForStarted(2000);
    qDebug() << "procErr=" << proc.errorString();
    proc.write(bashCommand +" && exit\n");//create symlint to zonefile forced and exit

    proc.waitForReadyRead(400);

    QByteArray readArr = proc.readAll();

    while(time.elapsed() < timeLimitMsec && proc.waitForReadyRead(200)  ){
        proc.waitForReadyRead(200);
        readArr.append(proc.readAll());
        if(proc.state() != QProcess::Running)
            break;
    }
    proc.close();
    return readArr;
}
//-----------------------------------------------------------------------------------------
QString MatildaProtocolHelper::getInfoAboutSystem()
{
    QString readArr;
#ifdef VERSION_4_PC
    QStringList lLeft, lRight;
    lLeft = QString("CPU (build/run):Kernel type:Kernel version:HostName:ProductName").split(":");
    lRight.append(QSysInfo::buildCpuArchitecture() + "/" + QSysInfo::currentCpuArchitecture());
    lRight.append(QSysInfo::kernelType());
    lRight.append(QSysInfo::kernelType());
    lRight.append(QSysInfo::machineHostName());
    lRight.append(QSysInfo::prettyProductName());

    int maxLen = 0;
    for(int i = 0, iMax = lLeft.size(); i < iMax; i++){
        if(lLeft.at(i).length() > maxLen)
            maxLen = lLeft.at(i).length();
    }
    for(int i = 0, iMax = lLeft.size(); i < iMax; i++)
        readArr.append( lLeft.at(i).rightJustified(maxLen) + ": " + lRight.at(i) + "\n" );
#else
    readArr = MatildaProtocolHelper::runProc(SettLoader4matilda::path2script() + "/sysSettings.sh", QStringList(), 4000, "END\n", true);

#endif
    return readArr;
}
//-----------------------------------------------------------------------------------------
QByteArray MatildaProtocolHelper::getTaskInfo(bool &ok)
{

#ifdef VERSION_4_PC
    ok = true;
    return "not allowed(\n";
#else
    ok = true;
    return MatildaProtocolHelper::runProc(SettLoader4matilda::path2script() + "/taskInfo.sh");
#endif
}
//-----------------------------------------------------------------------------------------
QByteArray MatildaProtocolHelper::getNtpServices(bool &ok)
{
    ok = false;
    QByteArray arr("");

    #ifdef VERSION_4_PC
    return "";
    #else
    QFile file("/etc/systemd/timesyncd.conf");
    if(file.open(QFile::ReadOnly)){
        QString readArr = file.readAll();
        if(readArr.contains("[Time]") && readArr.contains("\nServers=")){
            readArr = readArr.mid(readArr.indexOf("\nServers=") + 9);
            if(readArr.contains("\n"))
                readArr = readArr.left(readArr.indexOf("\n"));
        }else{
            qDebug() << "time " << readArr.contains("[Time]") << readArr.contains("\nServers=")  << readArr;
        }
        file.close();
        ok = true;

        if(!readArr.isEmpty())
            arr = QVariant(readArr).toByteArray();
    }
    #endif
    return arr;
}
//-----------------------------------------------------------------------------------------

bool MatildaProtocolHelper::isIpAddress(const QString &s){ return (QHostAddress(s).protocol() == QAbstractSocket::IPv4Protocol || QHostAddress(s).protocol() == QAbstractSocket::IPv6Protocol); }

//-----------------------------------------------------------------------------------------
void MatildaProtocolHelper::showDebugOut4varHash(const QVariantHash &h, const QString &firstLine)
{


    QList<QString> lk = h.keys();
    qDebug() << "---Start:" << firstLine;
    for(int i = 0, iMax = lk.size(); i < iMax; i++)
        qDebug() << lk.at(i) << h.value(lk.at(i));
    qDebug() << "---End: " << firstLine;
}
//-----------------------------------------------------------------------------------------
int MatildaProtocolHelper::calcTMsecCheck(int tMSec, bool &tMSecCheck)
{
    tMSecCheck = true;
    if(tMSec < 1){
        tMSec = 0xFFFFFFF;//268 435 455 ms   4 473,92425 sec
        tMSecCheck = false;
    }else{
        if(tMSec < 700)
            tMSec = 700;
    }
    return tMSec;
}
//-----------------------------------------------------------------------------------------
void MatildaProtocolHelper::syncRtcWithSystemcClock()
{
    QFileInfo fi;
    QStringList listRtc;
    for(int i = 0; i < 5 ; i++){
        fi.setFile(QString("/dev/rtc%1").arg(i));
        if(fi.exists())
            listRtc.prepend("hwclock --systohc -f " + QString("/dev/rtc%1").arg(i));
    }

    qDebug() << "syncRtcWithSystemcClock list=" << QVariant(listRtc.join(" && ")).toByteArray();

    if(!listRtc.isEmpty())
        qDebug() << "syncRtcWithSystemcClock rez=" << MatildaProtocolHelper::runBashProc( QVariant(listRtc.join(" && ")).toByteArray(), listRtc.size() * 5000);
}
//-----------------------------------------------------------------------------------------
bool MatildaProtocolHelper::checkCreateDir(const QString &path2dir)
{
    QDir dir(path2dir);
    return (dir.exists() || dir.mkpath(path2dir));
}
//-----------------------------------------------------------------------------------------
QString MatildaProtocolHelper::getDevSerialNumber()
{
#ifdef VERSION_4_PC
    QString sn("EMUL1");
#ifdef Q_OS_ANDROID
   sn = SettLoader::loadSett(SETT_MAIN_UDP_ID).toString();
#endif

#else
    QString sn = runBashProc("/usr/bin/bb-show-serial.sh");
#endif
    if(sn.isEmpty())
        sn = "0";
    sn = sn.simplified().trimmed();
    return sn;
}

//-----------------------------------------------------------------------------------------

QByteArray MatildaProtocolHelper::compressVarList(const QVariantList &varList, const bool addSize)//= true
{
    QByteArray writeArr;
    QDataStream stream(&writeArr, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_6);
    if(addSize)
        stream << (quint32)varList.size() + 1;
    stream << varList;
    return qCompress(writeArr, 9);
}

//-----------------------------------------------------------------------------------------

QVariantList MatildaProtocolHelper::uncompress2varList(const QByteArray &inArr, bool &ok, const bool hasSize)
{
    ok = false;
    QVariantList l;
    QByteArray arr = qUncompress(inArr);
    if(arr.isEmpty())
        return l;
    QDataStream stream(&arr, QIODevice::ReadOnly);
    quint32 s;
    if(!stream.atEnd()){
        if(hasSize)
            stream >> s ;
        stream >> l;
        if(hasSize){
            s--;
            ok = (s == (quint32)l.size());
        }else{
            ok = true;
        }
    }
    return l;
}
//-----------------------------------------------------------------------------------------
QByteArray MatildaProtocolHelper::compressVarHash(const QVariantHash &hash, const bool addSize)
{
    QByteArray writeArr;
    QDataStream stream(&writeArr, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_6);
    if(addSize)
        stream << (quint32)hash.size() + 1;
    stream << hash;
    return qCompress(writeArr, 9);
}
//-----------------------------------------------------------------------------------------
QVariantHash MatildaProtocolHelper::uncompress2varHash(const QByteArray &inArr, bool &ok, const bool hasSize)
{
    ok = false;
    QVariantHash h;
    QByteArray arr = qUncompress(inArr);
    if(arr.isEmpty())
        return h;
    QDataStream stream(&arr, QIODevice::ReadOnly);
    quint32 s;
    if(!stream.atEnd()){
        if(hasSize)
            stream >> s ;
        stream >> h;
        if(hasSize){
            s--;
            ok = (s == (quint32)h.size());
        }else{
            ok = true;
        }
    }
    return h;
}

//--------------------------------------------------------------



//QString MatildaProtocolHelper::getKeysL edLamp()         { return QString("lamp_ni,grp_id,crdnts,pll,memo")                      ; }

QString MatildaProtocolHelper::getKeysLedLamp()         { return QString("NI dt pwr pstrtw pstrt pnaw pna tnaw tna grpw grp pll crdnts stts sync memo mdl strt type"); }

//"Model,NI,Group,Last Exchange,Power [%],Start Power [%],NA Power [%],Tna [sec],Coordinates,Poll On/Off,Street,Memo"
QString MatildaProtocolHelper::getKeysLedLampCol()         { return QString("mdl NI grp dt pwr pstrt pna tna crdnts pll strt memo"); }

QString MatildaProtocolHelper::getKeysLedLampRW()       { return QString("NI grp pstrt pna tna crdnts pll memo mdl strt")                      ; }


//Model,NI,Group,Last Exchange,Power,Start Power,NA Power,Tna,Coordinates,Poll On/Off,Street,Memo
QString MatildaProtocolHelper::getKeysLedLampWriteOnly(){ return QString("mdl NI grpw pstrtw pnaw tnaw crdnts pll strt memo type")  ; }

QString MatildaProtocolHelper::getKeysMeterList()       { return QString("model,SN,NI,memo,passwd,on,politic,trff,crdnts,vrsn") ;}

//-----------------------------------------------------------------------------------------

QHash<QString, QString> MatildaProtocolHelper::strHashFromQsl(const QStringList &l, const QString &separ, QList<QString> &lk, const int key, const int val, const bool oneMode)
{
    QHash<QString,QString> h;
    lk.clear();
    for(int i = 0, iMax = l.size(); i < iMax; i++){
        QStringList ll = l.at(i).split(separ);
        if(ll.isEmpty())
            continue;
        int s = ll.size();
        if(s <= key || s <= val)
            continue;

        h.insert(ll.at(key), oneMode ? ll.at(val) : ll.mid(val).join(separ));

    }
    return h;
}
//-----------------------------------------------------------------------------------------
QStringList MatildaProtocolHelper::qslFromHash(const QHash<QString, QString> &h, const QString &separ, QList<QString> lk)
{
    if(lk.isEmpty()){
        lk = h.keys();
        GlobalHelper::lSort(lk);
    }

    QStringList l;
    for(int i = 0, iMax = lk.size(); i < iMax; i++)
        l.append(lk.at(i) + separ + h.value(lk.at(i)));
    return l;
}
//-----------------------------------------------------------------------------------------
QString MatildaProtocolHelper::evcodeV2toV1(QString evLine, QString &lastModel, const QString defModel)
{
    lastModel.clear();
    QStringList ll = evLine.split(" ");
    if(ll.size() > 2){
        //<UCM> <Model> <Meter Ev Code>   to 0x00 <comment>
        ll.removeFirst();
        lastModel = ll.takeFirst();
        evLine = ll.takeFirst();
        if(evLine.left(1) == "_" && !ll.isEmpty())
            evLine = evLine.mid(1) + " " + ll.takeFirst();

        evLine.append(" <" + QString(ll.isEmpty() ? "-" : ll.join(" ")) + " >");

    }
    if(lastModel.isEmpty())
        lastModel = defModel;
    return evLine;
}
//-----------------------------------------------------------------------------------------
QHash<QString, QString> MatildaProtocolHelper::getMeterSn2meterModel()
{
    QHash<QString,QString> h;
    QFile file(QString("%1/meter").arg(SettLoader4matilda::path2config()));
    if(file.open(QFile::ReadOnly)){
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_5_6);
        while(!stream.atEnd()){
            QVariantHash hashOneMeter;
            stream >> hashOneMeter;
            if(hashOneMeter.value("SN").toString().isEmpty() || hashOneMeter.value("model").toString().isEmpty())
                continue;
            h.insert(hashOneMeter.value("SN").toString(), hashOneMeter.value("model").toString());
        }
        file.close();
    }
    return h;
}
//-----------------------------------------------------------------------------------------
QString MatildaProtocolHelper::validateCoordinates(const QString &instr, bool &ok)
{
    ok = false;
    QString s = instr.simplified().trimmed();

    if(s.isEmpty()){
        ok = true;
        return s;
    }
    QStringList l = s.split(",", QString::SkipEmptyParts);
    s.clear();
    if(l.size() == 2){
        bool oklat ,oklon;
        qreal lat = l.first().toFloat(&oklat);
        qreal lon = l.last().toFloat(&oklon);

        if(oklat && oklon && lat <= 90.0 && lat >= (-90.0)){
            quint32 cntr = 0, cntrMax = 0xFFFFFF;
            if(lon > 180.0){
                while(lon > 180.0 && cntr < cntrMax){
                    lon -= 360;
                    cntr++;
                }
            }else if(lon < -180.0){
                while(lon < -180.0 && cntr < cntrMax){
                    lon += 360;
                    cntr++;
                }
            }
            if(cntr < cntrMax){
                s = QString::number(lat, 'f', 6).replace(",", ".") + "," + QString::number(lon, 'f', 6).replace(",", ".");
                ok = true;
            }
        }
    }
    return s;
}
//-----------------------------------------------------------------------------------------
QStringList MatildaProtocolHelper::addLine2list(QString line, QStringList log, const int &maxLogSize, const QString timeStampMask)
{
    if(!timeStampMask.isEmpty())
        line.prepend(QDateTime::currentDateTime().toString(timeStampMask) + " ");
    log.append(line);
    int logs = log.size();
    if(logs > maxLogSize)
        log = log.mid(logs - maxLogSize);
    return log;
}
//-----------------------------------------------------------------------------------------
QStringList MatildaProtocolHelper::addLines2list(QStringList lines, QStringList log, const int &maxLogSize, const QString timeStampMask)
{
    QString lpart;
    if(!timeStampMask.isEmpty())
        lpart = QDateTime::currentDateTime().toString(timeStampMask) + " ";
    for(int i = 0, iMax = lines.size(); i < iMax; i++)
        log.append(lpart + lines.at(i));
    int logs = log.size();
    if(logs > maxLogSize)
        log = log.mid(logs - maxLogSize);
    return log;
}
//-----------------------------------------------------------------------------------------
QByteArray MatildaProtocolHelper::calcFileSha1(const QString &fileName, bool &ok)
{
    //for files < 1MB - readAll, else readPart(1MB)/ MAX 10MB
    QFileInfo fi(fileName);
    qint64 fileSize = fi.size();

    ok = false;
    QFile file(fileName);
    QByteArray sha1;
    if(file.open(QFile::ReadOnly)){
        for(qint64 i = 0, readLen = 1000000, j = 0; j < 10 && i < fileSize; j++, i += readLen){
            QByteArray r = file.read(readLen);
            if(r.isEmpty())
                continue;
            sha1 = QCryptographicHash::hash(sha1 + r, QCryptographicHash::Sha1);

        }
        file.close();
        ok = !sha1.isEmpty();
    }
    return sha1;
}
//-----------------------------------------------------------------------------------------
QVariantHash MatildaProtocolHelper::getLastAboutObject(const QString &version, const QString &devId, const int &devType)
{
    /*(if exists)
     *
     * ID
     * SN <serial number>
     * vrsn
     * DEV
     * app
     * MAC<x>   0 < x < 10
     *
     * //gsm section
     * IMEI
     * IMSI
     * CID
     * BAND
     * RSSI
     * ATI
     * RCSP
     * Ec/No
     *
     * //zigbee
     * ZCH
     * ZID
     * ZRSSI
     * LQI
     * VR
     * HV
     * Type
     * EUI64
     *
*/
    QVariantHash h;
    h.insert("SN", getDevSerialNumber());
    h.insert("vrsn", MATILDA_PROTOCOL_VERSION);
    h.insert("DEV", devType);
    h.insert("app", version);
    h.insert("ID", devId);

   QStringList lmacs;

    QList<QNetworkInterface> listIface = QNetworkInterface::allInterfaces();
    for(int i = 0, iMax = listIface.size(), macCounter = 0; i < iMax && macCounter < 10; i++){

        if(listIface.at(i).isValid()){

            if(!listIface.at(i).hardwareAddress().startsWith("00:00:00:00:00") && !listIface.at(i).hardwareAddress().isEmpty() && listIface.at(i).hardwareAddress().length() >= 10){
                lmacs.append(listIface.at(i).hardwareAddress());
                macCounter++;
            }
        }
    }
#ifdef Q_OS_ANDROID

    if(lmacs.isEmpty()){
        QAndroidJniObject s = QAndroidJniObject::callStaticObjectMethod<jstring>
                ("ua/ktsintek/zbyrator2/CustomStorageInfo", QByteArray("getMacAddress").data());
        qDebug() << "mac java = " << s.toString();
        if(!s.toString().isEmpty())
        lmacs.append(s.toString().toUpper());
        else
            lmacs.append("03:00:00:00:00:00");
    }
#endif
    if(!lmacs.isEmpty()){
        std::sort(lmacs.begin(), lmacs.end());
        for(int i = 0; !lmacs.isEmpty(); i++)
            h.insert(QString("MAC%1").arg(i), lmacs.takeFirst());

    }
#ifndef Q_OS_ANDROID
    addData2hash(h, readFromSharedMemory(MTD_EXT_NAME_ABOUT_GSM), QString("IMEI IMSI CellID LAC RSSI RCSP ATI Ec/No").split(" "), QString("IMEI IMSI CID LAC RSSI RCSP ATI EcNo").split(" "));
    addData2hash(h, readFromSharedMemory(MTD_EXT_NAME_ABOUT_ZB), QString("Channel,Network ID,RSSI,LQI,VR,HV,Type,SN (EUI64)").split(","),
                 QString("ZCH,ZID,ZRSSI,LQI,VR,HV,Type,EUI64").split(","));
#endif
    return h;
}
//-----------------------------------------------------------------------------------------
void MatildaProtocolHelper::addData2hash(QVariantHash &mainHash, const QVariantHash &h, const QStringList &lk, const QStringList &lk4maint)
{
    for(int i = 0, iMax = lk.size(), iMax2 = lk4maint.size(); i < iMax; i++){
        if(h.contains(lk.at(i)))
            mainHash.insert( (i < iMax2) ? lk4maint.at(i) : lk.at(i), h.value(lk.at(i)).toString().simplified().replace(" ", "_"));
    }
}
//-----------------------------------------------------------------------------------------
void MatildaProtocolHelper::checkHostName()
{

    QString devNameStr = SettLoader4matilda().loadOneSett(SETT_ABOUT_MEMOLINE).toString().simplified().toLatin1();

    devNameStr = devNameStr.remove(" ");
    devNameStr = devNameStr.remove("?");//Latin1 not ASCII char is '?'

    if(devNameStr.isEmpty())
        devNameStr = "Matilda";

    QByteArray devName = devNameStr.toLatin1();


    QFile file("/etc/hostname");
    QByteArray hostName;
    if(file.open(QFile::ReadOnly))
        hostName = file.readAll().simplified().trimmed();
    file.close();

    if(hostName != devName){
        QSaveFile sFile("/etc/hostname");
        if(sFile.open(QSaveFile::WriteOnly|QSaveFile::Unbuffered)){                        
            devName.append("\n");
            sFile.write(devName);
            sFile.commit();
        }

    }

}
//-----------------------------------------------------------------------------------------
QString MatildaProtocolHelper::devType2str(const int &devType)
{
    QString s;
    switch(devType){
    case DEV_UNKNWN          : s = "DEV_UNKNWN"          ; break;
    case DEV_POLL            : s = "DEV_POLL"            ; break;//  1// пристрій опитування лічильників. Режими: опитування, шлюз, зберігання і передача даних, + street lightings system
    case DEV_STOR            : s = "DEV_STOR"            ; break;//  2// пристрій зберігання даних. Режими: зберігання і передача даних (дані можуть додаватись з іншого джерела, наприклад опитування лічильників ПЗ Збриратор )
    case DEV_GATE            : s = "DEV_GATE"            ; break;// 3// пристрій який працює в режимі шлюзу
    case DEV_SVAHA           : s = "DEV_SVAHA"           ; break;//  4// пристрій який комутує сокет активного клієнта матільди і сокет ПЗ конфігурації

    case DEV_POLL_EMULATOR_L0: s = "DEV_POLL_EMULATOR_L0"; break;// 20// емуляція пристрою опитування лічильників. Режими: опитування, зберігання і передача даних (урізана версія DEV_POLL)
    case DEV_POLL_EMULATOR_L1: s = "DEV_POLL_EMULATOR_L1"; break;// 21// емуляція пристрою опитування лічильників. Режими: опитування, шлюз (через протокол матільди), зберігання і передача даних (урізана версія DEV_POLL)
    case DEV_POLL_EMULATOR_L2: s = "DEV_POLL_EMULATOR_L2"; break;// 22// емуляція пристрою опитування лічильників. Режими: опитування, шлюз (в режимі сервісу), зберігання і передача даних (урізана версія DEV_POLL)
    default: s = "NaN"; break;
    }
    return s;
}
//-----------------------------------------------------------------------------------------
QStringList MatildaProtocolHelper::getSyncDirPaths(const QString &workDir)
{
    QStringList l;
    QDir dir(workDir);
    //<work dir>/<year>/<month>/<files>
    QStringList lYears = dir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot);
    for(int i = 0, iMax = lYears.size(); i < iMax; i++){
        QString yearPath = QString("%1/%2").arg(workDir).arg(lYears.at(i));
        dir.setPath(yearPath);
        QStringList lMonth = dir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot);
        for(int j = 0, jMax = lMonth.size(); j < jMax; j++)
            l.append(yearPath + "/" + lMonth.at(j));
    }
    return l;
}
//-----------------------------------------------------------------------------------------
QHash<QString, QStringList> MatildaProtocolHelper::getSyncDirPathsFiles(const QString &workDir, QStringList &lk)
{
    lk.clear();
    QHash<QString, QStringList> h;
    QDir dir(workDir);
    //<work dir>/<year>/<month>/<files>
    QStringList lYears = dir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot);
    for(int i = 0, iMax = lYears.size(); i < iMax; i++){
        QString yearPath = QString("%1/%2").arg(workDir).arg(lYears.at(i));
        dir.setPath(yearPath);
        QStringList lMonth = dir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot);
        for(int j = 0, jMax = lMonth.size(); j < jMax; j++){
            QString monthPath = yearPath + "/" + lMonth.at(j);
            lk.append(monthPath);
            dir.setPath(monthPath);

            QStringList fileNames = dir.entryList(QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Readable, QDir::Time);
            if(fileNames.isEmpty())
                continue;
            h.insert(monthPath, fileNames);
        }
    }
    if(!lk.isEmpty())
        std::sort(lk.begin(), lk.end());
    h.insert("wd", workDir.split("\r\n\t"));
    return h;
}
//-----------------------------------------------------------------------------------------
bool MatildaProtocolHelper::isCommandAllowed4thisDev(const quint16 &command, const int &devType)
{
    if(command > COMMAND_ZULU && command < COMMAND_READ_SYSTEM_SETTINGS)
        return true;//4all dev
    bool r = false;
    switch(devType){
    case DEV_SVAHA: r = getCommand4devSvaha().contains(command); break;
    case DEV_POLL : r = getCommand4devPoll().contains(command); break;
    }
    return r;
}
//-----------------------------------------------------------------------------------------
QList<quint16> MatildaProtocolHelper::getCommand4devSvaha()
{
    return QList<quint16>()
            << COMMAND_READ_SYSTEM_SETTINGS      << COMMAND_READ_TASK_INFO         << COMMAND_READ_DATE_SETT            << COMMAND_READ_GPRS_SETT           << COMMAND_READ_STATE
            << COMMAND_READ_IFCONFIG             << COMMAND_READ_APP_LOG           << COMMAND_READ_ABOUT_OBJECT         << COMMAND_READ_IP_FILTER_SETT      << COMMAND_READ_MATILDA_AC_SETT
            << COMMAND_READ_BACKUP_LIST          << COMMAND_READ_UDP_BEACON        << COMMAND_READ_TCP_SETT             << COMMAND_READ_IPTABLE             << COMMAND_READ_PPP_SUPERVISOR
            << COMMAND_READ_DEVICE_SERIAL_NUMBER << COMMAND_READ_SVAHA_CONN_SETT   << COMMAND_READ_SVAHA_BACKUP_SETT    << COMMAND_READ_CONNECT_LIST        << COMMAND_READ_BACKUPLIST_FRAMED

            << COMMAND_READ_SNTP_SETT            << COMMAND_READ_SNTP_LOG_EV       << COMMAND_READ_SNTP_LOG_ERR         << COMMAND_READ_SNTP_LOG_WARN       << COMMAND_READ_SNTP_IP_HISTORY

            << COMMAND_WRITE_FIRST_4_OPERATOR    << COMMAND_WRITE_DATE_SETT        << COMMAND_WRITE_RESET_MODEM         << COMMAND_WRITE_GET_BACKUPFILE     << COMMAND_WRITE_SVAHA_KILL_CLIENT


            << COMMAND_WRITE_FIRST
            << COMMAND_WRITE_UPGRADE             << COMMAND_WRITE_GPRS_SETT        << COMMAND_WRITE_REBOOT              << COMMAND_WRITE_DAEMON_RESTART     << COMMAND_WRITE_FULL_UPGRADE
            << COMMAND_WRITE_ROOT_LOGIN_PSWD     << COMMAND_WRITE_GUEST_LOGIN_PSWD << COMMAND_WRITE_OPEARTOR_LOGIN_PSWD << COMMAND_WRITE_TIMEZONE           << COMMAND_WRITE_NTP_SETTINGS
            << COMMAND_WRITE_ABOUT_OBJECT        << COMMAND_WRITE_IP_FILTER_SETT   << COMMAND_WRITE_MATILDA_AC_SETT     << COMMAND_WRITE_DELETE_BACKUP_FILE << COMMAND_WRITE_BACK_IN_TIME
            << COMMAND_WRITE_CREATE_BACK_IN_TIME << COMMAND_WRITE_UDP_BEACON       << COMMAND_WRITE_TCP_SETT            << COMMAND_WRITE_COMMAND2BASH       << COMMAND_GET_BACKUP_FILE
            << COMMAND_PUSH_BACKUP_FILE_AND_APPLY<< COMMAND_WRITE_IPTABLE          << COMMAND_WRITE_PPP_SUPERVISOR      << COMMAND_WRITE_SVAHA_CONN_SETT    << COMMAND_WRITE_SVAHA_BACKUP_SETT
            << COMMAND_WRITE_SNTP_SETT;

}
//-----------------------------------------------------------------------------------------
QList<quint16> MatildaProtocolHelper::getCommand4devPoll()
{
    return QList<quint16>()
            << COMMAND_READ_SYSTEM_SETTINGS         << COMMAND_READ_TASK_INFO               << COMMAND_READ_ABOUT_PLG               << COMMAND_READ_DATE_SETT           << COMMAND_READ_GPRS_SETT
            << COMMAND_READ_STATE                   << COMMAND_READ_IFCONFIG                << COMMAND_READ_APP_LOG                 << COMMAND_READ_POLL_SCHEDULE       << COMMAND_READ_METER_LIST
            << COMMAND_READ_DATABASE                << COMMAND_READ_DATABASE_GET_TABLES     << COMMAND_READ_DATABASE_GET_VAL        << COMMAND_READ_METER_LOGS          << COMMAND_READ_METER_LOGS_GET_TABLES
            << COMMAND_READ_METER_LOGS_GET_VAL      << COMMAND_READ_METER_LIST_FRAMED       << COMMAND_READ_DATABASE_TABLES_PARSING << COMMAND_READ_ZBR_LOG             << COMMAND_READ_ABOUT_OBJECT
            << COMMAND_READ_POLL_SETT               << COMMAND_READ_POLL_STATISTIC          << COMMAND_READ_TABLE_HASH_SUMM         << COMMAND_READ_IP_FILTER_SETT      << COMMAND_READ_METER_LIST_HASH_SUMM
            << COMMAND_READ_SERIAL_LOG              << COMMAND_READ_COMMANDS                << COMMAND_READ_DA_DATA_FROM_COORDINATOR<< COMMAND_READ_DA_SERVICE_SETT     << COMMAND_READ_PLUGIN_LOG_WARN
            << COMMAND_READ_PLUGIN_LOG_ERROR        << COMMAND_READ_PEREDAVATOR_AC_SETT     << COMMAND_READ_MATILDA_AC_SETT         << COMMAND_READ_BACKUP_LIST         << COMMAND_READ_UDP_BEACON
            << COMMAND_READ_METER_LIST_HASH_SUMM_EXT<< COMMAND_READ_METER_LIST_FRAMED_EXT   << COMMAND_READ_ZIGBEE_SETT             << COMMAND_READ_TCP_SETT            << COMMAND_READ_FRWRD_SETT
            << COMMAND_READ_IPTABLE                 << COMMAND_READ_PPP_SUPERVISOR          << COMMAND_READ_DEVICE_SERIAL_NUMBER    << COMMAND_READ_LEDLAMPLIST_FRAMED  << COMMAND_READ_GROUP_SCHEDULE
            << COMMAND_READ_FF_TASK_TABLE_FRAMED    << COMMAND_READ_POWER_RELAY_SETT        << COMMAND_READ_FIREFLY_MODE_SETT       << COMMAND_READ_FIREFLY_LOG_EVENT   << COMMAND_READ_FIREFLY_LOG_ERROR
            << COMMAND_READ_FIREFLY_LOG_WARN        << COMMAND_WRITE_FIRST_4_OPERATOR       << COMMAND_WRITE_POLL_SCHEDULE          << COMMAND_WRITE_METER_LIST         << COMMAND_WRITE_METER_LIST_FRAMED
            << COMMAND_WRITE_DATE_SETT              << COMMAND_WRITE_RESET_MODEM            << COMMAND_WRITE_POLL_SETT              << COMMAND_WRITE_METER_LIST_ONE_PART<< COMMAND_WRITE_METER_LIST_POLL_ON
            << COMMAND_WRITE_METER_LIST_POLL_OFF    << COMMAND_WRITE_METER_LIST_DEL_NI      << COMMAND_WRITE_COMMANDS               << COMMAND_WRITE_DA_SERVICE_SETT    << COMMAND_WRITE_PEREDAVATOR_AC_SETT
            << COMMAND_WRITE_DA_OPEN_CLOSE          << COMMAND_WRITE_DA_DATA_2_COORDINATOR  << COMMAND_WRITE_FRWRD_SETT             << COMMAND_WRITE_LEDLAMPLIST_FRAMED << COMMAND_WRITE_GROUP_SCHEDULE
            << COMMAND_WRITE_POWER_RELAY_SETT       << COMMAND_WRITE_FIREFLY_MODE_SETT      << COMMAND_WRITE_FIRST                  << COMMAND_WRITE_UPGRADE            << COMMAND_WRITE_GPRS_SETT
            << COMMAND_WRITE_REBOOT                 << COMMAND_WRITE_DAEMON_RESTART         << COMMAND_WRITE_FULL_UPGRADE           << COMMAND_WRITE_ROOT_LOGIN_PSWD    << COMMAND_WRITE_GUEST_LOGIN_PSWD
            << COMMAND_WRITE_OPEARTOR_LOGIN_PSWD    << COMMAND_WRITE_DROP_TABLE_GET_COUNT   << COMMAND_WRITE_DROP_TABLE             << COMMAND_WRITE_DROP_TABLE_ALL     << COMMAND_WRITE_TIMEZONE
            << COMMAND_WRITE_NTP_SETTINGS           << COMMAND_WRITE_ABOUT_OBJECT           << COMMAND_WRITE_IP_FILTER_SETT         << COMMAND_WRITE_ERASE_ALL_DATA     << COMMAND_WRITE_MATILDA_AC_SETT
            << COMMAND_WRITE_DELETE_BACKUP_FILE     << COMMAND_WRITE_BACK_IN_TIME           << COMMAND_WRITE_CREATE_BACK_IN_TIME    << COMMAND_WRITE_UDP_BEACON         << COMMAND_WRITE_ZIGBEE_SETT
            << COMMAND_WRITE_TCP_SETT               << COMMAND_WRITE_COMMAND2BASH           << COMMAND_GET_BACKUP_FILE              << COMMAND_PUSH_BACKUP_FILE_AND_APPLY<< COMMAND_WRITE_IPTABLE
            << COMMAND_WRITE_PPP_SUPERVISOR  ;
}
//-----------------------------------------------------------------------------------------
QVariantHash MatildaProtocolHelper::aboutObjLine2hashObj(const QString &aboutObjLine)
{
    /*(if exists)
     * ID
     * SN <serial number>
     * vrsn
     * DEV
     * app
     * MAC<x>   0 < x < 10
     *
     * //gsm section
     * IMEI
     * IMSI
     * CID
     * BAND
     * RSSI
     * ATI
     * RCSP
     * Ec/No
     *
     * //zigbee
     * ZCH
     * ZID
     * ZRSSI
     * LQI
     * VR
     * HV
     * Type
     * EUI64
     *
*/

    QStringList info = aboutObjLine.split("; ", QString::SkipEmptyParts);

    QVariantHash h;
    QStringList lkm = QString("SN vrsn DEV app IMEI IMSI CID LAC RSSI RCSP ATI Ec/No ZCH ZID ZRSSI LQI VR HV Type EUI64 CellID ZEUI64 EcNo").split(" ", QString::SkipEmptyParts);


    for(int j = 0, jMax = info.size(); j < jMax; j++){
        for(int i = 0, iMax = lkm.size(); i < iMax; i++){
            if(info.at(j).startsWith(lkm.at(i) + "=")){
                h.insert(lkm.at(i), info.at(j).mid(lkm.at(i).length() + 1));
                iMax--;
                lkm.removeAt(i);
            }
        }
    }

    if(h.contains("CellID"))
        h.insert("CID", h.take("CellID"));
    if(h.contains("ZEUI64"))
        h.insert("EUI64", h.take("ZEUI64"));


    return h;
}
//-----------------------------------------------------------------------------------------
QVariantHash MatildaProtocolHelper::backupFileName2hashObj(QString fileName)
{
    fileName = fileName.replace("=", "/");
    QStringList lk = QString("ID SN vrsn DEV app IMEI IMSI CID LAC ZCH ZID EUI64 RSSI RCSP ATI EcNo ZRSSI LQI VR HV Type").split(" ");//Low priority

    QVariantHash h;
    h.insert("sha1", fileName.split("_").first());

    for(int i = h.value("sha1").toString().length(), c = 0, iMax = fileName.length(); i < iMax && c < 10; i++){
        QString mc = QString("MAC%1").arg(c);
        if(fileName.mid(i).startsWith(QString("_%1:").arg(mc))){
            h.insert(mc, fileName.mid(i + 2 + 4).split("_").first()); //_MAC0:80:30:DC:93:C1:F0_MAC1:80:30:DC:93:C1:FB_ID:matilda2_SN:A335
            c++;
        }else{
            if(fileName.mid(i).startsWith("_ID:"))
                break;
        }
    }

    for(int i = 0, iMax = lk.size(); i < iMax; i++){
        QString s = getValFromFileName(fileName, lk.at(i));
        if(!s.isEmpty())
            h.insert(lk.at(i), s);
    }
    return h;
}
//-----------------------------------------------------------------------------------------
QString MatildaProtocolHelper::getValFromFileName(const QString &fileName, const QString &key)
{
    QStringList l = fileName.split(QString("_%1:").arg(key));
    if(l.size() == 2)
        return l.last().split("_").first();
    return "-";
}
//-----------------------------------------------------------------------------------------
