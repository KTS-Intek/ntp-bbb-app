/****************************************************************************
**
**   Copyright © 2016-2021 The KTS-INTEK Ltd.
**   Contact: http://www.kts-intek.com
**   bohdan@kts-intek.com
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

#include "ntpdataprocessorbase.h"


///[!] ntp-bbb
#include "ntp-bbb-src/ntpprotocolhelper.h"



NTPDataProcessorBase::NTPDataProcessorBase(const bool &verboseMode, QObject *parent) : QObject(parent)
{
    myParams.verboseMode = verboseMode;

}

QString NTPDataProcessorBase::remIpPort2key(QHostAddress ipAddr, quint16 port)
{
    return QString("%1-%2").arg(ipAddr.toString()).arg(QString::number(port));

}

bool NTPDataProcessorBase::addThisClient2queueSmart(const QHostAddress &addr, const quint16 &remPort, const QByteArray &datagramArr, const QDateTime &dtReadUtc, QDateTime &dtRemote)
{
    const QString key = remIpPort2key(addr, remPort);

    removeThisKeyFromTheQueue(key);


    const quint64 size = myParams.queueList.size();
    if(size >= myParams.queueMaxSize){
        if(myParams.verboseMode)
            qDebug() << "addThisHost2queue queue overloaded " << size << myParams.queueMaxSize;
        emit add2systemLogError(tr("Queue is too big, size is %1, the limit is %2").arg(size).arg(myParams.queueMaxSize));
        return false;
    }

    const quint64 lastSec = QDateTime::currentSecsSinceEpoch();

    QByteArray remDtArr;
    dtRemote = NTPProtocolHelper::dtFromDataGram(datagramArr, remDtArr);

    if(dtRemote.isValid()){

//        RemNtpUdpClient client;
        NTPQueueItem client;
        client.sender = addr;
        client.port = remPort;
        client.leftArr =  NTPProtocolHelper::getCurrentNtpTimeLeftPartFromDt(dtReadUtc, remDtArr);

        client.secWhen = lastSec;

        addThisClientKeyToTheQueue(key, client);


        return true;
    }
    if(myParams.verboseMode)
        qDebug() << "dtRemote is not valid " << dtRemote << addr << remPort ;
    return false;
}

void NTPDataProcessorBase::removeThisKeyFromTheQueue(const QString &key)
{
    if(myQueue.hashQueueTable.contains(key)){
        myQueue.hashQueueTable.remove(key);
        myQueue.queue.removeOne(key);
    }
}

void NTPDataProcessorBase::addThisClientKeyToTheQueue(const QString &key, const NTPDataProcessorBase::NTPQueueItem &client)
{
    myQueue.queue.append(key);
    myQueue.hashQueueTable.insert(key, client);
}

bool NTPDataProcessorBase::checkSendQueue()
{
    const qint64 currSec = QDateTime::currentSecsSinceEpoch();//it is seconds

    QList<QHostAddress> lclntip;
    QList<quint16> lclntport;
    QList<QByteArray> lclntleftarr;
    int counter = 0;
    for(quint64 i = 0 ; i < myParams.maximumThreads && !myQueue.queue.isEmpty(); i++){
        const QString key = myQueue.queue.takeFirst();
        const auto oneItem = myQueue.hashQueueTable.take(key);

        const quint64 secsInQueue = qAbs(oneItem.secWhen - currSec) ;

        if(secsInQueue > myParams.secsInQueue){
            if(myParams.verboseMode)
                qDebug() << "secs in queue " << secsInQueue << myParams.secsInQueue << key;
            continue;
        }

        lclntip.append(oneItem.sender);
        lclntport.append(oneItem.port);
        lclntleftarr.append(oneItem.leftArr);
        counter++;
    }

    if(counter > 0)
        emit sendDt2clnt(lclntip, lclntport, lclntleftarr, counter);

    return myQueue.queue.isEmpty();

}



void NTPDataProcessorBase::setQueueParams(quint64 queueMaxSize, quint64 secsInQueue, quint64 maximumThreads)
{

    if(queueMaxSize != myParams.queueMaxSize || secsInQueue != myParams.secsInQueue || maximumThreads != myParams.maximumThreads){
        myParams.queueMaxSize = queueMaxSize;
        myParams.secsInQueue = secsInQueue;
        myParams.maximumThreads = maximumThreads;

        if(myParams.verboseMode)
            qDebug() << "changed config " << queueMaxSize << secsInQueue << maximumThreads;
        emit add2systemLogEvent(tr("Changed configuration, queue: %1, secs: %2, thread: %3").arg(queueMaxSize).arg(secsInQueue).arg(maximumThreads));
    }


}


