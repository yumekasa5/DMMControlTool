#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DMMControl.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    mCtrl(new DMMControl)
{
    ui->setupUi(this);

    connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);

    mCtrl->start();
}

MainWindow::~MainWindow()
{
    mCtrl->quit();
    mCtrl->wait();
    delete ui;
}

void MainWindow::on_pushButton_Open_clicked()
{
    QString ipaddress = "127.0.0.1";
    quint16 port = 8080;
    QString message = QString("Connect to server...[%1 : %2]").arg(ipaddress).arg(port);
    emit newMessage(message);
    mCtrl->GetInstance()->Open(ipaddress, port);
}

void MainWindow::on_pushButton_Close_clicked()
{
  QString message = QString("Disconnect from server...");
  emit newMessage(message);
  mCtrl->GetInstance()->Close();
}

void MainWindow::displayMessage(const QString& msg)
{
  ui->textBrowser_Log->append(msg);
}


void MainWindow::on_pushButton_SendCmd_clicked()
{
  bool ret = false;
  //! コマンドの送信
  QString cmd = ui->lineEdit_Command->text();
  QString message = QString("-> %1").arg(cmd);
  emit newMessage(message);
  ret = mCtrl->GetInstance()->SendCommand(cmd);
  if(ret){
      ui->lineEdit_Command->clear();
  }else{
      QMessageBox::critical(this, "Send Command", "Failed to send command to server.");
      emit newMessage("Failed to send command");
  }
}

