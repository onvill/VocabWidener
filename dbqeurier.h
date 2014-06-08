#ifndef DBQEURIER_H
#define DBQEURIER_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtWidgets>

class DBQeurier : public QMainWindow
{
public:
    static DBQeurier* instance();

    QStringList getLangList();

    //Dictionary methods
    QString getDefinition(int lang_id, QString wordToFind);
    QStringList getDictOutput(int lang_id, QString wordToFind);
    void updateDefinition(int lang_id, QString word, QString newDef);
    void addEntry(QString word, int lang_id, QString def, QString pronun, QByteArray picByte, QByteArray soundByte);
    void updateMediaFile(QString word);

    //Thesaurus methods
    QStringList getSynonyms(int lang_id, QString word);
    void associateWord(QString word, QString wordToAdd);
    bool addNewLanguage(int lang_id, QString language, QString iso_code);
    void addSynEntry(QString word, QString syn);
    void addSynEntry(QString word);

    //Games methods
    QStringList getWordsSet(int lang_id, int level);
    QStringList getSynSet(int lang_id, int level);
    QByteArray getSoundOrPicBytes(QString word, QString fileType);
    bool okToPlay(int lang_id);

    //Login
    int login(QString username, QString password);

    DBQeurier();
    ~DBQeurier(){};

private:
    static DBQeurier* m_instance;

    QString qStm;
    QSqlDatabase db;
    QString output;
    QStringList pairSet;
    QStringList synonyms;
    QStringList dictOutput;
    QByteArray byteArray;

};

#endif // DBQEURIER_H
