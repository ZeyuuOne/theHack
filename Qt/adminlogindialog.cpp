#include "adminlogindialog.h"
#include "ui_adminlogindialog.h"
#include "userdatabase.h"
#include "adminwindow.h"

adminLoginDialog::adminLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminLoginDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    ui->labelWarning->setStyleSheet("color:red");
    ui->labelWarning->clear();
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    connect(ui->pushButtonConfirm,&QPushButton::clicked,this,&adminLoginDialog::confirm);
    connect(ui->pushButtonCancel,&QPushButton::clicked,this,&QWidget::close);
}

adminLoginDialog::~adminLoginDialog()
{
    delete ui;
}

void adminLoginDialog::confirm(){
    QString password=ui->lineEditPassword->text();
    if (password==""){
        ui->labelWarning->setText(QStringLiteral("输入为空"));
        return;
    }
    if (!userDataBase().adminPasswordIs(password)){
        ui->labelWarning->setText(QStringLiteral("密码错误"));
        return;
    }
    adminWindow* aW=new adminWindow(parentWidget());
    aW->show();
    close();
}
