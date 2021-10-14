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

#ifndef NTPSERVICEBASE_H
#define NTPSERVICEBASE_H

#include <QUdpSocket>
#include <QDateTime>
#include <QStringList>
#include <QTimer>
#include <QByteArray>
#include <QHostAddress>

class NTPServiceBase : public QUdpSocket
{
    Q_OBJECT
public:
    explicit NTPServiceBase(const bool &verboseMode, QObject *parent = nullptr);

    struct ServiceParams
    {
        QDateTime dtRelease;
        bool verboseMode;
        ServiceParams() : verboseMode(false) {}
    } myParams;


signals:
    //to ntp data processor
    void addThisHost2queue(QList<QHostAddress> lsender, QList<quint16> lport, QList<QByteArray> ldatagram, QDateTime dtUtc, int counter);
//    void onClntDone(quint32 counter);
    void blockThisIP(QString ip);

    //to the parent
    void add2systemLogError(QString err);
    void add2systemLogWarn(QString warn);
    void add2systemLogEvent(QString evnt);



public slots:
    void sendDt2clnt(QList<QHostAddress> remSender, QList<quint16> remPort, QList<QByteArray> leftArr, quint32 counter);

    void setDtRelease(const QDateTime &dt);

    void mReadyRead();


private:
    bool mReadyReadF();

    bool checkHasBadDataGrams();

};

#endif // NTPSERVICEBASE_H
