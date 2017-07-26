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

#ifndef NTPRESPONSEMANAGER_H
#define NTPRESPONSEMANAGER_H

#include <QObject>
#include <QHostAddress>
#include <QtCore>


class NTPResponseManager : public QObject
{
    Q_OBJECT
public:
    explicit NTPResponseManager(const bool &enVerboseMode, const QDateTime &dtRelease, QObject *parent = 0);

signals:
    void startTmrQueue(int msec);

public slots:
    void addThisHost2queue(QHostAddress sender, quint16 port, QByteArray datagram, QDateTime dtReadUtc);

    void checkQueue();

    void onThreadStarted();

    void onResponserDestr();

private:
    QString remIpPort2key(QHostAddress sender, quint16 port);

    QDateTime dtFromDataGram(const QByteArray &readArr, QByteArray &remDtArr);


    struct RemNtpUdpClient{
        QHostAddress sender;
        quint16 port;
        QByteArray remDtArr;
        QDateTime dtReadUtc;
    };

    QHash<QString, RemNtpUdpClient > hashQueue;
    QHash<QString, QDateTime> hashQueueDt;
    QStringList queueList;
    quint64 queueMaxSize;
    bool verboseMode;

    quint64 maximumThreads;
    quint64 activeThreads;

    qint64 secsInQueue;
    bool sayServerIsBusy;

    QDateTime lastCurrDt;
    QDateTime dtRelease;

};

#endif // NTPRESPONSEMANAGER_H
