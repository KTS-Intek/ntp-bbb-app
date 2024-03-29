/****************************************************************************
**
**   Copyright © 2016-2021 The KTS-INTEK Ltd.
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
//#include "udpservice.h"

#include "ntp-bbb-src/ntpresourcemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NTPResourceManager manager;
    manager.startNTPService(qApp->arguments().contains("-vv"));



//    QThread *t = new QThread;

//    t->setObjectName("UdpService");
//    UdpService *s = new UdpService(qApp->arguments().contains("-vv"), 123);
//    s->moveToThread(t);
//    QObject::connect(t, SIGNAL(started()), s, SLOT(onThreadStarted()) );
//    QObject::connect(t, SIGNAL(finished()), s, SLOT(saveSharedMemory2file()) );

//    int r = a.exec();
//    t->quit();
//    QThread::sleep(1);//save 2 file

    return a.exec();
}
