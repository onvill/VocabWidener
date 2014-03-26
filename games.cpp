#include "games.h"
#include "ui_games.h"
#include <QPropertyAnimation>
#include <QPixmap>
#include <QtGui>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QStateMachine>

Games::Games(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Games)
{
    ui->setupUi(this);

    this->setWindowTitle("Vocabulary Games");
    dbqe = DBQeurier::instance();
    timer = new QTimer(this);


    QPixmap correct(":/prog_icons/icons/accept.png");
    QPixmap wrong(":/prog_icons/icons/cross.png");
    QIcon correctIcon(correct);
    QIcon wrongIcon(wrong);


    QPixmap soundIcon(":/prog_icons/icons/sound-volume.png");
    QIcon ButtonIcon(soundIcon);
    ui->soundButton->setIcon(ButtonIcon);
    ui->soundButton->setIconSize(QSize(40,40));


    ui->wordContainer_2->setText(tr("Which One: Choose the Definition"));

    // PickGame dialog
    dictionaryGame();

}

Games::~Games()
{
    delete ui;
}

int Games::randomInt(){
    //qrand() % ((high + 1) - low) + low;
    int hello = (qrand() % (LASTQUESTION + 1));
    return hello;
}

QString Games::wrongAnswerPicker(QStringList list, int i){ //index i is the correct answer
    int wrongIndex = i;

    while(wrongIndex == i || wrongIndex == question){
        wrongIndex = randomInt();
    }

    QStringList wrongAnswer = list[wrongIndex].split(" : ");
    return wrongAnswer[1];
}

// when invoking getPairSet(language, game),,,, 2nd arg is either "definition" or "synonyms"
void Games::setUpDefinitionGame(){ //getPairSet("atiman", lang_index, "to take care of self", "a-ti-man");
    question = 0;
    score = 0;
    qDebug() << "Question: " << question << "score: " << score;
    ui->questionIndecatorLabel->setText(QString("%1 of 8").arg(question + 1));
    liste = dbqe->getWordsSet(3, 8, "dummy_arg");

    portion = liste[question].split(" : ");
    ui->wordContainer->setText(portion[0]); // the word
    ui->pushButton_AnswerA->setText(portion[1]); // the definition
    ui->pushButton_AnswerB->setText(wrongAnswerPicker(liste, 0)); // gets a distractor
}

void Games::nextQuestion(){
    question++;
    ui->questionIndecatorLabel->setText(QString("%1 of 8").arg(question +1));
    if(question <= LASTQUESTION){

        portion = liste[question].split(" : ");
        ui->wordContainer->setText(portion[0]); // the word
        ui->pushButton_AnswerA->setText(portion[1]); // the definition
        ui->pushButton_AnswerB->setText(wrongAnswerPicker(liste, 0)); // gets a distractor
    } else {
        QMessageBox::about(this, "VocabWidener",
                           QString("Game is Done. Your score is %1").arg(score));
    }

    //qDebug() << "Question: " << question << " portion[0]: " << portion[0] << " portion[1]: " << portion[1];
}

void Games::answerChecker(QString ans, int question){
    bool correct = liste[question].split(" : ").contains(ans);
    qDebug() << "The User Clicked: " << ans;
    foreach(QString str, liste[question].split(" : "))
        qDebug() << str << " ";

    if(correct == true){
        score++;
    }

    qDebug() << "Question no: " << question << " is " << correct;
}

void Games::gameFinished(){

}

void Games::on_pushButton_AnswerA_clicked(){ // A
    if(question <= LASTQUESTION ){
        answer = ui->pushButton_AnswerA->text();
        answerChecker(answer, question);
        nextQuestion();
    }
}

/*QImage myImage;
         myImage.load(":/prog_icons/icons/accept.png");
         QImage myImage2;
          myImage2.load(":/prog_icons/icons/cross.png");
        if(answerChecker(answer, question) == true){
            ui->answerIndecator->setPixmap(QPixmap::fromImage(myImage));
        } else {
            ui->answerIndecator->setPixmap(QPixmap::fromImage(myImage2));
        }*/

void Games::on_pushButton_AnswerB_clicked(){ // B
    if(question <= LASTQUESTION ){
        answer = ui->pushButton_AnswerB->text();
        answerChecker(answer, question);
        nextQuestion();
    }
}

void Games::dictionaryGame(){
    setUpDefinitionGame();
}

void Games::thesaurusGame(){
    //ui->stackedWidget->hide();
}

void Games::on_soundButton_clicked(){
    // Play the sound of the word/ image
    qDebug() << "SOUND";
}


