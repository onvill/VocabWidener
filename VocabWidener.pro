#-------------------------------------------------
#
# Project created by QtCreator 2014-02-12T17:40:33
#
#-------------------------------------------------

QT += core gui\
            widgets
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VocabWidener
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbqeurier.cpp \
    games.cpp \
    login.cpp

TRANSLATIONS += spa_translatione.ts gle_translatione.ts ceb_translatione.ts

HEADERS  += mainwindow.h \
    dbqeurier.h \
    games.h \
    login.h

FORMS    += mainwindow.ui \
    games.ui \
    login.ui

RC_FILE = myapp.rc

RESOURCES += \
    languagesIcons.qrc \
    icons.qrc \
    translation_files.qrc
