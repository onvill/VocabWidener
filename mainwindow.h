#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QTabWidget>
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QTranslator>
#include "dbqeurier.h"
#include <QLabel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();
    void on_actionSpanish_triggered();
    void on_actionCebuano_triggered();
    void on_actionIrish_triggered();
    void on_actionExit_triggered();
    void on_dictionary_Button_clicked();
    void on_games_Button_clicked();
    void on_thesaurus_Button_clicked();
    void on_lookUpButton_clicked();
    void on_actionEnglish_triggered();
    void on_offTeacherMode_clicked();
    void on_actionTeacher_Login_triggered();
    void logInSuccess();

    void on_newLang_button_clicked();

    void on_addWord_button_clicked();

    void on_updateDef_button_clicked();

    void on_addSyn_button_clicked();

protected slots:
    void changeEvent(QEvent* event);

private:
    int buttonClicked;
    QStringList languageList;
    bool teacherMode;
    QTranslator translator;

    Ui::MainWindow *ui;
    QSqlTableModel *sqlTableModel;
    //QSqlDatabase db;
    QStringListModel *stringListModel;
    DBQeurier *dbqe;
    QTranslator m_translator;

    void retranslate();
};

#endif // MAINWINDOW_H
