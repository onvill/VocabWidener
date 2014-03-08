#ifndef DBQEURIER_H
#define DBQEURIER_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class DBQeurier : public QMainWindow
{
public:
    DBQeurier();

    int one();
    //Dictionary methods
    QString getDefinition(QString language, QString wordToFind);
    void updateDefinition(QString language, QString word);
    void addEntry(QString language, QString word, QString def);
    void newlanguage();


    //Thesaurus methods
    QStringList getSynonyms();
    void associateWord(QString addWord, QString word);

    void addNewLanguage();


private:
    QString qStm;
    QSqlDatabase db;
   // QSqlQuery *query;
};

#endif // DBQEURIER_H
