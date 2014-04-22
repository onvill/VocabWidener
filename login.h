#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "dbqeurier.h"
#

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

signals:
   void updateGeneralStatusSignal();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::Login *ui;
    DBQeurier *db;
};

#endif // LOGIN_H
