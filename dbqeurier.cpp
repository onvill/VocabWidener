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

    //QString sQuery = "INSERT INTO test.people (id, first, last) VALUES(:id, :first, :last)";

}

//Dictionary methods
QString DBQeurier::getDefinition(QString language, QString wordToFind){
    //QSqlQuery query("SELECT cebuano FROM language",db);
    /* query.exec("SELECT irish FROM language"); */

   // qDebug() << language << wordToFind;
    qStm = QString("SELECT definition FROM %1 WHERE word = '%2'").arg(language.toLower()).arg(wordToFind.toLower());
    QSqlQuery query(qStm,db);
    while(query.next()){
        output = query.value(0).toString();
        //qDebug() << output;
    }

    return output;
}

void DBQeurier::addEntry(QString language, QString word, QString def){
    qStm = QString("INSERT INTO finalproject.%1 (word, definition) VALUES(:word, :definition)").arg(language.toLower());
    qDebug() << qStm;
    QSqlQuery query;
    query.prepare(qStm);
    query.bindValue(":word", word);
    query.bindValue(":definition", def);

    if(query.exec()){
        QMessageBox::information(this, "Database Updated",
            QString("The entry '%1 - %2' is added to the language %3.").arg(word).arg(def).arg(language));
    } else {
        QMessageBox::warning(this, "Entry Error",
            QString("There's a problem adding entry. %1 is probably added already.").arg(word));
    }
}

void DBQeurier::updateDefinition(QString language, QString word, QString newDef){
    qStm = QString("UPDATE %1 SET definition = '%2' WHERE word = '%3'").arg(language.toLower()).arg(newDef).arg(word);
    //qDebug() << qStm;
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
QStringList DBQeurier::getSynonyms(QString language, QString word){
    // the synonyms of a word are on the third column of the table
    QStringList synonyms;
    qStm = QString("SELECT synonyms FROM %1 WHERE word = '%2'").arg(language.toLower()).arg(word.toLower());
    QSqlQuery query(qStm,db);

    while(query.next()){
        output = query.value(0).toString();
    }
     synonyms = output.split("|");
     qDebug() << "METHOD INVOKED";
    /*QMessageBox::warning(this, "Word not in Database",
        QString("%1 is not in the database").arg(word));*/

    return synonyms;
}

void DBQeurier::associateWord(QString language, QString word, QString wordToAdd){
    qStm = QString("UPDATE %1 SET synonyms = CONCAT(synonyms, '%2|') WHERE word = '%3'").arg(language.toLower()).arg(wordToAdd.toLower()).arg(word.toLower());
    QSqlQuery query;
    query.prepare(qStm);
    if(query.exec()){
        QMessageBox::information(this, "Synonym Added",
                                 QString("The word '%1' now has a synonym '%2'").arg(word).arg(wordToAdd));
    }
}

bool DBQeurier::addNewLanguage(QString language){
    // Create table "person"
    qStm = QString("CREATE TABLE finalproject.%1 (word VARCHAR(45) NOT NULL, definition VARCHAR(255) NOT NULL, synonyms TEXT NULL, PRIMARY KEY (word))").arg(language.toLower());
    bool ret = false;
    if (db.isOpen()){
        QSqlQuery query;
        query.prepare(qStm);
        ret = query.exec();
    }
    qDebug() << "ERROR: " << db.lastError().text();
    return ret;
}
