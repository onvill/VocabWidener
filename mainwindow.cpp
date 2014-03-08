#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbqeurier.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QTabWidget>
#include <QAction>
#include <QPushbutton>
#include <QIcon>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stringListModel = new QStringListModel(this);
    QStringList languageList;
    languageList << "Irish" << "Spanish" << "Cebuano";

    stringListModel->setStringList(languageList);
    ui->comboBox->setModel(stringListModel);

    dbqe = new DBQeurier();
    /*db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("LOCALHOST");
    db.setDatabaseName("FinalProjectDB");
    if(!db.open()){
        qDebug() << "ERROR = " << db.lastError().text();
        QApplication::quit();
    }*/

    ui->lineEditL->setText("Search");

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_actionAbout_triggered(){
    QMessageBox::about(this, "VocabWidener",
                       "This program will help you to enrich your Vocabulary. The Dictionary tab allows you to search definitions of words. The Thesaurus finds synonyms of a word you enter. The Games tests your vocabulary.");
}

void MainWindow::on_actionSpanish_triggered(){
    ui->statusBar->showMessage("Interface language Changed to Spanish",2000);
}

void MainWindow::on_actionCebuano_triggered(){
    ui->statusBar->showMessage("Interface language Changed to Cebuano",2000);
}

void MainWindow::on_actionIrish_triggered(){
    ui->statusBar->showMessage("Interface language Changed to Irish",2000);
}

void MainWindow::on_actionExit_triggered(){
    QApplication::quit();   // EXIT
}

void MainWindow::on_actionNew_Language_triggered(){ // New Language
    // Dialog asking for Info of new language. Name,
    //ui->comboBox->addItem("Spanish"); // to add language in comboBox
}

void MainWindow::on_dictionary_Button_clicked(){ // DICTIONARY
    qDebug() << "The current Language is: ";
    ui->dictionary_Button->setStyleSheet("background-color: gray");
}

void MainWindow::on_games_Button_clicked(){ // GAMES
    ui->games_Button->setStyleSheet("background-color: gray");
}

void MainWindow::on_thesaurus_Button_clicked(){ //THESAURUS
    ui->thesaurus_Button->setStyleSheet("background-color: gray");
}

void MainWindow::on_lookUpButton_clicked(){ // Lookup
    QString data =  ui->comboBox->currentText();
    //qDebug() << "Language Chosen is: " << data;
    qDebug() << dbqe->getDefinition(data, "ansin");
    //dbqe->addEntry("cebuano","test", "teststs");
}
