#-------------------------------------------------
#
# Project created by QtCreator 2014-02-12T17:40:33
#
#-------------------------------------------------

QT += core gui\
            widgets
QT += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia


TARGET = VocabWidener
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbqeurier.cpp \
    games.cpp \
    login.cpp \
    addword.cpp \
    newlanguage.cpp \
    updatedefinition.cpp \
    addsynonym.cpp \
    fileexplorer.cpp

TRANSLATIONS += spa_translatione.ts gle_translatione.ts ceb_translatione.ts eng_translatione.ts

HEADERS  += mainwindow.h \
    dbqeurier.h \
    games.h \
    login.h \
    addword.h \
    newlanguage.h \
    updatedefinition.h \
    addsynonym.h \
    fileexplorer.h

FORMS    += mainwindow.ui \
    games.ui \
    login.ui \
    addword.ui \
    newlanguage.ui \
    updatedefinition.ui \
    addsynonym.ui \
    fileexplorer.ui

RC_FILE = myapp.rc

RESOURCES += \
    languagesIcons.qrc \
    icons.qrc \
    translation_files.qrc


