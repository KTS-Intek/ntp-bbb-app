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

#include "ntpservice.h"


///[!] type-converter
#include "src/base/prettyvalues.h"



NTPService::NTPService(const bool &verboseMode, QObject *parent) : NTPServiceBase(verboseMode, parent)
{
    makeRegistration();
}

void NTPService::makeRegistration()
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qRegisterMetaType<QList<QHostAddress> >("QList<QHostAddress>");
    qRegisterMetaType<QList<QDateTime> >("QList<QDateTime>");
    qRegisterMetaType<QList<quint16> >("QList<quint16>");
}

void NTPService::onThreadStarted()
{
    setDtRelease(PrettyValues::getBuildDate());

    connect(this, &NTPServiceBase::readyRead, this, &NTPServiceBase::mReadyRead);
    emit add2systemLogEvent(tr("Starting SNTPv4 server..."));
    if(startNTPService()){
        emit onNTPServiceIsReady();
        emit add2systemLogEvent(tr("SNTPv4 server is ready"));
    }else{
        emit add2systemLogEvent(tr("SNTPv4 server is not ready"));
        QThread::sleep(3);
        for(int i = 0; i < 10; i++){
            i = i / (i - i); //kill the application with just a zero
        }

    }

}

void NTPService::killAllObjects()
{
    close();

    deleteLater();;
}

bool NTPService::startNTPService()
{
    bool exitLater = false;

    for(int i = 0; i < 30000; i++){
        exitLater = false;


        if(!myParams.dtRelease.isValid() || myParams.dtRelease > QDateTime::currentDateTimeUtc()){
            emit add2systemLogError(tr("dtRelease > currDt: %1 %2")
                                    .arg(myParams.dtRelease.toUTC().toString("yyyy-MM-dd hh:mm"))
                                    .arg(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm")));
//            qDebug() << "dtRelease " <<  dtRelease.isValid() << QDateTime::currentDateTimeUtc();
            exitLater = true;

        }

        if(!exitLater && !bind(QHostAddress::Any, 123) ){
            qDebug() << "can't bind port 123 " << errorString() << myParams.dtRelease.isValid();
            emit add2systemLogError(tr("Can't bind, port: %1, error: %2")
                                    .arg(QString::number(123))
                                    .arg(errorString()));
            exitLater = true;
        }

        if(!exitLater)
            break;

        emit onFailedToStartNTPService();
        QThread::sleep(10);
    }
    return !exitLater;

}
