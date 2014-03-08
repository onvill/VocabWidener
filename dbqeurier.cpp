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


   /*if(query.exec()){
            qDebug() << "Record Inserted!!";
    } else {
        qDebug() << "ERRORRR = " << db.lastError().text();
    }
    */

    //db.close();
}

//Dictionary methods
QString DBQeurier::getDefinition(QString language, QString wordToFind){
    //QSqlQuery query("SELECT cebuano FROM language",db);
    /* query.exec("SELECT irish FROM language"); */

    QString output;
    qStm = QString("SELECT definition FROM %1 WHERE word = '%2'").arg(language.toLower()).arg(wordToFind.toLower());
    QSqlQuery query(qStm,db);
    while(query.next()){
        output = query.value(0).toString();
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

void DBQeurier::updateDefinition(QString language, QString word){
    /*qry.bindValue(":id","4");
    qry.bindValue(":first","Heather");
    qry.bindValue(":last","SuperMan");*/
}

//Thesaurus methods
QStringList DBQeurier::getSynonyms(){

}

void DBQeurier::associateWord(QString addWord, QString word){

}

void DBQeurier::addNewLanguage(){

}
