#include "DMMComControl.h"
#include "SCPICommandDefine.h"

#include <QDebug>
#include <QMutexLocker>

DMMComControl::DMMComControl(QObject *parent)
    : QObject{parent},
    mIsOpened(false),
    mTcpClientSocket(new QTcpSocket(this))
{
    connect(mTcpClientSocket, &QTcpSocket::readyRead, this, &DMMComControl::ReadSocket);

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
        mIsOpened = true;
    } else {
        qDebug() << "Failed to connect to server: ";
        mIsOpened = false;
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
        mIsOpened = false;
    }else{
        ret = false;
    }
    if(ret){
        qDebug() << "Disconnected from server";
    }else{
        qDebug() << "No active connection to close";
    }
}

bool DMMComControl::SendCommand(const QString &data) const
{
    if(!mIsOpened){
        return false;
    }else{
        QByteArray byte_data = data.toLocal8Bit();

        QDataStream socketStream(mTcpClientSocket);
        socketStream.setVersion(QDataStream::Qt_5_15);

        QByteArray header;
        header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(data.size()).toUtf8());
        header.resize(128);
        byte_data.prepend(header);  // header(128byte) + dataの形にする

        //! 送信処理
        socketStream << byte_data;
        return true;
    }

}

bool DMMComControl::GetDeviceInfoCmd(QString &info)
{
    QByteArray buffer;
    bool ret = false;
    if(ret){
        info = QString::fromLatin1(buffer);
    }
    return ret;
}

void DMMComControl::ReadSocket()
{
    QByteArray buffer;

//    QDataStream socketStream(mTcpClientSocket);
//    socketStream.setVersion(QDataStream::Qt_5_15);

//    socketStream.startTransaction();
//    socketStream >> buffer;

//    QString header = buffer.mid(0, 128);
//    QString fileType = header.split(",")[0].split(":")[1];

//    buffer = buffer.mid(128);  // bufferの位置128以降のデータを読み出す

    buffer = mTcpClientSocket->readAll();
    qDebug() << "Received data:" << buffer;
    mReceiveData = buffer;
}

bool DMMComControl::DiscardSocket()
{
    mTcpClientSocket->deleteLater();
    mTcpClientSocket = nullptr;
    return true;
}
