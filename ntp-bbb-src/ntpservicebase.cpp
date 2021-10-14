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

#include "ntpservicebase.h"


///[!] ntp-bbb
#include "ntp-bbb-src/ntpprotocolhelper.h"


///[!] type-converter
#include "src/shared/networkconverthelper.h"


#include "matildalimits.h"

//-------------------------------------------------------------------------------------------------------------------------------

NTPServiceBase::NTPServiceBase(const bool &verboseMode, QObject *parent) : QUdpSocket(parent)
{
    myParams.verboseMode = verboseMode;
}

//-------------------------------------------------------------------------------------------------------------------------------

void NTPServiceBase::sendDt2clnt(QList<QHostAddress> remSender, QList<quint16> remPort, QList<QByteArray> leftArr, quint32 counter)
{
    QDateTime currDt = QDateTime::currentDateTimeUtc();
    if(currDt > myParams.dtRelease){
        QByteArray currDtarr = NTPProtocolHelper::getArrDateTimeStamp(currDt);

        for(quint32 i = 0, updateDt = 0; i < counter; i++, updateDt++){
            const QByteArray writeArr = leftArr.at(i) + currDtarr;
            const qint64 writeSize = writeDatagram(writeArr, remSender.at(i), remPort.at(i));

            if(myParams.verboseMode){
                if(writeSize < 1)
                    qDebug() << "write NTP " << remSender.at(i).toString() << remPort.at(i) << writeArr.toHex() << writeSize <<  currDt.toString("yyyy-MM-dd hh:mm:ss.zzz") << errorString();
                else
                    qDebug() << "writeNTP " << writeSize << writeArr.toHex() << remSender.at(i).toString() << remPort.at(i) << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz")<< errorString();
            }
            if(updateDt > 50){
                updateDt = 0;
                currDt = QDateTime::currentDateTimeUtc();
                if(currDt < myParams.dtRelease)
                    break;
                currDtarr = NTPProtocolHelper::getArrDateTimeStamp(currDt);
            }

        }
    }
//    emit onClntDone(counter); I don't use it anymore
}

//-------------------------------------------------------------------------------------------------------------------------------

void NTPServiceBase::setDtRelease(const QDateTime &dt)
{
    myParams.dtRelease = dt;
}

//-------------------------------------------------------------------------------------------------------------------------------

void NTPServiceBase::mReadyRead()
{
    disconnect(this, &NTPServiceBase::readyRead, this, &NTPServiceBase::mReadyRead);

    const bool r = mReadyReadF();
    connect(this, &NTPServiceBase::readyRead, this, &NTPServiceBase::mReadyRead);

    if(r)
        QTimer::singleShot(1, this, SLOT(mReadyRead()));
}

//-------------------------------------------------------------------------------------------------------------------------------

bool NTPServiceBase::mReadyReadF()
{
    const QDateTime currDt = QDateTime::currentDateTimeUtc();
    const bool blockEmit = (!myParams.dtRelease.isValid() || myParams.dtRelease > currDt);



    if(blockEmit){
        emit add2systemLogError(tr("dtRelease > currDt: %1 %2").arg(myParams.dtRelease.toUTC().toString("yyyy-MM-dd hh:mm")).arg(currDt.toUTC().toString("yyyy-MM-dd hh:mm")));
        if(myParams.verboseMode)
            qDebug() << "dtRelease > currDt" << myParams.dtRelease.toString("yyyy-MM-dd hh:mm:ss.zzz") << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz") << myParams.dtRelease.isValid();
    }


    //QHostAddress sender, quint16 port, QByteArray datagram, QDateTime dtUtc
    QList<QHostAddress> lsender;
    QList<quint16> lport;
    QList<QByteArray> ldatagram;

    int counter = 0;

    QTime timeGlobal;
    timeGlobal.start();

    for(int i = 0; i < 100 && timeGlobal.elapsed() < 500 && hasPendingDatagrams() && counter < 10; i++) {
        const auto datalen = pendingDatagramSize();

        if(datalen < 42 ){
            waitForReadyRead(10);
            continue; //must be more
        }

        if(datalen > MAX_DATALEN)
            break; //too much data

        QByteArray datagram;
        datagram.resize(datalen);


        QHostAddress sender;
        quint16 senderPort;
        readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        lsender.append(sender);
        lport.append(senderPort);
        ldatagram.append(datagram);
        counter++;

        if(myParams.verboseMode){
            qDebug() << "";
            qDebug() << "-------------------------------";
            qDebug() << "has datagram " << currDt.toString("yyyy-MM-dd hh:mm:ss.zzz") << datagram.toHex() << sender.toString() << senderPort << blockEmit;
        }
    }


    if(!blockEmit && !lsender.isEmpty())
        emit addThisHost2queue(lsender, lport, ldatagram, currDt, counter);

    const bool hadBadDataGrams = checkHasBadDataGrams();
    if(hadBadDataGrams)
        return false;//everything was vanished

    return hasPendingDatagrams();
}

//-------------------------------------------------------------------------------------------------------------------------------

bool NTPServiceBase::checkHasBadDataGrams()
{
    //return after vanishing of bad data
    //true - there is no more data
    //false - there is some data


    //it must be fast

    for(int i = 0; i < 10 && hasPendingDatagrams(); i++){


        const auto datalen = pendingDatagramSize();

        QByteArray datagram;
        QHostAddress sender;
        quint16 senderPort;

        if(datalen > MAX_DATALEN){
            readDatagram(datagram.data(), 0, &sender, &senderPort);

            const QString ipstr = NetworkConvertHelper::showNormalIP(sender.toString());
            emit blockThisIP(ipstr);

            emit add2systemLogError(tr("Bad datagram, size is %1, last IP: %2")
                                    .arg(datalen)
                                    .arg(ipstr));

            if(myParams.verboseMode){
                qDebug() << "";
                qDebug() << "-------------------------------";
                qDebug() << "has bad datagram " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << datalen<< ipstr << senderPort ;
            }

            continue; //too much data, it must receive the whole datagram, so the buffer must be clear
        }

        if(datalen >= 42){ //it has some good data
            return false;// receive it
        }

//        if(datalen < 42 ){



        readDatagram(datagram.data(), 0, &sender, &senderPort);

        const QString ipstr = NetworkConvertHelper::showNormalIP(sender.toString());
        emit blockThisIP(ipstr);


        emit add2systemLogError(tr("Bad datagram, size is %1, last IP: %2")
                                .arg(datalen)
                                .arg(ipstr));

        if(myParams.verboseMode){
            qDebug() << "";
            qDebug() << "-------------------------------";
            qDebug() << "has bad datagram " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << datalen<< ipstr << senderPort ;
        }
    }

    if(!hasPendingDatagrams())
        return false; //there is no more data

    return true;

}

//-------------------------------------------------------------------------------------------------------------------------------
