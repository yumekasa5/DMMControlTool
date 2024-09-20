#include "DMMControl.h"


DMMControl::DMMControl(QObject *parent)
    : QThread{parent},
    mCtrl(new DMMComControl(this))
{
    qDebug() << "[DMMComControl]:" << QThread::currentThread();

}

DMMControl::~DMMControl()
{
    delete mCtrl;

}

void DMMControl::run()
{
    exec();
}

DMMComControl *DMMControl::GetInstance()
{
    return mCtrl;
}
