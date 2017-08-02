#include "sharedmemorymanager.h"
#include "settloader4matilda.h"
#include "matildaprotocolhelper.h"

#include <QtCore>
#include <QSystemSemaphore>

//-----------------------------------------------------------------------------------------
SharedMemoryManager::SharedMemoryManager(const bool &verboseMode, QObject *parent) : QObject(parent)
{
    this->verboseMode = verboseMode;
}
//-----------------------------------------------------------------------------------------
void SharedMemoryManager::onThreadStarted()
{

    reloadFromMemory();
    shmem.setKey(SettLoader4matilda::defSntpServerMemoName());
/*
 * hashStatus
 * ntp-err      300 lines
 * ntp-ev       300 lines
 * ntp-warn     300 lines
 * ntp-lastip  remote IP|local time|remote time   5000 lines
*/

    if(verboseMode)
        qDebug() << "shared memo is ready " << shmem.errorString();

    tmrSave2fileIsRun = false;

    QTimer *tmrSharedMemo2file = new QTimer;
    tmrSharedMemo2file->setInterval(180000);
    tmrSharedMemo2file->setSingleShot(true);
    connect(this, SIGNAL(startTmrSave2file()), tmrSharedMemo2file, SLOT(start()) );
    connect(tmrSharedMemo2file, SIGNAL(timeout()), this, SLOT(saveSharedMemory2file()) );

    QTimer *tmrSave2sharedMemoLater = new QTimer;
    tmrSave2sharedMemoLater->setSingleShot(true);
    tmrSave2sharedMemoLater->setInterval(1111);
    connect(this, SIGNAL(startTmrSave2sharedMemoLater()), tmrSave2sharedMemoLater, SLOT(start()) );
    connect(tmrSave2sharedMemoLater, SIGNAL(timeout()), this, SLOT(onTmrAdd2sharedMemoLater()) );

    tmrLaterCounter = 0;
    startTmrSave2fileLater();

}
//-----------------------------------------------------------------------------------------
void SharedMemoryManager::add2systemLogError(QString err)
{
   hashStatus.insert("ntp-err", MatildaProtocolHelper::addLine2list(err, hashStatus.value("ntp-err").toStringList(), 300));
   startTmrSave2fileLater();
}
//-----------------------------------------------------------------------------------------
void SharedMemoryManager::add2systemLogWarn(QString warn)
{
    hashStatus.insert("ntp-warn", MatildaProtocolHelper::addLine2list(warn, hashStatus.value("ntp-warn").toStringList(), 300));
    startTmrSave2fileLater();
}
//-----------------------------------------------------------------------------------------
void SharedMemoryManager::add2systemLogEvent(QString evnt)
{
    hashStatus.insert("ntp-ev", MatildaProtocolHelper::addLine2list(evnt, hashStatus.value("ntp-ev").toStringList(), 300));
    startTmrSave2fileLater();
}
//-----------------------------------------------------------------------------------------
void SharedMemoryManager::add2ipHistory(QList<QHostAddress> lhost, QDateTime dtReadUtc, QList<QDateTime> lDtRemoteUtc, int counter)
{

    QStringList l;
    QString dts = dtReadUtc.toString("yyyyMMddhhmmsszzz");
    for(int i = 0; i < counter; i++)
        l.append(QString("%1\t%2\t%3")
                 .arg(SettLoader4matilda::showNormalIP(lhost.at(i).toString()))
                 .arg(dts)
                 .arg(lDtRemoteUtc.at(i).toString("yyyyMMddhhmmsszzz")));


    hashStatus.insert("ntp-lastip", MatildaProtocolHelper::addLines2list(l, hashStatus.value("ntp-lastip").toStringList(), 5000));

    startTmrSave2fileLater();

}
//-----------------------------------------------------------------------------------------
void SharedMemoryManager::saveSharedMemory2file()
{
    tmrSave2fileIsRun = false;
    QDir dir(SettLoader4matilda::path2config() + "/tmp/");
    if(!dir.exists())
        dir.mkpath(SettLoader4matilda::path2config() + "/tmp/");

    QSaveFile sFile(SettLoader4matilda::path2config() + "/tmp/ntp-bbb");
    if(sFile.open(QFile::WriteOnly|QFile::Unbuffered)){
        sFile.write(MatildaProtocolHelper::compressVarHash(hashStatus));
        bool r = sFile.commit();
        if(!r && verboseMode)
            qDebug() << "Can't save file " << sFile.fileName() << sFile.errorString();

        if(verboseMode){
            QList<QString> lk = hashStatus.keys();
            std::sort(lk.begin(), lk.end());
            qDebug() << "hashStatus saved=" << r;
            for(int i = 0, iMax = lk.size(); i < iMax; i++){
                qDebug() << "hashStatus key " << lk.at(i);
                QStringList l = hashStatus.value(lk.at(i)).toStringList();
                for(int j = 0, jMax = l.size(); j < jMax; j++)
                    qDebug() << l.at(j);
            }
            qDebug() << "hashStatus end";

        }
    }
}
//-----------------------------------------------------------------------------------------
void SharedMemoryManager::onTmrAdd2sharedMemoLater()
{
    tmrLaterCounter = 0;
    add2sharedMemo(hashStatus);
}
//-----------------------------------------------------------------------------------------
void SharedMemoryManager::startTmrSave2fileLater()
{
    tmrLaterCounter++;
    if(tmrLaterCounter > 10)
        onTmrAdd2sharedMemoLater();
    else if(tmrLaterCounter < 5)
        emit startTmrSave2sharedMemoLater();
}

//-----------------------------------------------------------------------------------------
void SharedMemoryManager::reloadFromMemory()
{
    QSystemSemaphore sema(SettLoader4matilda::defSntpServerSemaName(), 1);
    sema.acquire();
#ifndef Q_OS_WIN
    QSharedMemory nix_fix_shmem(SettLoader4matilda::defSntpServerMemoName());
    if(nix_fix_shmem.attach()){
        if(verboseMode)
            qDebug() << "Found shared memory form crashed ntp-bbb. Detaching...";
        QBuffer buffer;
        nix_fix_shmem.lock();
        if(verboseMode)
            qDebug() << "shared memory size " << nix_fix_shmem.size();

        buffer.setData((char *)nix_fix_shmem.constData(), nix_fix_shmem.size());
        buffer.open(QBuffer::ReadOnly);
        QDataStream in(&buffer);
        in.setVersion(QDataStream::Qt_5_6);

        in >> hashStatus;
        nix_fix_shmem.unlock();
//дані не скидаю, вони будуть потрібні

    }else{
        QFile file(SettLoader4matilda::path2config() + "/tmp/ntp-bbb");
        if(file.open(QFile::ReadOnly)){
            bool ok;
            hashStatus = MatildaProtocolHelper::uncompress2varHash(file.readAll(), ok);
            qDebug() << "file nix fix " <<  hashStatus.keys() << ok;

        }
        file.close();
    }
#endif
    sema.release();
}
//-----------------------------------------------------------------------------------------
void SharedMemoryManager::add2sharedMemo(QVariantHash hashStat)
{

    QBuffer buffer;
    buffer.open(QBuffer::WriteOnly);
    QDataStream out(&buffer);
    out.setVersion(QDataStream::Qt_5_6);
    out << hashStat;
    int size = buffer.size();

    QSystemSemaphore sema(SettLoader4matilda::defSntpServerSemaName(), 1);
    sema.acquire();

    if(shmem.isAttached())
        shmem.detach();

//    qDebug() << "shared memory size=, data=" << shmem.size() << size << shmem.key();

    if(!shmem.create(size)){
        shmem.attach();
        shmem.lock();
        shmem.unlock();
        shmem.detach();
        if(verboseMode)
            qDebug() << "can't create shared memory " << size << shmem.errorString() << shmem.key();
        sema.release();

        return;
    }

    shmem.lock();
    char *to = (char *)shmem.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(shmem.size(), size));
    shmem.unlock();

    sema.release();

    if(!tmrSave2fileIsRun ){
        tmrSave2fileIsRun = true;
        emit startTmrSave2file();
    }
}
//-----------------------------------------------------------------------------------------
