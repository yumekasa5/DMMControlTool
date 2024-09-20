#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DMMControl.h"
#include "SCPICommandDefine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    mCtrl(new DMMControl)
{
    ui->setupUi(this);

    connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
    ui->comboBox->addItem(IDENTIFY_DEVICE);
    ui->comboBox->addItem(RESET_DEVICE);
    ui->comboBox->addItem(SENSING);
    ui->comboBox->addItem(VOLT);

    ui->lineEdit_IpAddress->setText("192.168.100.13"); // デフォルト:192.168.100.13
    ui->lineEdit_Port->setText("5025");                // デフォルト:5025

    mCtrl->start();
    qDebug() << "[DMMControl]:"  << mCtrl->currentThread();
    qDebug() << "[GUI]" << QThread::currentThread();
}

MainWindow::~MainWindow()
{
    mCtrl->quit();
    mCtrl->wait();
    delete ui;
}

void MainWindow::on_pushButton_Open_clicked()
{
    QString ipaddress = ui->lineEdit_IpAddress->text();
    quint16 port = ui->lineEdit_Port->text().toUShort();
    QString message = QString("Connect to server...[%1 : %2]").arg(ipaddress).arg(port);
    emit newMessage(message);

    // 別スレッドで非同期でOpen処理を行うようにしたい
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
//  QString cmd = ui->lineEdit_Command->text();
  QString cmd = ui->comboBox->currentText();
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

