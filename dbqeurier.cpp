#include "dbqeurier.h"
#include <QtWidgets>
#include <QDebug>

DBQeurier::DBQeurier()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("LOCALHOST");
    db.setDatabaseName("FinalProjectDB");
    if(!db.open()){
        qDebug() << "ERROR = " << db.lastError().text();
    } else {
        qDebug() << "Its OPEN!!!";
    }
}

DBQeurier* DBQeurier::m_instance = NULL;

DBQeurier* DBQeurier::instance()
{
    if( !m_instance )
    {
        m_instance = new DBQeurier();
    }
    return m_instance;
}

//Dictionary methods
QString DBQeurier::getDefinition(int lang_id, QString wordToFind){
    //QSqlQuery query("SELECT cebuano FROM language",db);
    /* query.exec("SELECT irish FROM language"); */
    // qDebug() << language << wordToFind;
    qStm = QString("SELECT definition FROM word WHERE word = '%1' AND language_id = %2").arg(wordToFind.toLower()).arg(lang_id);

    QSqlQuery query(qStm,db);
    output = "";
    while(query.next()){
        output = query.value(0).toString();
    }
    query.finish();
    return output;
}

void DBQeurier::addEntry(QString word, int lang_id, QString def, QString pronun){

    addSynEntry(word, "take care");
    //QString sQuery = "INSERT INTO test.people (id, first, last) VALUES(:id, :first, :last)";
    qStm = QString("INSERT INTO finalproject.word (word, language_id, definition, pronunciation) VALUES(:word, :lang_id, :definition, :pronunciation)");
    qDebug() << qStm;
    QSqlQuery query;
    query.prepare(qStm);
    query.bindValue(":word", word);
    query.bindValue(":language_id", lang_id);
    query.bindValue(":definition", def);
    query.bindValue(":pronunciation", pronun);
    //query.bindValue(":sound", sound);
    //query.bindValue(":image", image);

    if(query.exec()){
        QMessageBox::information(this, "Database Updated",
            QString("The entry '%1 - %2' is added to the language ").arg(word).arg(def));
    } else {
        QMessageBox::warning(this, "Entry Error",
            QString("There's a problem adding entry. %1 is probably added already.").arg(word));
    }
}

void DBQeurier::addSynEntry(QString word, QString syn){
    qStm = QString("INSERT INTO finalproject.synonyms (word, synonyms) VALUES(:word, :synonyms)");

    QSqlQuery query;
    query.prepare(qStm);
    query.bindValue(":word", word);
    query.bindValue(":synonyms", syn + "|");

    if(query.exec()){
        qDebug() << "Synonym table appended";
    }
}

void DBQeurier::updateDefinition(int lang_id, QString word, QString newDef){
    qStm = QString("UPDATE word SET definition = '%1' WHERE language_id = %2 AND word = '%3'").arg(newDef).arg(lang_id).arg(word);
    qDebug() << word << " newDef is: " << newDef ;
    QSqlQuery query;
    query.prepare(qStm);

    if(query.exec()){
        QMessageBox::information(this, "Database Updated",
                                 QString("The definition of %1' is updated").arg(word));
    }
    /*qry.bindValue(":id","4");
    qry.bindValue(":first","Heather");
    qry.bindValue(":last","SuperMan");*/
}

//Thesaurus methods
QStringList DBQeurier::getSynonyms(int lang_id, QString word){
    // the synonyms of a word are on the third column of the table
    synonyms.clear();

    qStm = QString("SELECT synonyms FROM synonyms INNER JOIN word ON synonyms.word = word.word WHERE word.language_id = %1 AND synonyms.word = '%2'").arg(lang_id).arg(word.toLower());
    QSqlQuery query(qStm,db);
    qDebug() << lang_id << "    " << word;
    while(query.next()){
        output = query.value(0).toString();
    }
    synonyms = output.split("|");
    output = "";
    query.finish();
    /*QMessageBox::warning(this, "Word not in Database",
        QString("%1 is not in the database").arg(word));*/

    return synonyms;
}

void DBQeurier::associateWord(QString word, QString wordToAdd){
    qStm = QString("UPDATE synonyms SET synonyms = CONCAT(synonyms, '%1|') WHERE word = '%2'").arg(wordToAdd.toLower()).arg(word.toLower());
    QSqlQuery query;
    query.prepare(qStm);
    if(query.exec()){
        QMessageBox::information(this, "Synonym Added",
                                 QString("The word '%1' now has a synonym '%2'").arg(word).arg(wordToAdd));
    }
}

bool DBQeurier::addNewLanguage(int lang_id, QString language, QString iso_code){
    qStm = QString("INSERT INTO finalproject.language (language_id, language_name, iso_code) VALUES(:language_id, :language_name, :iso_code)");
    QSqlQuery query;
    query.prepare(qStm);
    query.bindValue(":language_id", lang_id);
    query.bindValue(":language_name", language);
    query.bindValue(":iso_code", iso_code);

    return query.exec();
}

QStringList DBQeurier::getWordsSet(int lang_id, int level){
    qStm = QString("SELECT word, definition FROM word WHERE language_id = %1 LIMIT %2").arg(lang_id).arg(level); //ORDER BY RAND() LIMIT 15
    QSqlQuery query(qStm,db);
    pairSet.clear();
    while(query.next()){
        // word and the definition or synonyms
        output = query.value(0).toString() + " : " +  query.value(1).toString();
        pairSet << output;
        //qDebug() << "Got Here,,, Invokedd" << output;
    }
    query.finish();
    return pairSet;
}

QStringList DBQeurier::getSynSet(int lang_id, int level){
    synonyms.clear();
    qStm = QString("SELECT  word.word, synonyms FROM synonyms INNER JOIN word ON synonyms.word=word.word WHERE word.language_id = %1 LIMIT %2").arg(lang_id).arg(level);
    QSqlQuery query(qStm,db);
    while(query.next()){
        output = query.value(0).toString() + " : " +  query.value(1).toString();
        synonyms << output;
    }
    query.finish();
    return synonyms;
}

int DBQeurier::login(QString username, QString password){
    int count = 0;
    qStm = QString("SELECT * FROM teacher WHERE username = '%1'  AND password = '%2' ").arg(username).arg(password);
    QSqlQuery query(qStm,db);

    while(query.next()){
        count++;
    }

    qDebug() << count;
    return count;
}
