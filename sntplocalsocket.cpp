#include "sntplocalsocket.h"
//-----------------------------------------------------------------------------------------------
SntpLocalSocket::SntpLocalSocket(QObject *parent) : LocalSocketTmplt(parent)
{
    setVerboseMode(false);
    initializeSocket(MTD_EXT_NAME_SNTP_SERVICE);

}
//-----------------------------------------------------------------------------------------------
void SntpLocalSocket::onConfigChangedSlot(quint16 command, QVariant var)
{
    if(!var.isValid())
        return;
    qDebug() << "onConfigChangedSlot =" << command << var;
}
//-----------------------------------------------------------------------------------------------
void SntpLocalSocket::onThreadStarted()
{
    connect(this, SIGNAL(onConfigChanged(quint16,QVariant)), this, SLOT(onConfigChangedSlot(quint16,QVariant)) );
    objInit();
}
//-----------------------------------------------------------------------------------------------
