#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTimer"
#include "QSerialPort"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QString id;
    int timer;
    QTimer* qtimer;
    QSerialPort *m_serialPort;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void callAdminLogin();
    void refreshView();
    void userLogin();
    void throwRubbish();
    void updateTimer();
    void receiveMsg();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
