#ifndef GAMES_H
#define GAMES_H

#include <QDialog>
#include "dbqeurier.h"

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
    bool paused;

    Ui::Games *ui;
    DBQeurier *dbqe;
    void gamesHere();
    int randomInt();
    QString wrongAnswerPicker(QStringList list, int i);
    QStringList definitionSet;
    QStringList synonymSet;
    void dictionaryGame();
    void thesaurusGame();
    void delay();
    QString answer;
    QTimer *timer;

};

#endif // GAMES_H
