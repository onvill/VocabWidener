#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

/*#ifndef QT_NO_TRANSLATION
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);
#endif*/
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/prog_icons/icons/cooltext1486342101 (1).png"));
    splash->show();

    MainWindow w;

    QTimer::singleShot(100,splash,SLOT(close()));
    QTimer::singleShot(100, &w, SLOT(show()));

    //w.show();

    return a.exec();
}
