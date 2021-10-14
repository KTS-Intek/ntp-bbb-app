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

#ifndef NTPLOCALSOCKET_H
#define NTPLOCALSOCKET_H

///[!] ipc
#include "localsockets/regularlocalsocket.h"

class NTPLocalSocket : public RegularLocalSocket
{
    Q_OBJECT
public:
    explicit NTPLocalSocket(bool verboseMode, QObject *parent = nullptr);

    void decodeReadData(const QVariant &dataVar, const quint16 &command);


signals:
    //default signals
    void reloadSett();
    void killApp();


public slots:
    //for client side


};

#endif // NTPLOCALSOCKET_H
