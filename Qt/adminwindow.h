#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

namespace Ui {
class adminWindow;
}

class adminWindow : public QMainWindow
{
    Q_OBJECT

    QString id;

public:
    explicit adminWindow(QWidget *parent = nullptr);
    ~adminWindow();

public slots:
    void refreshView();
    void setId(int row);

private:
    Ui::adminWindow *ui;
};

#endif // ADMINWINDOW_H
