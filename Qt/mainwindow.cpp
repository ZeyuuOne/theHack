#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adminwindow.h"
#include "adminlogindialog.h"
#include "userdatabase.h"
#include "adminregisterdialog.h"
#include "QDebug"
#include "QDateTime"
#include "QTimer"
#include "QSerialPort"
#include "QSerialPortInfo"
#include "QFile"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    timer=-1;
    ui->setupUi(this);
    if (userDataBase().isEmpty()){
        adminRegisterDialog* aRD=new adminRegisterDialog(this);
        aRD->exec();
    }
    ui->tableWidgetHistory->setColumnCount(4);
    ui->tableWidgetHistory->setRowCount(20);
    ui->tableWidgetHistory->verticalHeader()->setDefaultSectionSize(10);
    int width = ui->tableWidgetHistory->width()-21;
    ui->tableWidgetHistory->setColumnWidth(0,(int)width*0.3);
    ui->tableWidgetHistory->setColumnWidth(1,(int)width*0.2);
    ui->tableWidgetHistory->setColumnWidth(2,(int)width*0.2);
    ui->tableWidgetHistory->setColumnWidth(3,(int)width-width*0.3-width*0.2-width*0.2);
    ui->tableWidgetHistory->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidgetHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidgetHistory->setHorizontalHeaderLabels(QStringList() << QStringLiteral("垃圾类型") << QStringLiteral("分类") << QStringLiteral("积分") << QStringLiteral("时间"));
    ui->tableWidgetHistory->verticalHeader()->setVisible(false);
    ui->tableWidgetHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetHistory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetHistory->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->labelUserId->clear();
    ui->labelPoint->clear();
    ui->labelTimer->clear();
    connect(ui->actionQuit,&QAction::triggered,this,&QWidget::close);
    connect(ui->actionAdminLogin,&QAction::triggered,this,&MainWindow::callAdminLogin);
    connect(ui->pushButtonUserLogin,&QPushButton::clicked,this,&MainWindow::userLogin);
    connect(ui->pushButtonThrow,&QPushButton::clicked,this,&MainWindow::throwRubbish);
    qtimer = new QTimer(this);
    connect(qtimer, &QTimer::timeout, this, &MainWindow::updateTimer);
    qtimer->start(1000);
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
        qDebug()<<"serialPortName:"<<info.portName();
    }
    m_serialPort = new QSerialPort();
    if(m_serialPort->isOpen())
    {
        m_serialPort->clear();
        m_serialPort->close();
    }
    if (m_serialPortName.empty()) return;
    m_serialPort->setPortName(m_serialPortName[0]);
    if(!m_serialPort->open(QIODevice::ReadWrite))
    {
        qDebug()<<m_serialPortName[0]<<"打开失败!";
        return;
    }
    m_serialPort->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    connect(m_serialPort,&QSerialPort::readyRead,this,&MainWindow::receiveMsg);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
    }
    delete m_serialPort;
}

void MainWindow::callAdminLogin(){
    adminLoginDialog* ald=new adminLoginDialog(this);
    ald->show();
}

void MainWindow::refreshView(){
    ui->tableWidgetHistory->clear();
    ui->tableWidgetHistory->setHorizontalHeaderLabels(QStringList() << QStringLiteral("垃圾类型") << QStringLiteral("分类") << QStringLiteral("积分") << QStringLiteral("时间"));
    userDataBase uDB;
    if (timer>-1) ui->labelTimer->setText(QString::number((timer+4)/5)); else ui->labelTimer->clear();
    if (id==""){
        ui->labelUserId->clear();
        ui->labelPoint->clear();
    }
    if (uDB.find(id)==uDB.end()) return;
    uDB.find(id).value().printHistoryTo(ui->tableWidgetHistory);
    ui->labelUserId->setText(id);
    ui->labelPoint->setText(QString::number(userDataBase().find(id).value().point));
}

void MainWindow::userLogin(){
    id="123";
    userDataBase uDB;
    if (uDB.find(id)==uDB.end()) uDB.addUser(id);
    ui->labelHint->setText(QStringLiteral("用户识别成功"));
    timer=50;
    qtimer->start(200);
    refreshView();
}

void MainWindow::throwRubbish(){
    if (id=="") return;
    userDataBase().addHistoryEntry(id,historyEntry(1,1,(int)QDateTime::currentDateTime().toTime_t()));
    timer=50;
    qtimer->start(200);
    refreshView();
    m_serialPort->write("1",1);
    m_serialPort->write("1",1);
    m_serialPort->write("1",1);
}

void MainWindow::updateTimer(){
    timer--;
    if (timer==0) {
        id="";
        ui->labelHint->setText(QStringLiteral("等待用户中..."));
        QFile switchFile("./face_recognition/switch.txt");
        switchFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream out(&switchFile);
        out<<"0";
        switchFile.close();
        QFile qrcodeSwitchFile("./barcode_recognition/switch.txt");
        qrcodeSwitchFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream qrcodeOut(&switchFile);
        qrcodeOut<<"0";
        qrcodeSwitchFile.close();
    }
    QFile qrcode("./barcode_recognition/barresult.txt");
    qrcode.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!qrcode.atEnd()){
        QTextStream in(&qrcode);
        QString msg=in.readLine();
        if (id!=""){
            userDataBase().addHistoryEntry(id,historyEntry(msg.mid(8,1).toInt()-1,(int)(msg.mid(8,1).toInt()-1==0),(int)QDateTime::currentDateTime().toTime_t()));
            m_serialPort->write("1",1);
            m_serialPort->write("1",1);
            m_serialPort->write("1",1);
            timer=50;
            qtimer->start(200);
        }
        qrcode.close();
        qrcode.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QFile switchFile("./barcode_recognition/switch.txt");
        switchFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream out(&switchFile);
        out<<"0";
        switchFile.close();
    }else if (timer<0){

    }
    qrcode.close();
    QFile face("./face_recognition/faceresult.txt");
    face.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!face.atEnd()){
        QTextStream in(&face);
        QString msg=in.readLine();
        if (msg!="Unknown"){
            id=msg;
            userDataBase uDB;
            if (uDB.find(id)==uDB.end()) uDB.addUser(id);
            ui->labelHint->setText(QStringLiteral("用户识别成功"));
            timer=50;
            qtimer->start(200);
        }
        face.close();
        face.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QFile switchFile("./face_recognition/switch.txt");
        switchFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream out(&switchFile);
        out<<"0";
        switchFile.close();
    }
    face.close();
    refreshView();
}

void MainWindow::receiveMsg(){
    QByteArray msg = m_serialPort->readAll();
    qDebug()<<msg;
    if (timer<0){
        QFile switchFile("./face_recognition/switch.txt");
        switchFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream out(&switchFile);
        out<<"1";
        switchFile.close();
        QFile qrcodeSwitchFile("barcode_recognition/switch.txt");
        qrcodeSwitchFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream qrcodeOut(&switchFile);
        qrcodeOut<<"1";
        qrcodeSwitchFile.close();
        ui->labelHint->setText(QStringLiteral("用户识别中..."));
        timer=100;
        qtimer->start(200);
    }
}
