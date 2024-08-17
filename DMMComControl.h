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

    bool SendCommand(const QString &data) const;

    //! コマンド
    bool GetDeviceInfoCmd(QString &info);

    bool DiscardSocket();

private slots:
    void ReadSocket();

private:
    bool mIsOpened;
    QMutex mMutexIsOpened;
    QMutex mMutexConnect;

    QString mReceiveData;

    //! TCPクライアントオブジェクト
    QTcpSocket *mTcpClientSocket;

    //! 制御スレッド
    QThread mThread;

};

#endif // DMMCOMCONTROL_H
