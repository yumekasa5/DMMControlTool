#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

class DMMControl;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newMessage(QString);

private slots:
    void on_pushButton_Open_clicked();
    void on_pushButton_Close_clicked();

    void displayMessage(const QString& msg);

private:
    Ui::MainWindow *ui;
    DMMControl *mCtrl;

};
#endif // MAINWINDOW_H
