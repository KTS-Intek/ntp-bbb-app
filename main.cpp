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

#include <QCoreApplication>
#include <QtCore>
#include "udpservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QThread *t = new QThread;
    UdpService *s = new UdpService(qApp->arguments().contains("-vv"), 123);
    s->moveToThread(t);
    QObject::connect(t, SIGNAL(started()), s, SLOT(onThreadStarted()) );
    QTimer::singleShot(555, t, SLOT(start()));

    return a.exec();
}
