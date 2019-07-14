#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "userdatabase.h"
#include "QDebug"

adminWindow::adminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);
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
    ui->tableWidgetUserList->setColumnCount(2);
    ui->tableWidgetUserList->setRowCount(20);
    ui->tableWidgetUserList->verticalHeader()->setDefaultSectionSize(10);
    width = ui->tableWidgetUserList->width()-22;
    ui->tableWidgetUserList->setColumnWidth(0,(int)width*0.5);
    ui->tableWidgetUserList->setColumnWidth(1,(int)width-width*0.5);
    ui->tableWidgetUserList->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidgetUserList->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidgetUserList->setHorizontalHeaderLabels(QStringList() << QStringLiteral("用户ID") << QStringLiteral("积分"));
    ui->tableWidgetUserList->verticalHeader()->setVisible(false);
    ui->tableWidgetUserList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetUserList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetUserList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->labelUserId->clear();
    ui->labelPoint->clear();
    connect(ui->actionQuit,&QAction::triggered,this,&QWidget::close);
    connect(ui->tableWidgetUserList,&QTableWidget::cellClicked,this,&adminWindow::setId);
    refreshView();
}

adminWindow::~adminWindow()
{
    delete ui;
}

void adminWindow::refreshView(){
    ui->tableWidgetHistory->clear();
    ui->tableWidgetHistory->setHorizontalHeaderLabels(QStringList() << QStringLiteral("垃圾类型") << QStringLiteral("分类") << QStringLiteral("积分") << QStringLiteral("时间"));
    ui->tableWidgetUserList->clear();
    ui->tableWidgetUserList->setHorizontalHeaderLabels(QStringList() << QStringLiteral("用户") << QStringLiteral("积分"));
    ui->labelUserId->clear();
    ui->labelPoint->clear();
    userDataBase uDB;
    if (uDB.empty()) return;
    uDB.printUserListTo(ui->tableWidgetUserList);
    if (uDB.find(id)==uDB.end()) return;
    uDB.find(id).value().printHistoryTo(ui->tableWidgetHistory);
    ui->labelUserId->setText(id);
    ui->labelPoint->setText(QString::number(userDataBase().find(id).value().point));
}

void adminWindow::setId(int row){
    if (ui->tableWidgetUserList->item(row,0)==nullptr){
        id="";
        refreshView();
        return;
    }
    id=ui->tableWidgetUserList->item(row,0)->text();
    refreshView();
}
