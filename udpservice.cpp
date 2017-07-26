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

#include "udpservice.h"

#include <QHostAddress>
#include "ntpresponsemanager.h"

//-------------------------------------------------------------------------------------------
UdpService::UdpService(const bool &enVerboseMode, quint16 bindPort, QObject *parent) : QUdpSocket(parent)
{
    bport = bindPort;
    verboseMode = enVerboseMode;
}
//-------------------------------------------------------------------------------------------
void UdpService::onThreadStarted()
{
    dtRelease = QDateTime::fromString( QString::fromLocal8Bit(BUILDDATE) + " " + QString::fromLocal8Bit(BUILDTIME), "yyyy-MM-dd hh:mm:ss") ;

    if(!this->bind(QHostAddress::Any, bport) || !dtRelease.isValid()){
        qDebug() << "can't bind port " << bport << errorString() << dtRelease.isValid();
        QThread::sleep(5);
        qApp->exit(3);
        return;
    }

    qRegisterMetaType<QHostAddress>();

    NTPResponseManager *manager = new NTPResponseManager(verboseMode, dtRelease);
    QThread *t = new QThread(this);
    manager->moveToThread(t);
    connect(t, SIGNAL(started()), manager, SLOT(onThreadStarted()) );
    connect(manager, SIGNAL(destroyed(QObject*)), t, SLOT(quit()) );
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()) );
    connect(this, SIGNAL(addThisHost2queue(QHostAddress,quint16,QByteArray,QDateTime)), manager, SLOT(addThisHost2queue(QHostAddress,quint16,QByteArray,QDateTime)) );

    t->start();

    connect(this, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );

}
//-------------------------------------------------------------------------------------------
void UdpService::mReadyRead()
{
    disconnect(this, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );
    mReadyReadF();
    connect(this, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );
}
//-------------------------------------------------------------------------------------------
void UdpService::mReadyReadF()
{
    int badPackets = 0;
    int packetReads = 0;
    QTime totalTime;
    totalTime.start();
    bool blockEmit = false;

    QDateTime currDt = QDateTime::currentDateTimeUtc();

    if(!dtRelease.isValid() || dtRelease > currDt){
       qDebug() << "dtRelease > currDt" << dtRelease.toString("yyyy-MM-dd hh:mm:ss.zzz") << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz") << dtRelease.isValid();
       blockEmit = true;
    }
    quint8 updateCurrDt = 0;
    while (hasPendingDatagrams()) {
        packetReads++;
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());

        if(datagram.size() >= 42 ){
            badPackets = 0;
            QHostAddress sender;
            quint16 senderPort;
            readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
            if(verboseMode){
                qDebug() << endl;
                qDebug() << "-------------------------------";
                qDebug() << "has datagram " << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz") << datagram.toHex() << sender.toString() << senderPort << blockEmit;
            }

            if(updateCurrDt > 9){
                updateCurrDt = 0;
                currDt = QDateTime::currentDateTimeUtc();
                if(!dtRelease.isValid() || dtRelease > currDt){
                   qDebug() << "dtRelease > currDt" << dtRelease.toString("yyyy-MM-dd hh:mm:ss.zzz") << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz") << dtRelease.isValid();
                   blockEmit = true;
                }
            }else{
                updateCurrDt++;
            }

            if(!blockEmit)
                emit addThisHost2queue(sender, senderPort, datagram, currDt);
        }else{
            badPackets++;
            if(badPackets > 10){
                readDatagram(datagram.data(), datagram.size());
                if(verboseMode)
                    qDebug() << "has bad datagram " << badPackets << QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz ") << datagram.toHex();

            }
        }
    }
    if(verboseMode)
        qDebug() << "mReadyReadF " << packetReads << totalTime.elapsed() << totalTime.elapsed()/1000;
}

//-------------------------------------------------------------------------------------------
