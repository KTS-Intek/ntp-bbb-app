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

#ifndef NTPRESOURCEMANAGER_H
#define NTPRESOURCEMANAGER_H

#include <QObject>
#include <QtCore>
#include <QHostAddress>
#include <QStringList>


class NTPResourceManager : public QObject
{
    Q_OBJECT
public:
    explicit NTPResourceManager(QObject *parent = nullptr);

    void startNTPService(const bool &verboseMode);

signals:

    void killAllObjects();


    //4shared memory
    void add2systemLogError(QString err);
    void add2systemLogWarn(QString warn);
    void add2systemLogEvent(QString evnt);

    void add2ipHistory(QList<QHostAddress> host, QDateTime dtReadUtc, QList<QDateTime> dtRemoteUtc, int counter);


    //to processor
    void setQueueParams(quint64 queueMaxSize, quint64 secsInQueue, quint64 maximumThreads);
    void setAllowAndBlockList(QStringList allowIpList, QStringList blockThisIp);



public slots:
    void initObject();


    void onConfigChanged();

    void killApp();


    void onFailedToStartNTPService();




private slots:
    void createSharedMemory();

    void createLocalSocket();

    void createTimerServer();


private:

    struct ManagerParams
    {
        bool allowSharedMemory;
        bool activeDbgMessages;
        bool verboseMode;

        quint16 ntpServiceFailsConter;

        ManagerParams() : allowSharedMemory(true), activeDbgMessages(false), verboseMode(false),
            ntpServiceFailsConter(0) {}
    } myParams;




};

#endif // NTPRESOURCEMANAGER_H
