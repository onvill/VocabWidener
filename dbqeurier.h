#ifndef DBQEURIER_H
#define DBQEURIER_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

class DBQeurier : public QMainWindow
{
public:
    static DBQeurier* instance();

    //Dictionary methods
    QString getDefinition(int lang_id, QString wordToFind);
    void updateDefinition(int lang_id, QString word, QString newDef);
    void addEntry(QString word, int lang_id, QString def, QString pronun);
    void newlanguage();
    QByteArray getSoundOrPicBytes(QString word, QString fileType);
    void storePic(QByteArray pByte);
    void storeSound(QByteArray sByte);

    //Thesaurus methods
    QStringList getSynonyms(int lang_id, QString word);
    void associateWord(QString word, QString wordToAdd);
    bool addNewLanguage(int lang_id, QString language, QString iso_code);
    void addSynEntry(QString word, QString syn);

    //Games methods
    QStringList getWordsSet(int lang_id, int level);
    QStringList getSynSet(int lang_id, int level);

    //Login
    int login(QString username, QString password);


private:
    static DBQeurier* m_instance;
    DBQeurier();
    ~DBQeurier(){};

    QString qStm;
    QSqlDatabase db;
    QString output;
    QStringList pairSet;
    QStringList synonyms;
    QByteArray byteArray;

    // QSqlQuery *query;
};

#endif // DBQEURIER_H
