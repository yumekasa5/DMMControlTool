#include "DMMComControl.h"

#include <QDebug>
#include <QMutexLocker>

DMMComControl::DMMComControl(QObject *parent)
    : QObject{parent},
    mIsOpened(false),
    mTcpClientSocket(new QTcpSocket(this))
{

}

DMMComControl::~DMMComControl()
{
    bool ret = false;
    if(!mIsOpened){
//        ret = Close();
    }else{
        ret = true;
    }

    if(!ret){
        qDebug("Error");
    }
    delete mTcpClientSocket;

}

void DMMComControl::Open(const QString &ipaddress, const quint16 &port)
{
    QMutexLocker lock(&mMutexConnect);
    bool ret = false;

    mTcpClientSocket->connectToHost(ipaddress, port);
    ret = mTcpClientSocket->waitForConnected(3000);
    // Check if the connection is successful
    if (ret) {
        qDebug() << "Connected to server";
    } else {
        qDebug() << "Failed to connect to server: ";
    }
}

void DMMComControl::Close()
{
    QMutexLocker lock(&mMutexConnect);
    bool ret = true;
    mIsOpened = (mTcpClientSocket->state() == QAbstractSocket::ConnectedState);
    if(mIsOpened){
        mTcpClientSocket->disconnectFromHost();
        if(mTcpClientSocket->state() != QAbstractSocket::UnconnectedState){
            mTcpClientSocket->waitForDisconnected(3000);
        }
    }else{
        ret = false;
    }
    if(ret){
        qDebug() << "Disconnected from server";
    }else{
        qDebug() << "No active connection to close";
    }
}

bool DMMComControl::ReadSocket()
{
    QByteArray buffer;

    QDataStream socketStream(mTcpClientSocket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> buffer;

    QString header = buffer.mid(0, 128);
    QString fileType = header.split(",")[0].split(":")[1];

    buffer = buffer.mid(128);

    return true;

}

bool DMMComControl::DiscardSocket()
{
    mTcpClientSocket->deleteLater();
    mTcpClientSocket = nullptr;
    return true;
}
