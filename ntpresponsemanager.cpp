/****************************************************************************
**
**   Copyright © 2016-2017 The KTS-INTEK Ltd.
**   Contact: http://www.kts-intek.com.ua
**   bohdan@kts-intek.com.ua
**
**  This file is part of ntp-bbb.
**
**  ntp-bbb is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  ntp-bbb is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with ntp-bbb.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "ntpresponsemanager.h"
//#include "ntpresponser.h"
#include "settloader4matilda.h"
#include "sharedmemorymanager.h"

//----------------------------------------------------------------------
NTPResponseManager::NTPResponseManager(const bool &enVerboseMode, const QDateTime &dtRelease, const bool &enShMem, QObject *parent) : QObject(parent)
{
    verboseMode = enVerboseMode;
    this->dtRelease = dtRelease;
    activeThreads = 0;
    allowSharedMemory = enShMem;
}
//----------------------------------------------------------------------
QByteArray NTPResponseManager::getArrDateTimeStamp(const QDateTime &dt)
{
    QByteArray a = QByteArray::fromHex(QByteArray::number( QDateTime(QDate(1900, 1, 1), QTime(0,0,0,0), Qt::UTC).secsTo(dt), 16).rightJustified(8, '0') +
                                       QByteArray::number( (quint32)((quint32)0xFFFFFFFF/(quint32)1000) * (quint32)dt.time().msec() , 16).rightJustified(8, '0'));

    if(a.length() != 8){

        qDebug() << "getArrDateTimeStamp err " << QByteArray::number( QDateTime(QDate(1900, 1, 1), QTime(0,0,0,0), Qt::UTC).secsTo(dt), 16).rightJustified(8, '0')
                 << QDateTime(QDate(1900, 1, 1), QTime(0,0,0,0), Qt::UTC).secsTo(dt)
                 <<  dt.time().msec() << ((quint32)0xFFFFFFFF/(quint32)1000) * dt.time().msec()
                 << QByteArray::number( (((quint64)dt.time().msec() * (quint64)1000000 ) << 32)/(quint64)1000000000, 16).rightJustified(8, '0')
                 << dt.time().msec();
        a.clear();
        a = QByteArray::fromHex(a.rightJustified(16,'0'));
    }
    return a;
}
//----------------------------------------------------------------------

QByteArray NTPResponseManager::getCurrentNtpTimeLeftPart(const QByteArray &dtReadArr, const QByteArray &dtRemoteArr)
{
    /*
     * 0x24,    // No warning / Version 4 / Server (packed bitfield)
     * 3,       // Stratum 3 server
     * 3,       // Polling interval
     * 0xE9,     // Precision in log2 seconds (18 is 1 microsecond)
     * 0,0,0,0, // Delay to reference clock (we have PPS, so effectively zero)
     * 0,0,0,1, // Jitter of reference clock (the PPS is rated to +/- 50ns)
     * LOCL // uncalibrated local clock     'GPS0, // Reference ID - we are using a GPS receiver
    */
    return QByteArray::fromHex("24 03 03   E9  00000000  00000001 ") + "LOCL" + dtReadArr + dtRemoteArr + dtReadArr;
}


//----------------------------------------------------------------------
void NTPResponseManager::addThisHost2queue(QHostAddress sender, quint16 port, QByteArray datagram, QDateTime dtReadUtc)
{
    QString key = remIpPort2key(sender, port);
    if(hashQueue.contains(key)){
        queueList.removeOne(key);
        hashQueue.remove(key);
        hashQueueDt.remove(key);
    }

    quint64 size = queueList.size();
    if(size >= queueMaxSize){
        if(verboseMode)
            qDebug() << "addThisHost2queue queue overloaded " << size << queueMaxSize;
        return;
    }

    QByteArray remDtArr;
    QDateTime dtRemote = dtFromDataGram(datagram, remDtArr);
    if(dtRemote.isValid()){

        RemNtpUdpClient client;
        client.sender = sender;
        client.port = port;
        client.leftArr = getCurrentNtpTimeLeftPart(getArrDateTimeStamp(dtReadUtc), remDtArr);

        hashQueueDt.insert(key, lastCurrDt);
        hashQueue.insert(key, client);
        queueList.append(key);

        emit add2ipHistory(sender, dtReadUtc, dtRemote);

    }
    else if(verboseMode)
        qDebug() << "dtRemote is not valid " << dtRemote << sender << port ;
}
//----------------------------------------------------------------------
void NTPResponseManager::checkQueue()
{
    lastCurrDt = QDateTime::currentDateTimeUtc();
    bool addMsecs = (activeThreads >= maximumThreads);

    QList<QHostAddress> lclntip;
    QList<quint16> lclntport;
    QList<QByteArray> lclntleftarr;

    quint32 counter = 0;
    for( ; activeThreads < maximumThreads && !queueList.isEmpty(); ){
        if(hashQueueDt.value(queueList.first()).secsTo(lastCurrDt) > secsInQueue){
            if(verboseMode)
                qDebug() << "secs in queue " << hashQueueDt.value(queueList.first()).secsTo(lastCurrDt) << secsInQueue << queueList.first();
            queueList.removeFirst();
            continue;
        }
        RemNtpUdpClient client = hashQueue.take(queueList.takeFirst());
        activeThreads++;
        lclntip.append(client.sender);
        lclntport.append(client.port);
        lclntleftarr.append(client.leftArr);
        createdObjectCounter++;
        counter++;
    }
    if(counter > 0)
        emit sendDt2clnt(lclntip, lclntport, lclntleftarr, counter);


    int msec = 11;
    if(activeThreads >= maximumThreads){
        msec = sayServerIsBusy ? 111 : 55;
        if(addMsecs)
            msec *= 2;
        if(!sayServerIsBusy){
            sayServerIsBusy = true;
            if(verboseMode)
                qDebug() << "server is overloaded activeThreads=" << activeThreads << maximumThreads << queueList.size();
        }
    }else{
        sayServerIsBusy = false;
        if(queueList.isEmpty())
            queueIsEmptyCounter++;
    }



    if(verboseMode){
        if(activeThreads >= maximumThreads)
            qDebug() << "server is overloaded activeThreads=" << activeThreads << maximumThreads << queueList.size();
    }

    emit onCountersChanged(killedObjectCounter, createdObjectCounter, queueIsEmptyCounter);
    emit startTmrQueue(msec);
}
//----------------------------------------------------------------------
void NTPResponseManager::onThreadStarted()
{
    if(verboseMode)
        qDebug() << "NTPResponseManager ready" ;

    queueMaxSize = secsInQueue = maximumThreads = 9;

    if(allowSharedMemory){
        SharedMemoryManager *manager = new SharedMemoryManager(verboseMode);
        QThread *t = new QThread(this);

        manager->moveToThread(t);
        connect(manager, SIGNAL(destroyed(QObject*)), t, SLOT(quit()) );
        connect(t, SIGNAL(finished()), t, SLOT(deleteLater()) );
        connect(t, SIGNAL(started()), manager, SLOT(onThreadStarted()) );
        connect(this, SIGNAL(destroyed(QObject*)), manager, SLOT(deleteLater()) );

        connect(this, SIGNAL(add2ipHistory(QHostAddress,QDateTime,QDateTime)), manager, SLOT(add2ipHistory(QHostAddress,QDateTime,QDateTime)) );
        connect(this, SIGNAL(add2systemLogError(QString)                    ), manager, SLOT(add2systemLogError(QString))                     );
        connect(this, SIGNAL(add2systemLogEvent(QString)                    ), manager, SLOT(add2systemLogEvent(QString))                     );
        connect(this, SIGNAL(add2systemLogWarn(QString)                     ), manager, SLOT(add2systemLogWarn(QString))                      );
        connect(this, SIGNAL(saveSharedMemory2file()                        ), manager, SLOT(saveSharedMemory2file())                         );
        t->start();
        if(verboseMode)
            qDebug() << "shared memmory is activated";
    }



    QTimer *timerCheckQueue = new QTimer(this);
    timerCheckQueue->setInterval(10);
    timerCheckQueue->setSingleShot(true);
    connect(timerCheckQueue, SIGNAL(timeout()), this, SLOT(checkQueue()) );
    connect(this, SIGNAL(startTmrQueue(int)), timerCheckQueue, SLOT(start(int)) );
    lastCurrDt = QDateTime::currentDateTimeUtc();

    QTimer *tmrCheckConf = new QTimer(this);
    tmrCheckConf->setSingleShot(true);
    tmrCheckConf->setInterval(30 * 60 * 1000);
    connect(tmrCheckConf, SIGNAL(timeout()), this, SLOT(reloadConfiguration()) );
    connect(this, SIGNAL(startTmrCheckConf()), tmrCheckConf, SLOT(start()) );


    emit add2systemLogEvent(tr("Start SNTPv4 server..."));
    reloadConfiguration();
    emit startTmrQueue(111);
}

//----------------------------------------------------------------------
void NTPResponseManager::onResponserDestr(quint32 counter)
{
    if(activeThreads >= counter)
        activeThreads -= counter;
    else
        activeThreads = 0;

    if(activeThreads > maximumThreads)//захист від збою
        activeThreads = 0;

    killedObjectCounter++;
    if(verboseMode && activeThreads == 0){
        qDebug() << "activeThreads = 0 " << queueList.size();
    }
}
//----------------------------------------------------------------------
void NTPResponseManager::reloadConfiguration()
{
    QVariantHash h = SettLoader4matilda().loadOneSett(SETT_SNTP_SERVICE).toHash();
//    h.insert("queue", 1000);
//    h.insert("secs", 30);
//    h.insert("thrd", 50);

    quint32 queueMaxSize = SettLoader4matilda::integerValidator(h.value("queue").toULongLong(), 10, 5000);
    quint32 secsInQueue = SettLoader4matilda::integerValidator(h.value("secs").toULongLong(), 10, 60);
    quint32 maximumThreads = SettLoader4matilda::integerValidator(h.value("thrd").toULongLong(), 10, 500);

   if(queueMaxSize != this->queueMaxSize || secsInQueue != this->secsInQueue || maximumThreads != this->maximumThreads){
       this->queueMaxSize = queueMaxSize;
       this->secsInQueue = secsInQueue;
       this->maximumThreads = maximumThreads;
       if(verboseMode)
           qDebug() << "changed config " << queueMaxSize << secsInQueue << maximumThreads;
       emit add2systemLogEvent(tr("Changed configuration, queue: %1, secs: %2, thread: %3").arg(queueMaxSize).arg(secsInQueue).arg(maximumThreads));
   }

    emit startTmrCheckConf();
}
//----------------------------------------------------------------------
QString NTPResponseManager::remIpPort2key(QHostAddress sender, quint16 port)
{
    return QString("%1-%2").arg(sender.toString()).arg(QString::number(port));
}
//----------------------------------------------------------------------
QDateTime NTPResponseManager::dtFromDataGram(const QByteArray &readArr, QByteArray &remDtArr)
{
    QDateTime dt;
    if(readArr.length() >= 48 && readArr.at(0) == 0x23){
//        quint32 secs =
        remDtArr = readArr.mid(40, 8);
        bool ok1, ok2;
        quint32 secs = remDtArr.left(4).toHex().toUInt(&ok1, 16);
        quint32 frac = remDtArr.right(4).toHex().toUInt(&ok2, 16);

        quint32 msec = frac / ((quint32)(0xFFFFFFFF)/(quint32)1000);

        if(ok1 && ok2 && secs > 0)
            dt = QDateTime(QDate(1900, 1, 1), QTime(0,0,0,0), Qt::UTC).addSecs(secs).addMSecs(msec);// ((frac * 1000000) >> 32)/1000);//microsecs  = (((unsigned long long) frac * 1000000) >> 32)

        if(verboseMode)
            qDebug() << "dtFromDataGram " << remDtArr.toHex() << secs << frac << msec << ok1 << ok2 << dt.toString("yyyy-MM-dd hh:mm:ss.zzz");

    }
    return dt;
}
//----------------------------------------------------------------------
