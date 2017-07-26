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

#include "ntpresponser.h"
#include <QtCore>
//------------------------------------------------------------------------------------
NtpResponser::NtpResponser(const QHostAddress &remSender, const quint16 &remPort, const QByteArray &remDtArr, const QDateTime &dtReadUtc, const bool &enVerboseMode, const QDateTime &dtRelease, QObject *parent) : QUdpSocket(parent)
{
    verboseMode = enVerboseMode;
    this->remSender = remSender;
    this->remPort = remPort;
    this->remDtArr = remDtArr;
    this->dtReadUtc = dtReadUtc;
    this->dtRelease = dtRelease;

    QThread *t = new QThread;
    this->moveToThread(t);
    connect(t, SIGNAL(started()), this, SLOT(onTheadStarted()) );
    connect(this, SIGNAL(destroyed(QObject*)), t, SLOT(quit()) );
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()) );
    connect(this, SIGNAL(startThread()), t, SLOT(start()) );
}
//------------------------------------------------------------------------------------
void NtpResponser::onTheadStarted()
{
    QByteArray writeArr = getCurrentNtpTime( getArrDateTimeStamp(dtReadUtc), remDtArr);
    qint64 writeSize = 0;
    if(!writeArr.isEmpty()){
        writeSize = writeDatagram(writeArr, remSender, remPort);
    }

    if(writeSize < 1 && verboseMode)
            qDebug() << "write NTP " << remSender.toString() << remPort << writeArr.toHex() << writeSize << dtRelease << QDateTime::currentDateTime();

    QTimer::singleShot(111, this, SLOT(deleteLater()) );

}
//------------------------------------------------------------------------------------
void NtpResponser::uCanStartThread()
{
    emit startThread();
}
//------------------------------------------------------------------------------------
QByteArray NtpResponser::getCurrentNtpTime(const QByteArray &dtReadArr, const QByteArray &dtRemoteArr)
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

    QDateTime currdt = QDateTime::currentDateTimeUtc();
    if(dtRelease < currdt)
        return QByteArray::fromHex("24 03 03   E9  00000001  00000001 ") + "LOCL" + dtReadArr + dtRemoteArr + dtReadArr + getArrDateTimeStamp(currdt);

    return "";
}
//------------------------------------------------------------------------------------
QByteArray NtpResponser::getArrDateTimeStamp(const QDateTime &dt)
{
    QByteArray a = QByteArray::fromHex(QByteArray::number( QDateTime(QDate(1900, 1, 1), QTime(0,0,0,0), Qt::UTC).secsTo(dt), 16).rightJustified(8, '0') +
                                       QByteArray::number( (quint32)((quint32)0xFFFFFFFF/(quint32)1000) * (quint32)dt.time().msec() , 16).rightJustified(8, '0'));
//                               QByteArray::number( (((quint64)dt.time().msec() * (quint64)1000000 ) << 32)/(quint64)1000000000, 16).rightJustified(8, '0'));

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
//------------------------------------------------------------------------------------
