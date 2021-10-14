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

#include "ntpdataprocessor.h"

NTPDataProcessor::NTPDataProcessor(const bool &verboseMode, QObject *parent) :
    NTPDataProcessorBase(verboseMode, parent)
{

}

void NTPDataProcessor::onThreadStarted()
{
    QTimer *timerCheckQueue = new QTimer(this);
    timerCheckQueue->setInterval(10);
    timerCheckQueue->setSingleShot(true);
    connect(timerCheckQueue, &QTimer::timeout, this, &NTPDataProcessor::checkQueue);
    connect(this, SIGNAL(startTmrQueue(int)), timerCheckQueue, SLOT(start(int)) );

    emit add2systemLogEvent(tr("SNTP data processor is ready"));
    emit startTmrQueue(111);

}


void NTPDataProcessor::addThisHost2queue(QList<QHostAddress> lsender, QList<quint16> lport, QList<QByteArray> ldatagram, QDateTime dtReadUtc, int counter)
{
    QList<QHostAddress> lsender2;
    QList<QDateTime> lDtRemote;
    int counter2 = 0;

    for(int i = 0; i < counter; i++){

        const QString strIP = NetworkConvertHelper::showNormalIP(lsender.at(i));


        if(isIPblockedByTheAllowList(strIP) || isIPblockedByTheBlockList(strIP) || isIPblockedByTheTemporaryBlockList(strIP)){
            if(myParams.verboseMode)
                qDebug() << "addThisHost2queue ignore this ip " << strIP;
            continue;
        }

        QDateTime dtRemote;
        if(addThisClient2queueSmart(lsender.at(i), lport.at(i), ldatagram.at(i), dtReadUtc, dtRemote)){
            lsender2.append(lsender.at(i));
            lDtRemote.append(dtRemote);
            counter2++;
        }


    }

    if(counter2 > 0){
        emit add2ipHistory(lsender2, dtReadUtc, lDtRemote, counter2);
        emit startTmrQueue(1);//start processing ASAP
    }

}

void NTPDataProcessor::checkQueue()
{
    const int msec = checkSendQueue() ? 1111 : 1;
    emit startTmrQueue(msec);
}



void NTPDataProcessor::killAllObjects()
{

    deleteLater();
}


