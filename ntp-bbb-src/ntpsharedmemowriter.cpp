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

#include "ntpsharedmemowriter.h"

///[!] type-converter
#include "src/shared/networkconverthelper.h"


//all logs were QSL, so you have to add necessary methods to matilba-bbb


NTPSharedMemoWriter::NTPSharedMemoWriter(const QString &sharedMemoName, const QString &semaName, const QString &write2fileName,
                                         const int &delay, const int &delay2fileMsec, const bool &verboseMode, QObject *parent) :
    SharedMemoWriter(sharedMemoName, semaName, write2fileName, delay, delay2fileMsec, verboseMode, parent)
{

}

QStringList NTPSharedMemoWriter::getLines(const QString &s)
{
    return QString("%1 %2")
            .arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz"))
            .arg(s).split("\n");
}

void NTPSharedMemoWriter::add2systemLogError(QString err)
{
    add2systemLogErrorList(getLines(err));
}

void NTPSharedMemoWriter::add2systemLogWarn(QString warn)
{
    add2systemLogWarnList(getLines(warn));

}

void NTPSharedMemoWriter::add2systemLogEvent(QString evnt)
{
    add2systemLogEventList(getLines(evnt));

}

void NTPSharedMemoWriter::add2systemLogErrorList(QStringList list)
{
    appendLogDataSmart("ntp-err", list);
}

void NTPSharedMemoWriter::add2systemLogWarnList(QStringList list)
{
    appendLogDataSmart("ntp-warn", list);

}

void NTPSharedMemoWriter::add2systemLogEventList(QStringList list)
{
    appendLogDataSmart("ntp-ev", list);

}

void NTPSharedMemoWriter::add2ipHistory(QList<QHostAddress> lhost, QDateTime dtReadUtc, QList<QDateTime> lDtRemoteUtc, int counter)
{
    QStringList l;
    QString dts = dtReadUtc.toString("yyyyMMddhhmmsszzz");
    for(int i = 0; i < counter; i++)
        l.append(QString("%1\t%2\t%3")
                 .arg(NetworkConvertHelper::showNormalIP(lhost.at(i)))
                 .arg(dts)
                 .arg(lDtRemoteUtc.at(i).toString("yyyyMMddhhmmsszzz")));

//    hashStatus.insert("ntp-lastip", MatildaProtocolHelper::addLines2list(l, hashStatus.value("ntp-lastip").toStringList(), 5000, ""));

     appendLogData("ntp-lastip", l, "\n", 5000);
     if(verboseMode)
         qDebug() << "ntp-lastip" << l;
}

void NTPSharedMemoWriter::appendLogDataSmart(const QString &key, const QStringList &log)
{
    appendLogData(key, log, "\n", 500);
    if(verboseMode)
        qDebug() << key << log;

}
