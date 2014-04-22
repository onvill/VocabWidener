#include "games.h"
#include "ui_games.h"
#include <QPropertyAnimation>
#include <QPixmap>
#include <QtGui>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QStateMachine>
#include <QFile>


Games::Games(const QStringList& langList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Games)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Vocabulary Games"));
    dbqe = DBQeurier::instance();

    ui->langComboBox->addItems(langList); // populate the comboBox
    ui->LangLabel->setText(tr("Language to Play"));

    pixmap = QPixmap();
    QPixmap correct(":/prog_icons/icons/accept.png");
    QPixmap wrong(":/prog_icons/icons/cross.png");
    QIcon correctIcon(correct);
    QIcon wrongIcon(wrong);

    QPixmap soundIcon(":/prog_icons/icons/sound-volume.png");
    QIcon ButtonIcon(soundIcon);
    ui->soundButton->setIcon(ButtonIcon);
    ui->soundButton->setIconSize(QSize(40,40));

    ui->gameMenuLabel->setText(tr("Game Menu"));
    ui->StartDefGame_button->setText(tr("Word Definition"));
    ui->StartSynGame_button->setText(tr("Find the Synonym"));
    ui->quitButton->setText(tr("Quit"));
    ui->wordContainer_2->setText(tr("Which One: Choose the Definition"));
    ui->wordContainer_51->setText(tr("Which One: Choose the Synonym"));

    // PickGame dialog
    ui->stackedWidget->setCurrentIndex(0);
    level = INCREMENTLEVELBY;
    wordSound = "";

    ui->picture->setScaledContents(true);
    ui->exitSynGameButton->setText(tr("Quit"));
    ui->exitDefGameButton->setText(tr("Quit"));
    ui->answerIndecator->setText("");
    ui->answerIndecator_3->setText("");
}

Games::~Games(){
    delete ui;
}

/* These buttons(3) are in the Game Menu
*/
void Games::on_StartDefGame_button_clicked(){
    setUpDefinitionGame();
}

void Games::on_StartSynGame_button_clicked(){
    setupSynonymGame();
}

void Games::on_quitButton_clicked(){
    close();
}

int Games::randomAorB(){
    int rand = (qrand() % (2));
    return rand;
}

int Games::randomInt(){
    //qrand() % ((high + 1) - low) + low;
    int hello = (qrand() % (LASTQUESTION + 1));
    return hello;
}

// when invoking getPairSet(language, game),,,, 2nd arg is either "definition" or "synonyms"
void Games::setUpDefinitionGame(){ //getPairSet("atiman", lang_index, "to take care of self", "a-ti-man");
    ui->picture->clear();
    ui->stackedWidget->setCurrentIndex(1);
    question = 0;
    score = 0;
    gamePlayed = 1;
    ui->questionIndecatorLabel->setText(QString("%1 / 8").arg(question + 1));
    ui->progressBar_Def->setValue((question + 1) * 12.5);
    liste = dbqe->getWordsSet(ui->langComboBox->currentIndex() + 1, level);

    portion = liste[question].split(" : ");
    wordSound = portion[0];
    ui->wordContainer->setText(portion[0]); // the word

    pixmap.loadFromData(dbqe->getSoundOrPicBytes(portion[0], "image"));
    ui->picture->setPixmap(pixmap);


    if(randomAorB() == 1){
        ansOrBoggy = portion[1];
        ansOrBoggy2 = wrongAnswerPicker(liste, 0);
    } else {
        ansOrBoggy2 = portion[1];
        ansOrBoggy = wrongAnswerPicker(liste, 0);
    } // (one answer is a distractor)
    ui->pushButton_AnswerA->setText(ansOrBoggy); // button A
    ui->pushButton_AnswerB->setText(ansOrBoggy2); // button B
}

void Games::nextQuestion(){
    question++;

    if(question <= LASTQUESTION){
        ui->questionIndecatorLabel->setText(QString("%1 / 8").arg(question +1));
        ui->progressBar_Def->setValue((question + 1) * 12.5);
        portion = liste[question].split(" : ");
        wordSound = portion[0];
        ui->wordContainer->setText(portion[0]); // the word

        /*Picture*/
        ui->picture->clear();
        pixmap.loadFromData(dbqe->getSoundOrPicBytes(portion[0], "image"));
        ui->picture->setPixmap(pixmap);

        if(randomAorB() == 1){
            ansOrBoggy = portion[1];
            ansOrBoggy2 = wrongAnswerPicker(liste, 0);
        } else {
            ansOrBoggy2 = portion[1];
            ansOrBoggy = wrongAnswerPicker(liste, 0);
        } // (one answer is a distractor)
        ui->pushButton_AnswerA->setText(ansOrBoggy); // button A
        ui->pushButton_AnswerB->setText(ansOrBoggy2); // button B
    } else {
        gameFinished();
    }
}

void Games::gameFinished(){
    liste.clear();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "VocabWidener",
                       QString(tr("Game is Done. Your score is %1\nContinue playing this Game?")).arg(score),
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        level += INCREMENTLEVELBY;
        if(gamePlayed == 1){
            setUpDefinitionGame();
        }
        if(gamePlayed == 2){
            setupSynonymGame();
        }
        // show Level indicatior then disappear.
    } else {
        level = INCREMENTLEVELBY;
        ui->stackedWidget->setCurrentIndex(0);
    }

}

void Games::on_pushButton_AnswerA_clicked(){ // A
    if(question <= LASTQUESTION ){
        answer = ui->pushButton_AnswerA->text();
        answerChecker(answer, question);
        nextQuestion();
    }
}

void Games::on_pushButton_AnswerB_clicked(){ // B
    if(question <= LASTQUESTION ){
        answer = ui->pushButton_AnswerB->text();
        answerChecker(answer, question);
        nextQuestion();
    }
}

/* Play the sound of the word. The current word in question is queried
 * from the database. An array of the sound is returned, this is then
 * writen to a sound file; wav format. The sound file is then played.
*/
void Games::on_soundButton_clicked(){
    QByteArray array = dbqe->getSoundOrPicBytes(wordSound, "sound");
    QFile file(QString("sounds/%1.wav").arg(wordSound));
    file.open(QIODevice::WriteOnly);
    file.write(array);
    file.close();
    
    QSound::play(QString("sounds/%1.wav").arg(wordSound));
}

void Games::on_exitDefGameButton_clicked(){
    level = INCREMENTLEVELBY;
    ui->stackedWidget->setCurrentIndex(0);
}

/*  QImage myImage;
    myImage.load(":/prog_icons/icons/accept.png");
    QImage myImage2;
    myImage2.load(":/prog_icons/icons/cross.png");
    if(answerChecker(answer, question) == true){
        ui->answerIndecator->setPixmap(QPixmap::fromImage(myImage));
    } else {
        ui->answerIndecator->setPixmap(QPixmap::fromImage(myImage2));
    }

void Games::dictionaryGame(){
    // go to Defination game Page
    setUpDefinitionGame();
}

void Games::thesaurusGame(){
    // go to Synonym game Page
    setUpSynonymGame();
}   */

/* Choses random(not the answer to the current question) at the query set
*/
QString Games::wrongAnswerPicker(QStringList list, int i){ //index i is the correct answer
    int wrongIndex = i;

    while(wrongIndex == i || wrongIndex == question){
        wrongIndex = randomInt();
    }

    QStringList wrongAnswer = list[wrongIndex].split(" : ");
    return wrongAnswer[1];
}

void Games::answerChecker(QString ans, int question){
    bool correct = liste[question].contains(ans);// see if the contents after ":" has the answer
    qDebug() << "The User Clicked: " << ans;
    foreach(QString str, liste[question].split(" : ")) // shows the question and answer
        qDebug() << str << " ";

    if(correct == true){
        score++;
    }

    qDebug() << "Question no: " << question << " is " << correct;
}

void Games::setupSynonymGame(){ //getPairSet("atiman", lang_index, "to take care of self", "a-ti-man");
    ui->stackedWidget->setCurrentIndex(2);
    question = 0;
    score = 0;
    gamePlayed = 2;
    qDebug() << "LEVELLLLLLLLLLLLLLLLLLL: " << level;
    qDebug() << "Question: " << question << "score: " << score;
    ui->questionIndecatorLabel_3->setText(QString("%1 / 8").arg(question + 1));
    ui->progressBar_Syn->setValue((question + 1) * 12.5);
    liste = dbqe->getSynSet(ui->langComboBox->currentIndex() + 1, level); // gets the Set

    portion = liste[question].split(" : ");
    ui->wordContainer_50->setText(portion[0]); // Question

    if(randomAorB() == 1){
        ansOrBoggy = answerChoper(portion[1]);
        ansOrBoggy2 = answerChoper(wrongAnswerPicker(liste, 0));
    } else {
        ansOrBoggy2 = answerChoper(portion[1]);
        ansOrBoggy = answerChoper(wrongAnswerPicker(liste, 0));
    } // (one answer is a distractor)
    ui->pushButton_AnswerA_26->setText(ansOrBoggy); // Button A
    ui->pushButton_AnswerB_26->setText(ansOrBoggy2); // Button B
}

int Games::randomInte(int ansListLegth){
    int hello = (qrand() % ((ansListLegth + 1) - 0) + 0);
    return hello;
}

/* This function solves the issue of syn|syn2|syn3 ,
 * showing up on the buttons
*/
QString Games::answerChoper(QString ans){
    QStringList answers = ans.split("|");
    qDebug() << answers.length();
    if(answers.length() == 1){
        return answers[0];
    } else {
        return answers[randomInte(answers.length()-1)]; // returns random between the |s
    }
}

void Games::nextSynQuestion(){
    question++;

    if(question <= LASTQUESTION){
        ui->questionIndecatorLabel_3->setText(QString("%1 / 8").arg(question +1));
        ui->progressBar_Syn->setValue((question + 1) * 12.5);
        portion = liste[question].split(" : ");
        ui->wordContainer_50->setText(portion[0]); // the Question

        if(randomAorB() == 1){
            ansOrBoggy = answerChoper(portion[1]);
            ansOrBoggy2 = answerChoper(wrongAnswerPicker(liste, 0));
        } else {
            ansOrBoggy2 = answerChoper(portion[1]);
            ansOrBoggy = answerChoper(wrongAnswerPicker(liste, 0));
        } // (one answer is a distractor)
        ui->pushButton_AnswerA_26->setText(ansOrBoggy); // Button A
        ui->pushButton_AnswerB_26->setText(ansOrBoggy2); // Button B
    } else {
        gameFinished();
    }
}

void Games::on_pushButton_AnswerA_26_clicked(){ // Synonym Answer button A
    if(question <= LASTQUESTION ){
        answer = ui->pushButton_AnswerA_26->text();
        answerChecker(answer, question);
        nextSynQuestion();
    }
}

void Games::on_pushButton_AnswerB_26_clicked(){ // Synonym Answer button B
    if(question <= LASTQUESTION ){
        answer = ui->pushButton_AnswerB_26->text();
        answerChecker(answer, question);
        nextSynQuestion();
    }
}

void Games::on_exitSynGameButton_clicked(){
    level = INCREMENTLEVELBY;
    ui->stackedWidget->setCurrentIndex(0);
}
