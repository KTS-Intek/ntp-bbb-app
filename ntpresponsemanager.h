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
    explicit NTPResponseManager(const bool &enVerboseMode, const QDateTime &dtRelease, const bool &enShMem, QObject *parent = 0);

    static QByteArray getArrDateTimeStamp(const QDateTime &dt);
    static QByteArray getCurrentNtpTimeLeftPart(const QByteArray &dtReadArr, const QByteArray &dtRemoteArr);

signals:
    void startTmrQueue(int msec);

    void startTmrCheckConf();

    void sendDt2clnt(QList<QHostAddress> remSender, QList<quint16> remPort, QList<QByteArray> leftArr, quint32 counter);

//    void sendDt2clnt(QHostAddress remSender, quint16 remPort, QByteArray dtReadArr, QDateTime dtReadUtc);


    //4shared memory
    void add2systemLogError(QString err);
    void add2systemLogWarn(QString warn);
    void add2systemLogEvent(QString evnt);
    void saveSharedMemory2file();

    void add2ipHistory(QList<QHostAddress> host, QDateTime dtReadUtc, QList<QDateTime> dtRemoteUtc, int counter);

    void onCountersChanged(quint32 killedObjectCounter, quint32 createdObjectCounter, quint32 queueIsEmptyCounter);

public slots:
    void addThisHost2queue(QList<QHostAddress> lsender, QList<quint16> lport, QList<QByteArray> ldatagram, QDateTime dtReadUtc, int counter);

    void checkQueue();

    void onThreadStarted();

    void onResponserDestr(quint32 counter);

    void reloadConfiguration();

private:
    QString remIpPort2key(QHostAddress sender, quint16 port);

    QDateTime dtFromDataGram(const QByteArray &readArr, QByteArray &remDtArr);



    struct RemNtpUdpClient{
        QHostAddress sender;
        quint16 port;
        QByteArray leftArr;
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

    quint32 killedObjectCounter;
    quint32 createdObjectCounter;
    quint32 queueIsEmptyCounter;

    bool allowSharedMemory;

};

#endif // NTPRESPONSEMANAGER_H
