#ifndef SHAREDMEMORYMANAGER_H
#define SHAREDMEMORYMANAGER_H

#include <QObject>
#include <QSharedMemory>
#include <QHostAddress>
#include <QDateTime>

class SharedMemoryManager : public QObject
{
    Q_OBJECT
public:
    explicit SharedMemoryManager(const bool &verboseMode, QObject *parent = 0);

signals:
    void startTmrSave2file();

    void startTmrSave2sharedMemoLater();


public slots:    

    void onThreadStarted();

    void add2systemLogError(QString err);
    void add2systemLogWarn(QString warn);
    void add2systemLogEvent(QString evnt);

    void add2ipHistory(QHostAddress host, QDateTime dtReadUtc, QDateTime dtRemoteUtc);

    void saveSharedMemory2file();

    void onTmrAdd2sharedMemoLater();

    void startTmrSave2fileLater();




private:
    void reloadFromMemory();

    void add2sharedMemo(QVariantHash hashStat);


    QSharedMemory shmem;
    QVariantHash hashStatus;

    bool verboseMode;
    bool tmrSave2fileIsRun;
    quint8 tmrLaterCounter;


};

#endif // SHAREDMEMORYMANAGER_H
