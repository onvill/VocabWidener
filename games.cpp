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
    QPixmap correct(":/prog_icons/icons/correct-tick-hi.png");
    QPixmap wrong(":/prog_icons/icons/X-400.png");
    QIcon correctIcon(correct);
    QIcon wrongIcon(wrong);
    correctMark.load(":/prog_icons/icons/correct-tick-hi.png");
    wrongMark.load(":/prog_icons/icons/X-400.png");

    /*    ---   BUTTONS   ---   */
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

    ui->picture->setScaledContents(true);
    ui->answerIndecator->setScaledContents(true);
    ui->answerIndecator_3->setScaledContents(true);
    ui->exitSynGameButton->setText(tr("Quit"));
    ui->exitDefGameButton->setText(tr("Quit"));
    ui->answerIndecator->setText("");
    ui->answerIndecator_3->setText("");

    /*    ---   SHORCUTS   ---   */
    QShortcut *upKey = new QShortcut(QKeySequence("Up"), this);
    QObject::connect(upKey, SIGNAL(activated()), this, SLOT(on_pushButton_AnswerA_clicked()));
    QShortcut *downKey = new QShortcut(QKeySequence("Down"), this);
    QObject::connect(downKey, SIGNAL(activated()), this, SLOT(on_pushButton_AnswerB_clicked()));
    QShortcut *rightKey = new QShortcut(QKeySequence("Right"), this);
    QObject::connect(rightKey, SIGNAL(activated()), this, SLOT(on_pushButton_AnswerB_26_clicked()));
    QShortcut *leftKey = new QShortcut(QKeySequence("Left"), this);
    QObject::connect(leftKey, SIGNAL(activated()), this, SLOT(on_pushButton_AnswerA_26_clicked()));

    // PickGame dialog
    ui->stackedWidget->setCurrentIndex(0);
    level = INCREMENTLEVELBY;
    wordSound = "";
}

Games::~Games(){
    delete ui;
}

/* These thee buttons are in the Game Menu.
 * on_StartDefGame_button_clicked starts of Definition Game
*/
void Games::on_StartDefGame_button_clicked(){
    if(dbqe->okToPlay(ui->langComboBox->currentIndex() + 1)){
        setUpDefinitionGame();
    }
}

/* Starts the Synonyms Game. Checks if the chosen language is OK to play
 * that is there at least 4
*/
void Games::on_StartSynGame_button_clicked(){
    if(dbqe->okToPlay(ui->langComboBox->currentIndex() + 1)){
        setupSynonymGame();
    }
}

/* Closes the Games window
*/
void Games::on_quitButton_clicked(){
    close();
}

/* The random int generator to randomise the contents of button
*/
int Games::randomAorB(){
    int rand = (qrand() % (2));
    return rand;
}

/* The random int generator for the distractor
 * qrand() % ((high + 1) - low) + low
*/
int Games::randomInt(){
    int hello = (qrand() % (LASTQUESTION + 1));
    return hello;
}

/* when invoking getPairSet(language, game),,,, 2nd arg is either "definition" or "synonyms"
*/
void Games::setUpDefinitionGame(){ //getPairSet("atiman", lang_index, "to take care of self", "a-ti-man");
    ui->picture->clear();
    ui->answerIndecator->setHidden(true);
    ui->stackedWidget->setCurrentIndex(1);
    question = 0;
    score = 0;
    gamePlayed = 1;
    ui->questionIndecatorLabel->setText(QString("%1 / 8").arg(question + 1));
    ui->progressBar_Def->setValue(question * INCREMENTPROGRESS);
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

/* The next question iterator for the Definition game.
 * This is the function that changes the contents in the button for each question.
 * The inner if statement randomises where the location of the correct answer.
*/
void Games::nextQuestion(){
    ui->answerIndecator->setHidden(true);
    question++;

    if(question <= LASTQUESTION){
        ui->questionIndecatorLabel->setText(QString("%1 / 8").arg(question +1));
        ui->progressBar_Def->setValue(question * INCREMENTPROGRESS);
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

/* This slot is invoked if the a game finishes. Shows a dilog how much the user
 * scored and a choice whether to continue the same game. Increses the game level
 * if YES, resets the level if No.
*/
void Games::gameFinished(){
    liste.clear();
    ui->progressBar_Def->setValue(100);
    ui->progressBar_Syn->setValue(100);
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
        // show Level indicator then disappear.
    } else {
        level = INCREMENTLEVELBY;
        ui->stackedWidget->setCurrentIndex(0);
    }

}

/* This is the slot for the A button in the Dictionary Game.
 * Gets the text in the button(the chosen answer by the player) and
 * answerChecker() is invoked to see if the answer is correct
*/
void Games::on_pushButton_AnswerA_clicked(){ // A
    if(question <= LASTQUESTION ){
        ui->pushButton_AnswerA->setEnabled(false);
        answer = ui->pushButton_AnswerA->text();
        answerChecker(answer, question);
        nextQuestion();
    }
}

/* This is the slot for the B button in the Dictionary Game. Does similar to above
*/
void Games::on_pushButton_AnswerB_clicked(){ // B
    if(question <= LASTQUESTION ){
        ui->pushButton_AnswerB->setEnabled(false);
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

    // Play the wav file
    QSound::play(QString("sounds/%1.wav").arg(wordSound));
}

/* The slot of the quit button in the Definition game.
*/
void Games::on_exitDefGameButton_clicked(){
    level = INCREMENTLEVELBY;
    ui->stackedWidget->setCurrentIndex(0);
}

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

/* Checks in the clicked answer is correct or not
*/
void Games::answerChecker(QString ans, int question){
    bool correct = liste[question].contains(ans);// see if the contents after ":" has the answer
    /*foreach(QString str, liste[question].split(" : ")) // shows the question and answer
        qDebug() << str << " ";*/

    if(correct == true){
        score++;
        ui->answerIndecator->setPixmap(QPixmap::fromImage(correctMark));
        ui->answerIndecator_3->setPixmap(QPixmap::fromImage(correctMark));
        QSound::play(":/new/prefix1/Sound/chime_up.wav");
    } else {
        ui->answerIndecator->setPixmap(QPixmap::fromImage(wrongMark));
        ui->answerIndecator_3->setPixmap(QPixmap::fromImage(wrongMark));
        QSound::play(":/new/prefix1/Sound/boing_spring.wav");
    }

    // show the answer indecator and pause
    ui->answerIndecator->setHidden(false);
    ui->answerIndecator_3->setHidden(false);
    delay();
}

/* The function that delays the screen before it proceed to the next question.
 * This allows time for the answer correct or wrong indicators to be displayed.
*/
void Games::delay(){
    QTime dieTime= QTime::currentTime().addSecs(1);
    while( QTime::currentTime() < dieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    ui->pushButton_AnswerB->setEnabled(true);
    ui->pushButton_AnswerA->setEnabled(true);
    ui->pushButton_AnswerB_26->setEnabled(true);
    ui->pushButton_AnswerA_26->setEnabled(true);
}

/* The random int generator for the distractor in the Synonyms game.
*/
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

/* Sets up the Synonyms game. Get a set of questions in the database, which is 8
 * pairs of word and its synonyms.
*/
void Games::setupSynonymGame(){ //getPairSet("atiman", lang_index, "to take care of self", "a-ti-man");
    ui->answerIndecator_3->setHidden(true);
    ui->stackedWidget->setCurrentIndex(2);
    question = 0;
    score = 0;
    gamePlayed = 2;
    qDebug() << "LEVELLLLL: " << level;
    qDebug() << "Question: " << question << "score: " << score;
    ui->questionIndecatorLabel_3->setText(QString("%1 / 8").arg(question + 1));
    ui->progressBar_Syn->setValue(question * INCREMENTPROGRESS);
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

/* The next question iterator for the Synonym game.
 * This is the function that changes the contents in the button for each question.
 * The inner if statement randomises where the location of the correct answer.
*/
void Games::nextSynQuestion(){
    ui->answerIndecator_3->setHidden(true);
    question++;

    if(question <= LASTQUESTION){
        ui->questionIndecatorLabel_3->setText(QString("%1 / 8").arg(question +1));
        ui->progressBar_Syn->setValue(question * INCREMENTPROGRESS);
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

/* This is the slot for the A(left) button in the Synonym Game.
 * Gets the text in the button(the chosen answer by the player) and
 * answerChecker() is invoked to see if the answer is correct
*/
void Games::on_pushButton_AnswerA_26_clicked(){ // Synonym Answer button A
    if(question <= LASTQUESTION ){
        ui->pushButton_AnswerA_26->setEnabled(false);
        answer = ui->pushButton_AnswerA_26->text();
        answerChecker(answer, question);
        nextSynQuestion();
    }
}

/* This is the slot for the B(right) button in the Synonym Game.
*/
void Games::on_pushButton_AnswerB_26_clicked(){ // Synonym Answer button B
    if(question <= LASTQUESTION ){
        ui->pushButton_AnswerB_26->setEnabled(false);
        answer = ui->pushButton_AnswerB_26->text();
        answerChecker(answer, question);
        nextSynQuestion();
    }
}

/* If student clicks quit button in the Synonym game, reset level to 1 and change
 * screen to the Game Menu.
*/
void Games::on_exitSynGameButton_clicked(){
    level = INCREMENTLEVELBY;
    ui->stackedWidget->setCurrentIndex(0);
}
