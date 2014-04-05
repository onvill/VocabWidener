#include "games.h"
#include "ui_games.h"
#include <QPropertyAnimation>
#include <QPixmap>
#include <QtGui>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QStateMachine>

Games::Games(const QStringList& langList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Games)
{
    ui->setupUi(this);

    this->setWindowTitle("Vocabulary Games");
    dbqe = DBQeurier::instance();
    timer = new QTimer(this);

    ui->langComboBox->addItems(langList); // populate the comboBox
    ui->LangLabel->setText(tr("Language to Play"));

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
}

Games::~Games(){
    delete ui;
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
    ui->stackedWidget->setCurrentIndex(1);
    question = 0;
    score = 0;
    gamePlayed = 1;
    qDebug() << "Question: " << question << "score: " << score;
    ui->questionIndecatorLabel->setText(QString("%1 of 8").arg(question + 1));
    liste = dbqe->getWordsSet(ui->langComboBox->currentIndex() + 1, 8);

    portion = liste[question].split(" : ");
    ui->wordContainer->setText(portion[0]); // the word

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
        ui->questionIndecatorLabel->setText(QString("%1 of 8").arg(question +1));
        portion = liste[question].split(" : ");
        ui->wordContainer->setText(portion[0]); // the word

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
                       QString("Game is Done. Your score is %1\nContinue to playing this Game?").arg(score),
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        if(gamePlayed == 1){
            setUpDefinitionGame();
        }
        if(gamePlayed == 2){
            setupSynonymGame();
        }
    } else {
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

void Games::on_soundButton_clicked(){
    // Play the sound of the word/ image
    qDebug() << "SOUND";
}

/* These buttons are in the Game Menu
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
    qDebug() << "Question: " << question << "score: " << score;
    ui->questionIndecatorLabel_3->setText(QString("%1 of 8").arg(question + 1));
    liste = dbqe->getSynSet(ui->langComboBox->currentIndex() + 1, 8); // gets the Set

    foreach(QString str, liste){
        qDebug() << str;
    }

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
    //qrand() % ((high + 1) - low) + low;
    int hello = (qrand() % ((ansListLegth + 1) - 0) + 0);
    return hello;
}

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
        ui->questionIndecatorLabel_3->setText(QString("%1 of 8").arg(question +1));
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

/* Things Left for Games:
 *   - More Levels
 *   - picture and Sound for thte dictionary game
*/
