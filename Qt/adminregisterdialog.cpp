#include "adminregisterdialog.h"
#include "ui_adminregisterdialog.h"
#include "userdatabase.h"

adminRegisterDialog::adminRegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminRegisterDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    ui->labelWarning->setStyleSheet("color:red");
    ui->labelWarning->clear();
    ui->lineEditPassword->setEchoMode(QLineEdit::Password);
    ui->lineEditRepeatPassword->setEchoMode(QLineEdit::Password);
    connect(ui->pushButtonConfirm,&QPushButton::clicked,this,&adminRegisterDialog::confirm);
}

adminRegisterDialog::~adminRegisterDialog()
{
    delete ui;
}

void adminRegisterDialog::confirm(){
    QString password=ui->lineEditPassword->text();
    QString repeatPassword=ui->lineEditRepeatPassword->text();
    if (password=="" || repeatPassword==""){
        ui->labelWarning->setText(QStringLiteral("输入为空"));
        return;
    }
    if (password!=repeatPassword){
        ui->labelWarning->setText(QStringLiteral("密码不匹配"));
        return;
    }
    userDataBase().setAdminPassword(password);
    close();
}
