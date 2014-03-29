#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("SignIn");

    db = DBQeurier::instance();

    ui->label_2->setText(tr("Username"));
    ui->label_3->setText(tr("Password"));
    ui->pushButton_login->setText(tr("Login"));
    ui->pushButton_login->setText(tr("Cancel"));
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_login_clicked(){
    QString username, password;
    username = ui->lineEdit_userName->text();
    password = ui->lineEdit_password->text();

    if(db->login(username, password) == 1){
        ui->label->setText("Login Credentials correct.");
    } else {
        ui->label->setText("Username or Password incorrect.");
    }

}

void Login::on_pushButton_cancel_clicked(){
    this->close();
}
