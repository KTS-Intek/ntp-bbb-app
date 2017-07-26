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

#ifndef UDPSERVICE_H
#define UDPSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QtCore>
#include <QHostAddress>

Q_DECLARE_METATYPE(QHostAddress)

class UdpService : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpService(const bool &enVerboseMode, quint16 bindPort, QObject *parent = 0);

signals:
    void addThisHost2queue(QHostAddress sender, quint16 port, QByteArray datagram, QDateTime dtUtc);


public slots:
    void onThreadStarted();


private slots:
    void mReadyRead();


private:
    void mReadyReadF();



    quint16 bport;

    bool verboseMode;

    QDateTime dtRelease;


};

#endif // UDPSERVICE_H
