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
    void updateDefinition(QString language, QString word, QString newDef);
    void addEntry(QString language, QString word, QString def);
    void newlanguage();


    //Thesaurus methods
     QStringList getSynonyms(QString language, QString word);
    void associateWord(QString language, QString word, QString wordToAdd);

    bool addNewLanguage(QString language);

private:
    QString qStm;
    QSqlDatabase db;
    QString output;
   // QSqlQuery *query;
};

#endif // DBQEURIER_H
