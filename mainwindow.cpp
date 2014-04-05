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
#include <QPalette>
#include "login.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("VocabWidener");
    translator.load("spa_translatione");
    qApp->installTranslator(&translator);
    retranslate();

    //this->setStyleSheet("background-color: green");
    buttonClicked = 1;
    ui->dictionary_Button->setStyleSheet("background-color: white");
    ui->games_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");

    stringListModel = new QStringListModel(this);
    languageList << "Spanish" << "Irish"  << "Cebuano";

    stringListModel->setStringList(languageList);
    ui->comboBox->setModel(stringListModel);

    dbqe = DBQeurier::instance();

    ui->lineEditL->setText("Search");
    //connect(ui->lookUpButton,SIGNAL(clicked()), this,SLOT(on_lookUpButton_clicked()));
    //ui->frame->setStyleSheet("border: 5px solid black" );
    /*Login *log = new Login(this);
    log->show();*/
}

MainWindow::~MainWindow(){
    delete ui;
}

//      DICTIONARY
void MainWindow::on_dictionary_Button_clicked(){
    qDebug() << "DICTIONARY";
    int lang_index = ui->comboBox->currentIndex() + 1;
    //dbqe->updateDefinition(lang_index, "ako", "me");

    buttonClicked = 1;
    ui->dictionary_Button->setStyleSheet("background-color: white");
    ui->games_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");
    Login *log = new Login(this);
    log->show();
}

//      THESAURUS
void MainWindow::on_thesaurus_Button_clicked(){
    buttonClicked = 2;
    ui->thesaurus_Button->setStyleSheet("background-color: white");
    ui->dictionary_Button->setStyleSheet("background-color: gray");
    ui->games_Button->setStyleSheet("background-color: gray");
    qDebug() << "THESAURUS";
    //dbqe->associateWord("1234","dark");

}

//      GAMES GAMES GAMES
void MainWindow::on_games_Button_clicked(){
    qDebug() << "GAMESSS";
    buttonClicked = 3;
    ui->games_Button->setStyleSheet("background-color: white");
    ui->dictionary_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");

    Games game(languageList, this);
    game.setModal(true);
    game.exec();
}

void MainWindow::on_lookUpButton_clicked(){ // Lookup
    ui->textEdit->clear();
    int lang_index = ui->comboBox->currentIndex() + 1;
    QString word = ui->lineEditL->text();
    QString definition, textToShow;

    if(buttonClicked == 1){// Dictionary
        definition = dbqe->getDefinition(lang_index, word);
        textToShow = QString("Word: %1 \n\n Definition: %2").arg(word).arg(definition);
        ui->textEdit->append(textToShow);
    }

    if(buttonClicked == 2){ // Thesaurus
        textToShow = QString("Word: %1 \n\n").arg(word);
        ui->textEdit->append(textToShow);
        textToShow = QString("Synonyms: ");
        ui->textEdit->append(textToShow);

        QStringList synonyms = dbqe->getSynonyms(lang_index, word);
        textToShow = "";
        foreach(QString str, synonyms){
            textToShow = QString(textToShow + "%1 \n").arg(str);
        }

        ui->textEdit->append(textToShow);
        synonyms.clear();
    }

    //dbqe->addEntry("atiman", lang_index, "to take care of self", "a-ti-man");
    //dbqe->updateDefinition("cebuano","itom", "black");
}

void MainWindow::on_actionNew_Language_triggered(){ // New Language
    // Dialog asking for Info of new language. Name,
    qDebug() << dbqe->addNewLanguage(ui->comboBox->count()+1, "Nawat", "na");
    //ui->comboBox->addItem("Spanish"); // to add language in comboBox
}

void MainWindow::on_actionAbout_triggered(){
    QMessageBox::about(this, "VocabWidener",
                       tr("This program will help you to enrich your Vocabulary. The Dictionary tab allows you to search definitions of words. The Thesaurus finds synonyms of a word you enter. The Games tests your vocabulary."));
}

void MainWindow::on_actionExit_triggered(){
    QApplication::quit();   // EXIT
}


/*
 * Functions related to changing the language of the UI
*/
void MainWindow::changeEvent(QEvent* event){
   if (event->type() == QEvent::LanguageChange){
       ui->retranslateUi(this);
       retranslate();
   }

   QMainWindow::changeEvent(event); // remember to call base class implementation
}

void MainWindow::on_actionSpanish_triggered(){
    ui->statusBar->showMessage(tr("Interface language Changed to Spanish"),2000);
    translator.load("spa_translatione");
    qApp->installTranslator(&translator);
}

void MainWindow::on_actionCebuano_triggered(){
    ui->statusBar->showMessage(tr("Interface language Changed to Cebuano"),2000);
    translator.load("ceb_translatione");
    qApp->installTranslator(&translator);
}

void MainWindow::on_actionIrish_triggered(){
    ui->statusBar->showMessage(tr("Interface language Changed to Irish"),2000);
    translator.load("gle_translatione");
    qApp->installTranslator(&translator);
}

void MainWindow::retranslate(){
    //ui->pushButton->setText(tr("PushButton"));
}

