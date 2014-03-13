#include "games.h"
#include "ui_games.h"
#include <QPropertyAnimation>

Games::Games(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Games)
{
    ui->setupUi(this);

    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(10000);
    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));

    animation->start();
}

Games::~Games()
{
    delete ui;
}
