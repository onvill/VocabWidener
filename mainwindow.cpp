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
#include "login.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->move(250,50);
    this->setWindowTitle("VocabWidener");
    // default UI language at start up is English
    translator.load("eng_translatione");

    //status bar
    statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statusLabel, 20);

    qApp->installTranslator(&translator);
    //retranslate();

    dbqe = DBQeurier::instance();

    buttonClicked = 1;
    teacherMode = false;
    ui->dictionary_Button->setStyleSheet("background-color: white");
    ui->games_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");

    stringListModel = new QStringListModel(this);
    languageList = dbqe->getLangList();
    stringListModel->setStringList(languageList);
    ui->comboBox->setModel(stringListModel);

    QShortcut *returnShortcut = new QShortcut(QKeySequence("Return"), this);
    QObject::connect(returnShortcut, SIGNAL(activated()), ui->lookUpButton, SLOT(click()));

    /*    ---   LABELS   ---   */
    QFont f("MS Shell Dlg 2", 13, QFont::Bold);
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
    ui->newLang_button->setEnabled(false);
    ui->addWord_button->setEnabled(false);
    ui->updateDef_button->setEnabled(false);
    ui->addSyn_button->setEnabled(false);
    ui->offTeacherMode->setEnabled(false);

    /*ui->newLang_button->setEnabled(true);
    ui->addWord_button->setEnabled(true);
    ui->updateDef_button->setEnabled(true);
    ui->addSyn_button->setEnabled(true);*/
    QPixmap soundIcon(":/prog_icons/icons/sound-volume.png");
    QIcon ButtonIcon(soundIcon);
    ui->soundButton->setIcon(ButtonIcon);
    ui->soundButton->setIconSize(QSize(50,50));

    /*    ---   SHORCUTS   ---   */
    ui->actionTeacher_Login->setShortcut(tr("Ctrl+L"));
    ui->actionExit->setShortcut(tr("Ctrl+Q"));
    ui->actionAbout->setShortcut(tr("Ctrl+H"));

    newLangShortcut = new QShortcut(QKeySequence("Ctrl+Shift+N"), this);
    addWordShortcut = new QShortcut(QKeySequence("Ctrl+Shift+A"), this);
    updateDefShortcut = new QShortcut(QKeySequence("Ctrl+Shift+U"), this);
    addSynShortcut = new QShortcut(QKeySequence("Ctrl+Shift+S"), this);
    QShortcut *enter = new QShortcut(QKeySequence("Enter"), this);
    QObject::connect(enter, SIGNAL(activated()), this, SLOT(on_lookUpButton_clicked()));

    QShortcut *dict = new QShortcut(QKeySequence("Ctrl+D"), this);
    QObject::connect(dict, SIGNAL(activated()), this, SLOT(on_dictionary_Button_clicked()));
    QShortcut *thesau = new QShortcut(QKeySequence("Ctrl+T"), this);
    QObject::connect(thesau, SIGNAL(activated()), this, SLOT(on_thesaurus_Button_clicked()));
    QShortcut *gamess = new QShortcut(QKeySequence("Ctrl+G"), this);
    QObject::connect(gamess, SIGNAL(activated()), this, SLOT(on_games_Button_clicked()));
}

MainWindow::~MainWindow(){
    delete ui;
}

/* DICTIONARY mode
*/
void MainWindow::on_dictionary_Button_clicked(){
    buttonClicked = 1;
    ui->dictionary_Button->setStyleSheet("background-color: white");
    ui->games_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");
    ui->textEdit->clear();
}

/* THESAURUS mode
*/
void MainWindow::on_thesaurus_Button_clicked(){
    buttonClicked = 2;
    ui->thesaurus_Button->setStyleSheet("background-color: white");
    ui->dictionary_Button->setStyleSheet("background-color: gray");
    ui->games_Button->setStyleSheet("background-color: gray");
    ui->textEdit->clear();
}

/* Starts the GAMES section
*/
void MainWindow::on_games_Button_clicked(){
    qDebug() << "GAMESSS";
    buttonClicked = 3;
    ui->games_Button->setStyleSheet("background-color: white");
    ui->dictionary_Button->setStyleSheet("background-color: gray");
    ui->thesaurus_Button->setStyleSheet("background-color: gray");

    Games game(languageList, this);
    game.move(250, 50);
    game.setModal(true);
    game.exec();
    on_dictionary_Button_clicked();
}

/* This is the slot for the "Search" button. var lang_index determines
 * if its in Dictionary or Thesaurus mode. A word is to be entered in the
 * lineEdit. with the mode, language chosen and word the correct query is
 * made to the database.
*/
void MainWindow::on_lookUpButton_clicked(){ // Lookup
    ui->textEdit->clear();
    int lang_index = ui->comboBox->currentIndex() + 1;
    QString word = ui->lineEditL->text();
    QString textToShow;
    QStringList defAndPron;

    if(buttonClicked == 1){// Dictionary
        defAndPron = dbqe->getDictOutput(lang_index, word);
        textToShow = QString("Word: \t%1 \n\nDefinition: \n\t%2 \n\nPronunciation: \n\t%3").arg(word).arg(defAndPron[0]).arg(defAndPron[1]);
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
    wordSound = word;
}

/* This slot if signaled pops the About MessageBox, displaying text what
 * the application is all about.
*/
void MainWindow::on_actionAbout_triggered(){
    QMessageBox::about(this, "About VocabWidener",
               tr("This program will help you to enrich your Vocabulary."
                  "The Dictionary tab allows you to search definitions of"
                  "words. The Thesaurus finds synonyms of a word you enter."
                  "The Games tests your vocabulary. \t" ) +
                  " <a href='http://blogs.computing.dcu.ie/wordpress/villavo2'>VocabWidener Blog</a>");
}

/* This slot if signaled closes the application. Signaled by clicking the
 * quit in the menu bar or pressing the accelerator Ctrl+Q
*/
void MainWindow::on_actionExit_triggered(){
    QApplication::quit();   // EXIT
}

/* Functions related to changing the language of the UI
*/
void MainWindow::changeEvent(QEvent* event){
   if (event->type() == QEvent::LanguageChange){
       ui->retranslateUi(this);
       retranslate();
   }

   QMainWindow::changeEvent(event); // remember to call base class implementation
}

/* Changes the language of the UI into Spanish
*/
void MainWindow::on_actionSpanish_triggered(){
    translator.load("spa_translatione");
    qApp->installTranslator(&translator);
}

/* Changes the language of the UI into Cebuano
*/
void MainWindow::on_actionCebuano_triggered(){
    translator.load("ceb_translatione");
    qApp->installTranslator(&translator);
}

/* Changes the language of the UI into Irish
*/
void MainWindow::on_actionIrish_triggered(){
    translator.load("gle_translatione");
    qApp->installTranslator(&translator);
}

/* Changes the language of the UI into English
*/
void MainWindow::on_actionEnglish_triggered(){
    translator.load("eng_translatione");
    qApp->installTranslator(&translator);
}

/* *repaints the Ui with the new language
*/
void MainWindow::retranslate(){
    ui->teacherOnly->setText(tr("Teacher\nSection"));
    if(teacherMode){
        ui->teacherModeLabel_2->setText(tr("Teacher Mode"));
    }
}

/* This slot if signaled pops the login window, where the use enters a username
 * and password. If login succesful logInSuccess is invoked.
*/
void MainWindow::on_actionTeacher_Login_triggered(){
    Login login(this); //sarg 123
    login.setModal(true);
    connect(&login, SIGNAL(updateGeneralStatusSignal()), SLOT(logInSuccess()));
    login.exec();
}

/* The next 7 following slots displays the appropriate text on the status bar */
void MainWindow::on_actionSpanish_hovered(){
    statusLabel->setText(tr("Changes the Interface language into Spanish"));
    delay();
}

void MainWindow::on_actionCebuano_hovered(){
    statusLabel->setText(tr("Changes the Interface language into Cebuano"));
    delay();
}

void MainWindow::on_actionIrish_hovered(){
    statusLabel->setText(tr("Changes the Interface language into Irish"));
    delay();
}

void MainWindow::on_actionEnglish_hovered(){
    statusLabel->setText(tr("Changes the Interface language into English"));
    delay();
}

void MainWindow::on_actionAbout_hovered(){
    statusLabel->setText(tr("Displays Information about the application."));
    delay();
}

void MainWindow::on_actionTeacher_Login_hovered(){
    statusLabel->setText(tr("Opens a Login window to enter Teacher Mode."));
    delay();
}

void MainWindow::on_actionExit_hovered(){
    statusLabel->setText(tr("Quits the application."));
    delay();
}

/* a function with two second timer and clears the status bar
*/
void MainWindow::delay(){
    QTime dieTime= QTime::currentTime().addSecs(2);
    while( QTime::currentTime() < dieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 2000);
    }
    statusLabel->clear();
}

/* This slot if signaled disables the buttons that are only available in
 * teacher mode. This button therefore turns off teacher mode.
*/
void MainWindow::on_offTeacherMode_clicked(){
    ui->teacherModeLabel_2->setHidden(true);
    teacherMode = false;
    ui->newLang_button->setEnabled(false);
    ui->addWord_button->setEnabled(false);
    ui->updateDef_button->setEnabled(false);
    ui->addSyn_button->setEnabled(false);
    ui->offTeacherMode->setEnabled(false);

    QObject::disconnect(newLangShortcut, SIGNAL(activated()), this, SLOT(on_newLang_button_clicked()));
    QObject::disconnect(addWordShortcut, SIGNAL(activated()), this, SLOT(on_addWord_button_clicked()));
    QObject::disconnect(updateDefShortcut, SIGNAL(activated()), this, SLOT(on_updateDef_button_clicked()));
    QObject::disconnect(addSynShortcut, SIGNAL(activated()), this, SLOT(on_addSyn_button_clicked()));
}

/* Teacher mode is turned on if this is invoked. showing the red Teacher mode text
*/
void MainWindow::logInSuccess(){
    ui->teacherModeLabel_2->setHidden(false);
    teacherMode = true;
    ui->teacherModeLabel_2->setText(tr("Teacher Mode"));

    ui->newLang_button->setEnabled(true);
    ui->addWord_button->setEnabled(true);
    ui->updateDef_button->setEnabled(true);
    ui->addSyn_button->setEnabled(true);
    ui->offTeacherMode->setEnabled(true);

    QObject::connect(newLangShortcut, SIGNAL(activated()), this, SLOT(on_newLang_button_clicked()));
    QObject::connect(addWordShortcut, SIGNAL(activated()), this, SLOT(on_addWord_button_clicked()));
    QObject::connect(updateDefShortcut, SIGNAL(activated()), this, SLOT(on_updateDef_button_clicked()));
    QObject::connect(addSynShortcut, SIGNAL(activated()), this, SLOT(on_addSyn_button_clicked()));

}

/* This slot if signaled
*/
void MainWindow::on_newLang_button_clicked(){
    NewLanguage newLang(languageList, this);
    newLang.setModal(true);
    connect(&newLang, SIGNAL(newLanguage(QString)), SLOT(updateLangList(QString)));
    newLang.exec();

}

/* Update languageList which in turn is displayed in the comboBox
*/
void MainWindow::updateLangList(QString lang){
    languageList << lang;
    stringListModel->setStringList(languageList);
    ui->comboBox->setModel(stringListModel);

    qDebug() <<  lang << " ADDEDD";
}

/* This slot if signaled pops the window where the teacher could enter a new
 * word into the system.
*/
void MainWindow::on_addWord_button_clicked(){
    AddWord newWord(languageList, this);
    newWord.setModal(true);
    newWord.exec();
}

/* This slot if signaled pops the window where the teacher could update the
 * definition a word the system.
*/
void MainWindow::on_updateDef_button_clicked(){
    UpdateDefinition updateDef(languageList, this);
    updateDef.setModal(true);
    updateDef.exec();
}

/* This slot if signaled pops the dialog where the teacher can associate word to
 * another to become its synonym.
*/
void MainWindow::on_addSyn_button_clicked(){
    AddSynonym addSyn(this);
    addSyn.setModal(true);
    addSyn.exec();
}

/* This slot is signaled by clicking the audio image. Plays an audio of the word
 * entered in the lindeEdit.
*/
void MainWindow::on_soundButton_clicked(){
    QByteArray array = dbqe->getSoundOrPicBytes(wordSound, "sound");
    QFile file(QString("sounds/%1.wav").arg(wordSound));
    file.open(QIODevice::WriteOnly);
    file.write(array);
    file.close();

    // Play the wav file
    QSound::play(QString("sounds/%1.wav").arg(wordSound));
}
