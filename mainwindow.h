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

    void on_actionNew_Language_triggered();

    void on_dictionary_Button_clicked();

    void on_games_Button_clicked();

    void on_thesaurus_Button_clicked();

    void on_lookUpButton_clicked();


private:
    Ui::MainWindow *ui;
    QSqlTableModel *sqlTableModel;
    //QSqlDatabase db;
    QStringListModel *stringListModel;
    DBQeurier *dbqe;
    int buttonClicked;

};

#endif // MAINWINDOW_H
