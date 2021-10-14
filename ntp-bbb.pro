##/****************************************************************************
##**
##**   Copyright © 2016-2017 The KTS-INTEK Ltd.
##**   Contact: http://www.kts-intek.com.ua
##**   bohdan@kts-intek.com.ua
##**
##**  This file is part of ntp-bbb.
##**
##**  ntp-bbb is free software: you can redistribute it and/or modify
##**  it under the terms of the GNU General Public License as published by
##**  the Free Software Foundation, either version 3 of the License, or
##**  (at your option) any later version.
##**
##**  ntp-bbb is distributed in the hope that it will be useful,
##**  but WITHOUT ANY WARRANTY; without even the implied warranty of
##**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##**  GNU General Public License for more details.
##**
##**  You should have received a copy of the GNU General Public License
##**  along with ntp-bbb.  If not, see <http://www.gnu.org/licenses/>.
##**
##**************************************************************************/
QT += core network
QT -= gui

CONFIG += c++11

TARGET = ntp-bbb
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

VERSION = 0.0.2

#DEFINES += APPLCTN_NAME=\\\"quick-collect\\\" it is only for GUI
DEFINES += "MYAPPNAME=\"\\\"ntp-bbb\\\"\""
DEFINES += "MYAPPOWNER=\"\\\"KTS-Intek Ltd\\\"\""
DEFINES += "MYAPPOWNERSITE=\"\\\"http://kts-intek.com\\\"\""


#DEFINES += ONLYSMPLSETT=1

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

linux-beagleboard-g++:{
    target.path = /opt/matilda/bin
    INSTALLS += target
}

linux:{
    target.path = /opt/matilda/bin
    INSTALLS += target
}

DEFINES += ENABLE_SETTLOADER4MATILDA

include(../../../Matilda-units/matilda-base/type-converter/type-converter.pri)

include(../../../Matilda-units/ipc/localsockets/localsockets.pri)
include(../../../Matilda-units/ipc/sharedmemory/sharedmemory.pri)
include(../../../Matilda-units/matilda-bbb/matilda-bbb-settings/matilda-bbb-settings.pri)
include(../../../Matilda-units/matilda-base/MatildaIO/MatildaIO.pri)


SOURCES += main.cpp \
    ntp-bbb-src/ntpdataprocessor.cpp \
    ntp-bbb-src/ntpdataprocessorbase.cpp \
    ntp-bbb-src/ntplocalsocket.cpp \
    ntp-bbb-src/ntpprotocolhelper.cpp \
    ntp-bbb-src/ntpresourcemanager.cpp \
    ntp-bbb-src/ntpservice.cpp \
    ntp-bbb-src/ntpservicebase.cpp \
    ntp-bbb-src/ntpsharedmemowriter.cpp


HEADERS += \
    ntp-bbb-src/ntpdataprocessor.h \
    ntp-bbb-src/ntpdataprocessorbase.h \
    ntp-bbb-src/ntplocalsocket.h \
    ntp-bbb-src/ntpprotocolhelper.h \
    ntp-bbb-src/ntpresourcemanager.h \
    ntp-bbb-src/ntpservice.h \
    ntp-bbb-src/ntpservicebase.h \
    ntp-bbb-src/ntpsharedmemowriter.h
