#include "games.h"
#include "ui_games.h"
#include <QPropertyAnimation>
#include <QPixmap>
#include <QtGui>
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

    QPixmap soundIcon(":/prog_icons/icons/sound-volume.png");
    QIcon ButtonIcon(soundIcon);
    ui->soundButton->setIcon(ButtonIcon);
    ui->soundButton->setIconSize(QSize(40,40));

    ui->wordContainer_2->setText(tr("Which One: Choose the Definition"));

    connect(ui->pushButton_AnswerA, SIGNAL(clicked()), this, SLOT(on_pushButton_AnswerA_clicked()));
    connect(ui->pushButton_AnswerB, SIGNAL(clicked()), this, SLOT(on_pushButton_AnswerB_clicked()));

    gamesHere();

}

Games::~Games()
{
    delete ui;
}

int Games::randomInt(){
    //qrand() % ((high + 1) - low) + low;
    int hello = (qrand() % ((7 + 1) - 0) + 0);
    qDebug() << hello;
    return hello;
}

QString Games::wrongAnswerPicker(QStringList list, int i){ //index i is the correct answer
    int wrongIndex = i;

    while(wrongIndex == i){
        wrongIndex = randomInt();
    }

    QStringList wrongAnswer = list[wrongIndex].split(" : ");
    return wrongAnswer[1];
}

// when invoking getPairSet(language, game),,,, 2nd arg is either "definition" or "synonyms"
void Games::gamesHere(){ //getPairSet("atiman", lang_index, "to take care of self", "a-ti-man");
    QStringList liste;
    //int level = 1;

    QStringList portion;

    liste = dbqe->getWordsSet(3, 8, "dummy_arg");
    portion = liste[0].split(" : ");
    QEventLoop loop;

    portion = liste[i].split(" : ");

    ui->wordContainer->setText(portion[0]);
    ui->pushButton_AnswerA->setText(portion[1]);
    ui->pushButton_AnswerB->setText(wrongAnswerPicker(liste, i)); // gets a distractor

    //wait for user button click!!





    //get the content of buttonZ

    /*if(answer == portion[1]){ // portion[1] is the correct answer
        score++;
    }*/
    }

    qDebug() << "Your score is: " << score << "/8 Cool.";
}

void Games::on_pushButton_AnswerA_clicked(){ // A
    answer = ui->pushButton_AnswerA->text();
    qDebug() << answer;
    //return answer;
}

void Games::on_pushButton_AnswerB_clicked(){ // B
    answer = ui->pushButton_AnswerB->text();
    qDebug() << answer;
    //return answer;
}

void Games::dictionaryGame(){

}

void Games::thesaurusGame(){

}

void Games::on_soundButton_clicked()
{
    // Play the sound of the word/ image
    qDebug() << "SOUND";
}

