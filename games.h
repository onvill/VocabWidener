#ifndef GAMES_H
#define GAMES_H

#include <QDialog>
#include "dbqeurier.h"
#define LASTQUESTION 7


namespace Ui {
class Games;
}

class Games : public QDialog
{
    Q_OBJECT
    
public:
    explicit Games(QWidget *parent = 0);
    ~Games();
    
private slots:
    void on_soundButton_clicked();

    void on_pushButton_AnswerA_clicked();

    void on_pushButton_AnswerB_clicked();


private:
    int score;
    bool m_buttonPressed;
    int question;
    QStringList liste;
    QStringList portion;

    Ui::Games *ui;
    DBQeurier *dbqe;
    void setUpDefinitionGame();
    int randomInt();
    QString wrongAnswerPicker(QStringList list, int i);
    QStringList definitionSet;
    QStringList synonymSet;
    void dictionaryGame();
    void thesaurusGame();
    void delay();
    QString answer;
    QTimer *timer;
    void answerChecker(QString ans, int q);
    void nextQuestion();
    void gameFinished();

};

#endif // GAMES_H
