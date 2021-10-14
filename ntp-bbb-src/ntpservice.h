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

#ifndef NTPSERVICE_H
#define NTPSERVICE_H

///[!] ntp-bbb
#include "ntp-bbb-src/ntpservicebase.h"


#include <QThread>


class NTPService : public NTPServiceBase
{
    Q_OBJECT
public:
    explicit NTPService(const bool &verboseMode, QObject *parent = nullptr);

    static void makeRegistration();

signals:
    //to the parent
    void onFailedToStartNTPService();

    void onNTPServiceIsReady();


public slots:
    void onThreadStarted();

    void killAllObjects();

private:
    bool startNTPService();

};

#endif // NTPSERVICE_H
