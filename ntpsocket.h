#ifndef NTPSOCKET_H
#define NTPSOCKET_H

#include <QLocalSocket>
#include <QObject>
#include <QtCore>

class NtpSocket : public QLocalSocket
{
    Q_OBJECT
public:
    explicit NtpSocket(const bool &verboseMode, QObject *parent = 0);

signals:
    void startReconnTmr();

    void startZombieKiller();
    void stopZombieKiller();

    void startZombieDetect();
    void stopZombieDetect();


//    void onConfigChanged(quint16, QVariant);

//    void command4dev(quint16,QString);

    void reloadSettings();

public slots:
    //for client side
    void initializeSocket(quint16 mtdExtName);
    void command2extensionClient(quint16 command, QVariant dataVar);

    void connect2extension();
    void stopConnection();



private slots:
    void mReadyRead();
    void mWrite2extension(const QVariant &s_data, const quint16 &s_command);
    void onDisconn();

    void onZombie();



private:
    void decodeReadData(const QVariant &dataVar, const quint16 &command);

    void mReadyReadF();

    quint16 mtdExtName;
    QTime timeHalmo;
    quint8 zombieNow;
    bool stopAll, verboseMode;

    int inConn;

};

#endif // NTPSOCKET_H
