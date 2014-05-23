#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Sets the Vocabwidener Logo
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/prog_icons/icons/cooltext1486342101 (1).png"));
    splash->show();

    MainWindow w;

    QTimer::singleShot(100,splash,SLOT(close()));
    QTimer::singleShot(100, &w, SLOT(show()));

    return a.exec();
}
