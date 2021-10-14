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

#include "ntpprotocolhelper.h"

//----------------------------------------------------------------------------

QByteArray NTPProtocolHelper::getArrDateTimeStamp(const QDateTime &dt)
{
    const QByteArray a = QByteArray::fromHex(QByteArray::number( QDateTime(QDate(1900, 1, 1), QTime(0,0,0,0), Qt::UTC).secsTo(dt), 16).rightJustified(8, '0') +

                                       QByteArray::number( (quint32)((quint32)0xFFFFFFFF/(quint32)1000) * (quint32)dt.time().msec() , 16).rightJustified(8, '0'));

    if(a.length() != 8){

//        qDebug() << "getArrDateTimeStamp err " << QByteArray::number( QDateTime(QDate(1900, 1, 1), QTime(0,0,0,0), Qt::UTC).secsTo(dt), 16).rightJustified(8, '0')
//                 << QDateTime(QDate(1900, 1, 1), QTime(0,0,0,0), Qt::UTC).secsTo(dt)
//                 <<  dt.time().msec() << ((quint32)0xFFFFFFFF/(quint32)1000) * dt.time().msec()
//                 << QByteArray::number( (((quint64)dt.time().msec() * (quint64)1000000 ) << 32)/(quint64)1000000000, 16).rightJustified(8, '0')
//                 << dt.time().msec();
//        a.clear();
        return QByteArray::fromHex(a.rightJustified(16,'0'));
    }
    return a;
}

QByteArray NTPProtocolHelper::getCurrentNtpTimeLeftPartFromDt(const QDateTime &dt, const QByteArray &dtRemoteArr)
{
    return getCurrentNtpTimeLeftPart(getArrDateTimeStamp(dt), dtRemoteArr);
}

//----------------------------------------------------------------------------

QByteArray NTPProtocolHelper::getCurrentNtpTimeLeftPart(const QByteArray &dtReadArr, const QByteArray &dtRemoteArr)
{
    /*
     * 0x24,    // No warning / Version 4 / Server (packed bitfield)
     * 3,       // Stratum 3 server
     * B,       // Polling interval
     * 0xE0,     // Precision in log2 seconds (18 is 1 microsecond)
     * 0,0,0,9, // Delay to reference clock (we have PPS, so effectively zero)
     * 0,0,0,9, // Jitter of reference clock (the PPS is rated to +/- 50ns)
     * LOCL // uncalibrated local clock     'GPS0, // Reference ID - we are using a GPS receiver
     *
     * dtRemoteArr - transmit timestamp із запиту клієнта
    */
    return QByteArray::fromHex("24 03 0C   03  00000019  00000019 ") + "LOCL" + dtReadArr + dtRemoteArr + dtReadArr;
}

//----------------------------------------------------------------------------

QDateTime NTPProtocolHelper::dtFromDataGram(const QByteArray &readArr, QByteArray &remDtArr)
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

//        if(verboseMode)
//            qDebug() << "dtFromDataGram " << remDtArr.toHex() << secs << frac << msec << ok1 << ok2 << dt.toString("yyyy-MM-dd hh:mm:ss.zzz");

    }
    return dt;
}

//----------------------------------------------------------------------------
