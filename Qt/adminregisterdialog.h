#ifndef ADMINREGISTERDIALOG_H
#define ADMINREGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class adminRegisterDialog;
}

class adminRegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit adminRegisterDialog(QWidget *parent = nullptr);
    ~adminRegisterDialog();

public slots:
    void confirm();

private:
    Ui::adminRegisterDialog *ui;
};

#endif // ADMINREGISTERDIALOG_H
