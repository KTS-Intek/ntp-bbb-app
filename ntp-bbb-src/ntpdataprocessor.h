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

#ifndef NTPDATAPROCESSOR_H
#define NTPDATAPROCESSOR_H

///[!] ntp-bbb
#include "ntp-bbb-src/ntpdataprocessorbase.h"


#include <QTimer>



//it processes data from NTP service
//and generates answers or ignores if the received IP is blocked

class NTPDataProcessor : public NTPDataProcessorBase
{
    Q_OBJECT
public:
    explicit NTPDataProcessor(const bool &verboseMode, QObject *parent = nullptr);




signals:

//local
    void startTmrQueue(int msec);

public slots:
    void onThreadStarted();

    void addThisHost2queue(QList<QHostAddress> lsender, QList<quint16> lport, QList<QByteArray> ldatagram, QDateTime dtReadUtc, int counter);




    void checkQueue();



    void killAllObjects();

};

#endif // NTPDATAPROCESSOR_H
