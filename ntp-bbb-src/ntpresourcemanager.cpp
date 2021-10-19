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


#include "ntpresourcemanager.h"


///[!] ntp-bbb
#include "ntp-bbb-src/ntplocalsocket.h"
#include "ntp-bbb-src/ntpsharedmemowriter.h"
#include "ntp-bbb-src/ntpdataprocessor.h"
#include "ntp-bbb-src/ntpservice.h"



///[!] matilda-bbb-settings
#include "src/matilda/settloader4matilda.h"


#include "moji_defy.h"

NTPResourceManager::NTPResourceManager(QObject *parent) : QObject(parent)
{

}

void NTPResourceManager::startNTPService(const bool &verboseMode)
{
    myParams.verboseMode = verboseMode;
#ifdef ENABLE_VERBOSE_SERVER

    myParams.verboseMode = activeDbgMessages;

#endif
    myParams.allowSharedMemory = true;

    QTimer::singleShot(54, this, SLOT(initObject()));

}

void NTPResourceManager::initObject()
{
//    createLocalSocket(); for test only

    if(myParams.allowSharedMemory){
        createSharedMemory();
    }

    QTimer::singleShot(333, this, SLOT(createTimerServer()));

}

void NTPResourceManager::onConfigChanged()
{
    //reloadConfiguration

     SettLoader4matilda sLoader;

    const QVariantHash h = sLoader.loadOneSett(SETT_SNTP_SERVICE).toHash();
//    h.insert("queue", 1000);
//    h.insert("secs", 30);
//    h.insert("thrd", 50);

    emit setQueueParams(
                SettLoader4matilda::integerValidator(h.value("queue").toULongLong(), 10, 5000),
                SettLoader4matilda::integerValidator(h.value("secs").toULongLong(), 10, 60),
                SettLoader4matilda::integerValidator(h.value("thrd").toULongLong(), 10, 500));

    emit setAllowAndBlockList(
                sLoader.loadOneSett(SETT_ALLOW_IP_LIST).toStringList(),
                sLoader.loadOneSett(SETT_BLOCK_IP_LIST).toStringList());


}

void NTPResourceManager::killApp()
{

    emit killAllObjects();
    QThread::sleep(1);
    qApp->exit(APP_CODE_RESTART);
}

void NTPResourceManager::onFailedToStartNTPService()
{
    myParams.ntpServiceFailsConter++;
    if(myParams.ntpServiceFailsConter > 10)
        killApp();
}

void NTPResourceManager::createSharedMemory()
{
    QThread *writerthred = new QThread;
    writerthred->setObjectName("NTPSharedMemoWriter");

    NTPSharedMemoWriter *writer = new NTPSharedMemoWriter(
                SharedMemoHelper::defSntpServerMemoName(),
                SharedMemoHelper::defSntpServerSemaName(),
                "", 2222, 60000, myParams.verboseMode);

    writer->mymaximums.write2ram = 120;
    writer->mymaximums.write2file = 250;

    writer->moveToThread(writerthred);
    connect(writer, SIGNAL(destroyed(QObject*)), writerthred, SLOT(quit()));
    connect(writerthred, SIGNAL(finished()), writerthred, SLOT(deleteLater()));
    connect(writerthred, SIGNAL(started()), writer, SLOT(initObjectLtr()));

    connect(this, &NTPResourceManager::killAllObjects, writer, &SharedMemoWriter::flushAllNowAndDie);

    connect(this, &NTPResourceManager::add2ipHistory, writer, &NTPSharedMemoWriter::add2ipHistory);
    connect(this, &NTPResourceManager::add2systemLogError, writer, &NTPSharedMemoWriter::add2systemLogError);
    connect(this, &NTPResourceManager::add2systemLogEvent, writer, &NTPSharedMemoWriter::add2systemLogEvent);
    connect(this, &NTPResourceManager::add2systemLogWarn , writer, &NTPSharedMemoWriter::add2systemLogWarn);

//    connect(writer, &NTPSharedMemoWriter::onRestoreOldVersion, )

//    writer->initObject(true);
//    writerthred->start();
    writerthred->start();




}

void NTPResourceManager::createLocalSocket()
{
    NTPLocalSocket *extSocket = new NTPLocalSocket(myParams.verboseMode);
    extSocket->activeDbgMessages = myParams.activeDbgMessages;
    extSocket->initializeSocket(MTD_EXT_NAME_SNTP_SERVICE);

    QThread *extSocketThrd = new QThread;
    extSocketThrd->setObjectName("NTPLocalSocket");
    extSocket->moveToThread(extSocketThrd);
#ifdef ENABLE_VERBOSE_SERVER
    connect(extSocket, &NTPLocalSocket::appendDbgExtData, this, &ZbyratorManager::appendDbgExtData );
#endif
    connect(extSocketThrd, &QThread::started, extSocket, &NTPLocalSocket::onThreadStarted);

    connect(extSocket, &NTPLocalSocket::onConfigChanged , this, &NTPResourceManager::onConfigChanged  );
    connect(extSocket, &NTPLocalSocket::killApp         , this, &NTPResourceManager::killApp  );

    connect(this, &NTPResourceManager::killAllObjects, extSocket, &NTPLocalSocket::killAllObjects);

    connect(extSocket, SIGNAL(destroyed(QObject*)), extSocketThrd, SLOT(quit()));
    connect(extSocketThrd, SIGNAL(finished()), extSocketThrd, SLOT(deleteLater()));


//    connect(extSocket, &NTPLocalSocket::command4dev     , zbyrator, &MeterManager::command4devStr      );

    extSocketThrd->start();



}

void NTPResourceManager::createTimerServer()
{
    NTPService *service = new NTPService(myParams.verboseMode);
    QThread *tservice = new QThread;

    service->moveToThread(tservice);

    connect(tservice, SIGNAL(started()), service, SLOT(onThreadStarted()));
    connect(service, SIGNAL(destroyed(QObject*)), tservice, SLOT(quit()));
    connect(tservice, SIGNAL(finished()), tservice, SLOT(deleteLater()));

    connect(this, &NTPResourceManager::killAllObjects, service, &NTPService::killAllObjects);

    connect(service, &NTPService::add2systemLogError, this, &NTPResourceManager::add2systemLogError);
    connect(service, &NTPService::add2systemLogEvent, this, &NTPResourceManager::add2systemLogEvent);
    connect(service, &NTPService::add2systemLogWarn, this, &NTPResourceManager::add2systemLogWarn);

    connect(service, &NTPService::onFailedToStartNTPService, this, &NTPResourceManager::onFailedToStartNTPService);




    NTPDataProcessor *processor = new NTPDataProcessor(myParams.verboseMode);
    QThread *tprocessor = new QThread;

    processor->moveToThread(tprocessor);
    connect(tservice, SIGNAL(started()), processor, SLOT(onThreadStarted()));
    connect(processor, SIGNAL(destroyed(QObject*)), tprocessor, SLOT(quit()));
    connect(tprocessor, SIGNAL(finished()), tprocessor, SLOT(deleteLater()));

    connect(this, &NTPResourceManager::killAllObjects, processor, &NTPDataProcessor::killAllObjects);


    connect(processor, &NTPDataProcessor::add2systemLogError, this, &NTPResourceManager::add2systemLogError);
    connect(processor, &NTPDataProcessor::add2systemLogEvent, this, &NTPResourceManager::add2systemLogEvent);
    connect(processor, &NTPDataProcessor::add2systemLogWarn, this, &NTPResourceManager::add2systemLogWarn);

    connect(processor, &NTPDataProcessor::add2ipHistory, this, &NTPResourceManager::add2ipHistory);

    connect(this, &NTPResourceManager::setQueueParams, processor, &NTPDataProcessor::setQueueParams);
    connect(this, &NTPResourceManager::setAllowAndBlockList, processor, &NTPDataProcessor::setAllowAndBlockList);


    connect(tprocessor, SIGNAL(started()), tservice, SLOT(start()));


    connect(service, &NTPService::addThisHost2queue, processor, &NTPDataProcessor::addThisHost2queue);
    connect(service, &NTPService::blockThisIP, processor, &NTPDataProcessor::blockThisIP);

    connect(processor, &NTPDataProcessor::sendDt2clnt, service, &NTPService::sendDt2clnt);


    connect(processor, &NTPDataProcessor::gimmeConfiguration, this, &NTPResourceManager::onConfigChanged);
    tprocessor->start();


}


