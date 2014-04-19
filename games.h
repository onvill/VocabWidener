#ifndef GAMES_H
#define GAMES_H

#include <QDialog>
#include "dbqeurier.h"
#include "qsound.h"
#include <QByteArray>
#include <QBuffer>
#include <QtMultimedia>
#include <QAudioOutput>
#define LASTQUESTION 7
#define INCREMENTLEVELBY 8

namespace Ui {
class Games;
}

class Games : public QDialog
{
    Q_OBJECT
    
public:
    explicit Games(const QStringList& langList, QWidget *parent = 0);
    ~Games();
    
private slots:
    void on_soundButton_clicked();
    void on_pushButton_AnswerA_clicked();
    void on_pushButton_AnswerB_clicked();
    void on_StartDefGame_button_clicked();
    void on_StartSynGame_button_clicked();
    void on_quitButton_clicked();
    void on_pushButton_AnswerA_26_clicked();
    void on_pushButton_AnswerB_26_clicked();

    void on_exitSynGameButton_clicked();

    void on_exitDefGameButton_clicked();

private:
    Ui::Games *ui;
    int score;
    int gamePlayed;
    bool m_buttonPressed;
    int question;
    int level;
    QStringList liste;
    QStringList portion;
    QString answer;
    QString wordSound;
    QString ansOrBoggy;
    QString ansOrBoggy2;
    DBQeurier *dbqe;
    QPixmap pixmap;

    void setUpDefinitionGame();
    int randomInt();
    int randomAorB();
    int randomInte(int ansListLegth);
    QString wrongAnswerPicker(QStringList list, int i);
    QStringList definitionSet;
    QStringList synonymSet;
    void answerChecker(QString ans, int q);
    void nextQuestion();
    void gameFinished();

    void setupSynonymGame();
    void nextSynQuestion();
    QString answerChoper(QString ans);

};

#endif // GAMES_H
