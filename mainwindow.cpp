#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_AddWord.h"
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
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->move(220,90);
    this->setWindowTitle("VocabWidener");
    translator.load("eng_translatione");
    qApp->installTranslator(&translator);
    //retranslate();

    dbqe = DBQeurier::instance();

    //this->setStyleSheet("background-color: green");
    buttonClicked = 1;
    teacherMode = false;
    ui->dictionary_Button->setStyleSheet("background-color: white");
    ui->games_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");

    stringListModel = new QStringListModel(this);
    languageList << "Spanish" << "Irish"  << "Cebuano";

    stringListModel->setStringList(languageList);
    ui->comboBox->setModel(stringListModel);


    QShortcut *returnShortcut = new QShortcut(QKeySequence("Return"), this);
    QObject::connect(returnShortcut, SIGNAL(activated()), ui->lookUpButton, SLOT(click()));


    /*    ---   LABELS   ---   */
    QFont f( "MS Shell Dlg 2", 10, QFont::Bold);
    ui->teacherOnly->setText(tr("Teacher Section"));
    ui->teacherModeLabel_2->setStyleSheet("QLabel {color : Red; font:bold 16px;}");
    ui->lineEditL->setText(tr("Search"));
    ui->textEdit->setFont(f);
    ui->teacherOnly->setFont(f);

    /*    ---   BUTTONS   ---   */
    ui->newLang_button->setText(tr("New Language"));
    ui->addWord_button->setText(tr("Add Word"));
    ui->updateDef_button->setText(tr("Update Definition"));
    ui->addSyn_button->setText(tr("New Synonym"));
    ui->offTeacherMode->setText(tr("Stop Mode"));
    /*ui->newLang_button->setEnabled(false);
    ui->addWord_button->setEnabled(false);
    ui->updateDef_button->setEnabled(false);
    ui->addSyn_button->setEnabled(false);*/
    ui->offTeacherMode->setEnabled(false);

    ui->newLang_button->setEnabled(true);
    ui->addWord_button->setEnabled(true);
    ui->updateDef_button->setEnabled(true);
    ui->addSyn_button->setEnabled(true);

    /*    ---   SHORCUTS   ---   */
    ui->actionTeacher_Login->setShortcut(tr("Ctrl+L"));
    ui->actionExit->setShortcut(tr("Ctrl+X"));

    //ui->toolBox->setEnabled(false);
}

MainWindow::~MainWindow(){
    delete ui;
}

//      DICTIONARY
void MainWindow::on_dictionary_Button_clicked(){
    qDebug() << "DICTIONARY";
    //int lang_index = ui->comboBox->currentIndex() + 1;
    //dbqe->updateDefinition(lang_index, "ako", "me");

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
    //game.move(220, 90);
    game.setModal(true);
    game.exec();
    on_dictionary_Button_clicked();
}

void MainWindow::on_lookUpButton_clicked(){ // Lookup
    ui->textEdit->clear();
    int lang_index = ui->comboBox->currentIndex() + 1;
    QString word = ui->lineEditL->text();
    QString definition, textToShow;

    if(buttonClicked == 1){// Dictionary
        definition = dbqe->getDefinition(lang_index, word);
        textToShow = QString("Word: \t%1 \n\nDefinition: \n\t%2").arg(word).arg(definition);
        ui->textEdit->append(textToShow);
    }

    if(buttonClicked == 2){ // Thesaurus
        textToShow = QString("Word: \t%1 \n").arg(word);
        ui->textEdit->append(textToShow);
        textToShow = QString("Synonyms: ");
        ui->textEdit->append(textToShow);

        QStringList synonyms = dbqe->getSynonyms(lang_index, word);
        textToShow = "";
        foreach(QString str, synonyms){
            textToShow = QString(textToShow + "\t%1 \n").arg(str);
        }

        ui->textEdit->append(textToShow);
        synonyms.clear();
    }

    //dbqe->addEntry("atiman", lang_index, "to take care of self", "a-ti-man");
    //dbqe->updateDefinition("cebuano","itom", "black");
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

void MainWindow::on_actionEnglish_triggered(){
    ui->statusBar->showMessage(tr("Interface language Changed to English"),2000);
    translator.load("eng_translatione");
    qApp->installTranslator(&translator);
}

void MainWindow::retranslate(){
    ui->teacherOnly->setText(tr("Teacher\nSection"));
    if(teacherMode){
        ui->teacherModeLabel_2->setText(tr("Teacher Mode"));
    }
}

// QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/path/to/file/",tr("Mp3 Files (*.mp3)"));
/*myImage = new QLabel();

// If your image is inside "images" folder then try  ":/images/imgfromresource.jpg".

    QImage image(":/imgfromresource.jpg");
    if(image.isNull()){
        // error loading image, show an error message here
    }
    myImage->setPixmap(QPixmap::fromImage(image));

*/

void MainWindow::on_actionTeacher_Login_triggered(){
    /*Login *log = new Login(this);
    log->show();*/
    Login login(this); //sarg 123
    login.setModal(true);
    connect(&login, SIGNAL(updateGeneralStatusSignal()), SLOT(logInSuccess()));
    login.exec();
}

void MainWindow::on_offTeacherMode_clicked(){
    ui->teacherModeLabel_2->setHidden(true);
    teacherMode = false;
    //ui->teacherOnly->setText(tr("Teacher\nSection"));
    ui->newLang_button->setEnabled(false);
    ui->addWord_button->setEnabled(false);
    ui->updateDef_button->setEnabled(false);
    ui->addSyn_button->setEnabled(false);
    ui->offTeacherMode->setEnabled(false);
}

void MainWindow::logInSuccess(){
    ui->teacherModeLabel_2->setHidden(false);
    teacherMode = true;
    ui->teacherModeLabel_2->setText(tr("Teacher Mode"));

    ui->newLang_button->setEnabled(true);
    ui->addWord_button->setEnabled(true);
    ui->updateDef_button->setEnabled(true);
    ui->addSyn_button->setEnabled(true);
    ui->offTeacherMode->setEnabled(true);
}

void MainWindow::on_newLang_button_clicked(){
    //dbqe->newlanguage(4, languageName, code);
    /*AddWord *newWord = new AddWord();
    newWord.show();*/

}

void MainWindow::on_addWord_button_clicked(){
    //dbqe->addEntry(word, id, definition, pronunciation);

}

void MainWindow::on_updateDef_button_clicked(){
    //dbqe->getDefinition(id, word, new_def);
}

void MainWindow::on_addSyn_button_clicked(){
    //dbqe->associateWord(word, wordToAdd);
}
