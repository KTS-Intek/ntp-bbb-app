#ifndef SNTPLOCALSOCKET_H
#define SNTPLOCALSOCKET_H

#include "localsockettmplt.h"
#include <QObject>


class SntpLocalSocket : public LocalSocketTmplt
{
    Q_OBJECT
public:
    explicit SntpLocalSocket(QObject *parent = 0);

signals:

public slots:
    void onConfigChangedSlot(quint16 command, QVariant var);

    void onThreadStarted();

};

#endif // SNTPLOCALSOCKET_H
