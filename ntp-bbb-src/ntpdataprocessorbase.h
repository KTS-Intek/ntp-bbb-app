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

#ifndef NTPDATAPROCESSORBASE_H
#define NTPDATAPROCESSORBASE_H

#include <QObject>
#include <QDateTime>
#include <QHostAddress>
#include <QStringList>

///[!] type-converter
#include "src/shared/networkconverthelper.h"


class NTPDataProcessorBase : public QObject
{
    Q_OBJECT
public:
    explicit NTPDataProcessorBase(const bool &verboseMode, QObject *parent = nullptr);

    struct NTPDataProcessorParams
    {
        bool verboseMode;

        QStringList queueList;
        quint64 queueMaxSize;
        quint64 secsInQueue;
        quint64 maximumThreads;


        NTPDataProcessorParams() :
            verboseMode(false),
            queueMaxSize(9), secsInQueue(9), maximumThreads(9) {}
    } myParams;



    struct NTPQueueItem
    {
        //it contains RemNtpUdpClient and dt
        QHostAddress sender;
        quint16 port;
        QByteArray leftArr;

        qint64 secWhen; //I need it know how long it is in the queue, yes it is seconds

        NTPQueueItem() : port(0), secWhen(0) {}
    };

    struct NTPQueue
    {
        QHash<QString, NTPQueueItem > hashQueueTable;
        QStringList queue;
        NTPQueue() {}
    } myQueue;




    QString remIpPort2key(QHostAddress ipAddr, quint16 port);



    bool addThisClient2queueSmart(const QHostAddress &addr, const quint16 &remPort, const QByteArray &datagramArr, const QDateTime &dtReadUtc, QDateTime &dtRemote);

    void removeThisKeyFromTheQueue(const QString &key);
    void addThisClientKeyToTheQueue(const QString &key, const NTPQueueItem &client);

    bool checkSendQueue();

signals:
    //4shared memory
    void add2systemLogError(QString err);
    void add2systemLogWarn(QString warn);
    void add2systemLogEvent(QString evnt);

    void add2ipHistory(QList<QHostAddress> host, QDateTime dtReadUtc, QList<QDateTime> dtRemoteUtc, int counter);

//to ntp service
    void sendDt2clnt(QList<QHostAddress> remSender, QList<quint16> remPort, QList<QByteArray> leftArr, quint32 counter);

public slots:
    void setQueueParams(quint64 queueMaxSize, quint64 secsInQueue, quint64 maximumThreads);


};

#endif // NTPDATAPROCESSORBASE_H
