#ifndef DMMCOMCONTROL_H
#define DMMCOMCONTROL_H

#include <QObject>
#include <QMutex>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QThread>

class DMMComControl : public QObject
{
    Q_OBJECT
public:
    explicit DMMComControl(QObject *parent = nullptr);
    ~DMMComControl();

    void Open(const QString &ipaddress, const quint16 &port);
    void Close();
    bool CheckIsOpened();

    bool ReadSocket();
    bool DiscardSocket();

private:
    bool mIsOpened;
    QMutex mMutexIsOpened;
    QMutex mMutexConnect;

    //! TCPクライアントオブジェクト
    QTcpSocket *mTcpClientSocket;

    //! 制御スレッド
    QThread mThread;

};

#endif // DMMCOMCONTROL_H
