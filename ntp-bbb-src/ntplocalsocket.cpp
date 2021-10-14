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

#include "ntplocalsocket.h"

#include "moji_defy.h"
#include "dbgaboutsourcetype.h"

NTPLocalSocket::NTPLocalSocket(bool verboseMode, QObject *parent) :
    RegularLocalSocket(verboseMode, parent)
{
    // dont forget to     void initializeSocket(quint16 mtdExtName);

}

void NTPLocalSocket::decodeReadData(const QVariant &dataVar, const quint16 &command)
{
    //only commands for ntp-bbb
    switch(command){
    //       case MTD_EXT_CUSTOM_COMMAND_0: {
    //           if(verboseMode) qDebug() << "ext " << mtdExtName << dataVar;
    //    #ifdef ENABLE_VERBOSE_SERVER
    //           if(activeDbgMessages)
    //               emit appendDbgExtData(DBGEXT_THELOCALSOCKET, QString("command r: %1, data=%2").arg(command).arg(dataVar.toHash().value("d").toString()));
    //    #endif
    //           emit command4dev(dataVar.toHash().value("c").toUInt(), dataVar.toHash().value("d").toString());
    //           break;}

    case MTD_EXT_COMMAND_RELOAD_SETT: emit reloadSett(); break;
    case MTD_EXT_COMMAND_RESTART_APP: emit killApp()   ;  break;

    default: {
        if(verboseMode)
            qDebug() << "default ext " << command << mtdExtName << dataVar;
        emit onConfigChanged(command,dataVar);
        break;}
    }
}
