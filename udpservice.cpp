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
#include "settloader4matilda.h"

//-------------------------------------------------------------------------------------------
UdpService::UdpService(const bool &enVerboseMode, quint16 bindPort, QObject *parent) : QUdpSocket(parent)
{
    bport = bindPort;
    verboseMode = enVerboseMode;
}
//-------------------------------------------------------------------------------------------
QString UdpService::getVersionName()
{
    return "0.0.1";
}
//-------------------------------------------------------------------------------------------
QString UdpService::getBuildDate()
{
    qDebug() << "ZbyratorManager::getBuildDate " << BUILDDATE << BUILDTIME << QString::fromLocal8Bit(BUILDDATE) << QString::fromLocal8Bit(BUILDTIME);
    return QString::fromLocal8Bit(BUILDDATE) + " " + QString::fromLocal8Bit(BUILDTIME);
}
//-------------------------------------------------------------------------------------------
void UdpService::saveSharedMemory2file()
{
    add2systemLogEventF(tr("Closing SNTPv4 server..."));
    emit saveAll2file();
}
//-------------------------------------------------------------------------------------------
void UdpService::onThreadStarted()
{
    dtRelease = QDateTime::fromString( QString::fromLocal8Bit(BUILDDATE) + " " + QString::fromLocal8Bit(BUILDTIME), "yyyy-MM-dd hh:mm:ss").toUTC() ;


    qRegisterMetaType<QHostAddress>();
    qRegisterMetaType<QList<QHostAddress> >();
    qRegisterMetaType<QList<quint16> >("QList<quint16>");
//    qRegisterMetaType<QList<QByteArray> >("");


    NTPResponseManager *manager = new NTPResponseManager(verboseMode, dtRelease, qApp->arguments().contains("--shared-memory"));
    QThread *t = new QThread(this);
    manager->moveToThread(t);
    connect(t, SIGNAL(started() ), manager, SLOT(onThreadStarted()) );
    connect(t, SIGNAL(finished()), t      , SLOT(deleteLater())     );

    connect(manager, SIGNAL(destroyed(QObject*)), t, SLOT(quit()) );

    connect(manager, SIGNAL(sendDt2clnt(QList<QHostAddress>,QList<quint16>,QList<QByteArray>,quint32)), this, SLOT(sendDt2clnt(QList<QHostAddress>,QList<quint16>,QList<QByteArray>,quint32)) );
    connect(this, SIGNAL(onClntDone(quint32)), manager, SLOT(onResponserDestr(quint32)) );

    connect(this, SIGNAL(addThisHost2queue(QList<QHostAddress>,QList<quint16>,QList<QByteArray>,QDateTime,int)), manager, SLOT(addThisHost2queue(QList<QHostAddress>,QList<quint16>,QList<QByteArray>,QDateTime,int)) );
    connect(this, SIGNAL(add2systemLogError(QString)), manager, SIGNAL(add2systemLogError(QString)) );
    connect(this, SIGNAL(add2systemLogEvent(QString)), manager, SIGNAL(add2systemLogEvent(QString)) );
    connect(this, SIGNAL(add2systemLogWarn(QString) ), manager, SIGNAL(add2systemLogWarn(QString))  );
    connect(this, SIGNAL(saveAll2file()             ), manager, SIGNAL(saveSharedMemory2file())     );


    bool exitLater = false;
    if(!dtRelease.isValid() || dtRelease > QDateTime::currentDateTimeUtc()){
        add2systemLogErrorF(tr("dtRelease > currDt: %1 %2").arg(dtRelease.toUTC().toString("yyyy-MM-dd hh:mm")).arg(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm")));
        qDebug() << "dtRelease " <<  dtRelease.isValid() << QDateTime::currentDateTimeUtc();
        exitLater = true;
    }


    if(!this->bind(QHostAddress::Any, bport) ){
        qDebug() << "can't bind port " << bport << errorString() << dtRelease.isValid();
        add2systemLogErrorF(tr("Can't bind, port: %1, error: %2").arg(QString::number(bport)).arg(errorString()));
        exitLater = true;
    }

    if(exitLater){
        QThread::sleep(5);
        qApp->exit(3);
        return;
    }
    t->start();
    connect(this, SIGNAL(readyRead()), this, SLOT(mReadyRead()) );
}
//-------------------------------------------------------------------------------------------
void UdpService::sendDt2clnt(QList<QHostAddress> remSender, QList<quint16> remPort, QList<QByteArray> leftArr, quint32 counter)
{
    QDateTime currDt = QDateTime::currentDateTimeUtc();
    if(currDt > dtRelease){
        QByteArray currDtarr = NTPResponseManager::getArrDateTimeStamp(currDt);

        for(quint32 i = 0, updateDt = 0; i < counter; i++, updateDt++){
            QByteArray writeArr = leftArr.at(i) + currDtarr;
            qint64 writeSize = writeDatagram(writeArr, remSender.at(i), remPort.at(i));
            if(writeSize < 1 && verboseMode)
                qDebug() << "write NTP " << remSender.at(i).toString() << remPort.at(i) << writeArr.toHex() << writeSize << dtRelease << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz") << errorString();
            else if(verboseMode)
                qDebug() << "writeNTP " << writeSize << writeArr.toHex() << remSender.at(i).toString() << remPort.at(i) << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz")<< errorString();

            if(updateDt > 50){
                updateDt = 0;
                currDt = QDateTime::currentDateTimeUtc();
                if(currDt < dtRelease)
                    break;
                currDtarr = NTPResponseManager::getArrDateTimeStamp(currDt);
            }

        }
    }
    emit onClntDone(counter);
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
    QDateTime currDt = QDateTime::currentDateTimeUtc();
    bool blockEmit = (!dtRelease.isValid() || dtRelease > currDt);

    quint8 updateCurrDt = 10;

    //QHostAddress sender, quint16 port, QByteArray datagram, QDateTime dtUtc
    QList<QHostAddress> lsender;
    QList<quint16> lport;
    QList<QByteArray> ldatagram;

    int counter = 0;
    while (hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());
        updateCurrDt++;

        if(datagram.size() >= 42 ){
            badPackets = 0;
            QHostAddress sender;
            quint16 senderPort;
            readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

            lsender.append(sender);
            lport.append(senderPort);
            ldatagram.append(datagram);
            counter++;

            if(updateCurrDt > 9){
                updateCurrDt = 0;
                if(!blockEmit && !lsender.isEmpty())
                    emit addThisHost2queue(lsender, lport, ldatagram, currDt, counter);
                counter = 0;
                lsender.clear();
                lport.clear();
                ldatagram.clear();
                currDt = QDateTime::currentDateTimeUtc();
                if(!dtRelease.isValid() || dtRelease > currDt){
                    add2systemLogErrorF(tr("dtRelease > currDt: %1 %2").arg(dtRelease.toUTC().toString("yyyy-MM-dd hh:mm")).arg(currDt.toUTC().toString("yyyy-MM-dd hh:mm")));
                   qDebug() << "dtRelease > currDt" << dtRelease.toString("yyyy-MM-dd hh:mm:ss.zzz") << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz") << dtRelease.isValid();
                   blockEmit = true;
                }
            }

            if(verboseMode){
                qDebug() << endl;
                qDebug() << "-------------------------------";
                qDebug() << "has datagram " << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz") << datagram.toHex() << sender.toString() << senderPort << blockEmit;
            }



        }else{
            badPackets++;

            if(badPackets > 10){

                lsender.clear();
                lport.clear();
                ldatagram.clear();

                QHostAddress sender;
                quint16 senderPort;
                readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
                add2systemLogErrorF(tr("Bad datagram count > 10, last IP: %1").arg(SettLoader4matilda::showNormalIP(sender.toString())));

                for(int i = 0; i < badPackets && hasPendingDatagrams(); i++){
                    datagram.clear();
                    datagram.resize(pendingDatagramSize());
                    readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
                }
                badPackets = 0;

                if(verboseMode)
                    qDebug() << "has bad datagram " << badPackets << QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz ") << datagram.toHex();

            }
        }
    }

    if(!blockEmit && !lsender.isEmpty())
        emit addThisHost2queue(lsender, lport, ldatagram, currDt, counter);

}
//-------------------------------------------------------------------------------------------
void UdpService::add2systemLogErrorF(QString err)
{
    if(err.isEmpty() || (err == lastErr ))
        return;
    lastErr = err;
    emit add2systemLogError(err);
}
//-------------------------------------------------------------------------------------------
void UdpService::add2systemLogWarnF(QString warn)
{
    if(warn.isEmpty() || (warn == lastWarn ))
        return;
    lastWarn = warn;
    emit add2systemLogWarn(warn);
}
//-------------------------------------------------------------------------------------------
void UdpService::add2systemLogEventF(QString evnt)
{
    if(evnt.isEmpty() || (evnt == lastEv ))
        return;
    lastEv = evnt;
    emit add2systemLogEvent(evnt);
}
//-------------------------------------------------------------------------------------------

