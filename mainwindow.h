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
#include "addword.h"
#include "newlanguage.h"
#include "updatedefinition.h"
#include "addsynonym.h"
#include <QShortcut>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

/* Slots are functions that are invoked if a signal is emitted. Eg. if a button
 * is clicked, that emits a signal and the slot that its associated with is invoked.
*/
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
    void on_soundButton_clicked();
    void updateLangList(QString lang);
    void on_actionSpanish_hovered();
    void on_actionCebuano_hovered();
    void on_actionAbout_hovered();
    void on_actionExit_hovered();
    void on_actionIrish_hovered();
    void on_actionEnglish_hovered();
    void on_actionTeacher_Login_hovered();

protected slots:
    void changeEvent(QEvent* event);

private:
    int buttonClicked;
    QStringList languageList;
    bool teacherMode;
    QString wordSound;
    QTranslator translator;
    QLabel *statusLabel;

    Ui::MainWindow *ui;
    QSqlTableModel *sqlTableModel;
    QStringListModel *stringListModel;
    DBQeurier *dbqe;
    QTranslator m_translator;
    AddWord *newWord;
    AddSynonym *addSyn;
    NewLanguage *newLang;
    UpdateDefinition *updateDef;
    QShortcut *newLangShortcut;
    QShortcut *addWordShortcut;
    QShortcut *updateDefShortcut;
    QShortcut *addSynShortcut;

    void retranslate();
    void delay();
};

#endif // MAINWINDOW_H
