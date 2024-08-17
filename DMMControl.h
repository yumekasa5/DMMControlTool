#ifndef DMMCONTROL_H
#define DMMCONTROL_H

#include "DMMComControl.h"

#include <QThread>

class DMMControl : public QThread
{
    Q_OBJECT
public:
    explicit DMMControl(QObject *parent = nullptr);
    ~DMMControl();

    void run() override;
    DMMComControl* GetInstance();

private:
    DMMComControl *mCtrl;

};

#endif // DMMCONTROL_H
