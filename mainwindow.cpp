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
#include "games.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buttonClicked = 1;
    ui->dictionary_Button->setStyleSheet("background-color: white");
    ui->games_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");

    stringListModel = new QStringListModel(this);
    QStringList languageList;
    languageList << "Irish" << "Spanish" << "Cebuano";

    stringListModel->setStringList(languageList);
    ui->comboBox->setModel(stringListModel);

    dbqe = new DBQeurier();

    ui->lineEditL->setText("Search");
    //connect(ui->lookUpButton,SIGNAL(clicked()), this,SLOT(on_lookUpButton_clicked()));


}

MainWindow::~MainWindow(){
    delete ui;
}

//      DICTIONARY
void MainWindow::on_dictionary_Button_clicked(){
    qDebug() << "DICTIONARY";
    buttonClicked = 1;
    ui->dictionary_Button->setStyleSheet("background-color: white");
    ui->games_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");
}

//      THESAURUS
void MainWindow::on_thesaurus_Button_clicked(){
    buttonClicked = 2;
    ui->thesaurus_Button->setStyleSheet("background-color: white");
    ui->dictionary_Button->setStyleSheet("background-color: gray");
    ui->games_Button->setStyleSheet("background-color: gray");
    qDebug() << "THESAURUS";
}

//      GAMES GAMES GAMES
void MainWindow::on_games_Button_clicked(){
    qDebug() << "GAMESSS";
    buttonClicked = 3;
    ui->games_Button->setStyleSheet("background-color: white");
    ui->dictionary_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");

    /*Games game;
    game.setModal(true);
    game.exec();*/
}

void MainWindow::on_lookUpButton_clicked(){ // Lookup
    ui->textEdit->clear();
    QString chosenLanguage = ui->comboBox->currentText();
    QString word = ui->lineEditL->text();
    QString definition, textToShow;
    QStringList synonyms;


    if(buttonClicked == 1){// Dictionary
        definition = dbqe->getDefinition(chosenLanguage, word);
        textToShow = QString("Word: %1 \n\n Definition: %2").arg(word).arg(definition);
        ui->textEdit->append(textToShow);
    }

    if(buttonClicked == 2){ // Thesaurus
        textToShow = QString("Word: %1 \n\n").arg(word);
        ui->textEdit->append(textToShow);
        textToShow = QString("Synonyms: ");
        ui->textEdit->append(textToShow);

        synonyms = dbqe->getSynonyms(chosenLanguage, word);
        textToShow = "";
        foreach(QString str, synonyms){
            textToShow = QString(textToShow + "%1 \n").arg(str);
        }

        ui->textEdit->append(textToShow);
    }

    //dbqe->addEntry("cebuano","test", "tests");
    //dbqe->updateDefinition("cebuano","itom", "black");
}

void MainWindow::on_actionNew_Language_triggered(){ // New Language
    // Dialog asking for Info of new language. Name,
    qDebug() << dbqe->addNewLanguage("Nawat");
    //ui->comboBox->addItem("Spanish"); // to add language in comboBox
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
