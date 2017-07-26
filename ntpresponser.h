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

#ifndef NTPRESPONSER_H
#define NTPRESPONSER_H

#include <QObject>
#include <QUdpSocket>
#include <QDateTime>

class NtpResponser : public QUdpSocket
{
    Q_OBJECT
public:
    explicit NtpResponser(const QHostAddress &remSender, const quint16 &remPort, const QByteArray &remDtArr, const QDateTime &dtReadUtc, const bool &enVerboseMode, const QDateTime &dtRelease, QObject *parent = 0);


signals:
    void startThread();

public slots:
    void onTheadStarted();

    void uCanStartThread();

private:

    QByteArray getCurrentNtpTime(const QByteArray &dtReadArr, const QByteArray &dtRemoteArr);

    QByteArray getArrDateTimeStamp(const QDateTime &dt);


    QHostAddress remSender;
    quint16 remPort;
    QByteArray remDtArr;
    QDateTime dtReadUtc;

    QDateTime dtRelease;
    bool verboseMode;
};

#endif // NTPRESPONSER_H
