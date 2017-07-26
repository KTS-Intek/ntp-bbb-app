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
#include "ntpresponser.h"

//----------------------------------------------------------------------
NTPResponseManager::NTPResponseManager(const bool &enVerboseMode, const QDateTime &dtRelease, QObject *parent) : QObject(parent)
{
    verboseMode = enVerboseMode;
    queueMaxSize = 1000;
    this->dtRelease = dtRelease;
    secsInQueue = 25;
    maximumThreads = 50;
    activeThreads = 0;
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
        client.remDtArr = remDtArr;
        client.dtReadUtc = dtReadUtc;
        hashQueueDt.insert(key, lastCurrDt);
        hashQueue.insert(key, client);
        queueList.append(key);

    }
    else if(verboseMode)
        qDebug() << "dtRemote is not valid " << dtRemote << sender << port ;




}
//----------------------------------------------------------------------
void NTPResponseManager::checkQueue()
{
    lastCurrDt = QDateTime::currentDateTimeUtc();
    bool addMsecs = (activeThreads >= maximumThreads);

    for( ; activeThreads < maximumThreads && !queueList.isEmpty(); ){
        if(hashQueueDt.value(queueList.first()).secsTo(lastCurrDt) > secsInQueue){
            if(verboseMode)
                qDebug() << "secs in queue " << hashQueueDt.value(queueList.first()).secsTo(lastCurrDt) << secsInQueue << queueList.first();
            queueList.removeFirst();
            continue;
        }
        RemNtpUdpClient client = hashQueue.take(queueList.takeFirst());

        NtpResponser *r = new NtpResponser(client.sender, client.port, client.remDtArr, client.dtReadUtc , verboseMode, dtRelease);
        activeThreads++;
        connect(r, SIGNAL(destroyed(QObject*)), this, SLOT(onResponserDestr()) );
        r->uCanStartThread();
    }

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
    }



    if(verboseMode){
        if(activeThreads >= maximumThreads){
            qDebug() << "server is overloaded activeThreads=" << activeThreads << maximumThreads << queueList.size();
        }

        if(queueList.isEmpty() && activeThreads != 0)
            qDebug() << "queue is emtpy!" << activeThreads << maximumThreads;

    }

    emit startTmrQueue(msec);
}
//----------------------------------------------------------------------
void NTPResponseManager::onThreadStarted()
{
    if(verboseMode)
        qDebug() << "NTPResponseManager ready" ;
    QTimer *timerCheckQueue = new QTimer(this);
    timerCheckQueue->setInterval(10);
    timerCheckQueue->setSingleShot(true);
    connect(timerCheckQueue, SIGNAL(timeout()), this, SLOT(checkQueue()) );
    connect(this, SIGNAL(startTmrQueue(int)), timerCheckQueue, SLOT(start(int)) );
    lastCurrDt = QDateTime::currentDateTimeUtc();
    emit startTmrQueue(111);
}

//----------------------------------------------------------------------
void NTPResponseManager::onResponserDestr()
{
    if(activeThreads > 0)
        activeThreads--;
    if(verboseMode && activeThreads == 0){
        qDebug() << "activeThreads = 0 " << queueList.size();
    }
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
