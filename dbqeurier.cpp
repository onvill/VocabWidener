#include "dbqeurier.h"
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

/* Returns the instance of this class if its already instantiated
*/
DBQeurier* DBQeurier::instance()
{
    if( !m_instance )
    {
        m_instance = new DBQeurier();
    }
    return m_instance;
}

// *************** DICTIONARY FUNCTIONS ***************
/* Returns the definition of a word
*/
QString DBQeurier::getDefinition(int lang_id, QString wordToFind){
    //QSqlQuery query("SELECT cebuano FROM language",db);
    /* query.exec("SELECT irish FROM language"); */
    qStm = QString("SELECT definition FROM word WHERE word = '%1' AND language_id = %2").arg(wordToFind.toLower()).arg(lang_id);

    QSqlQuery query(qStm,db);
    output = "";
    while(query.next()){
        output = query.value(0).toString();
    }
    query.finish();
    return output;
}

/* Returns the definition and pronunctiation of a word as a list of String
*/
QStringList DBQeurier::getDictOutput(int lang_id, QString wordToFind){
    qStm = QString("SELECT definition, pronunciation FROM word WHERE word = '%1' AND language_id = %2").arg(wordToFind.toLower()).arg(lang_id);
    QSqlQuery query(qStm,db);
    dictOutput.clear();
    QStringList notInDB;
    notInDB << " " << " ";
    while(query.next()){
        dictOutput << query.value(0).toString() <<  query.value(1).toString();
    }
    query.finish();

    if(dictOutput.length() < 1){
        return notInDB;
    }
    return dictOutput;
}

/* Adds a row in the words table, they are word, language ID, definition,
 * pronunciation, image and sound files
*/
void DBQeurier::addEntry(QString word, int lang_id, QString def, QString pronun, QByteArray picByte, QByteArray soundByte){
    addSynEntry(word);
    //QString sQuery = "INSERT INTO test.people (id, first, last) VALUES(:id, :first, :last)";
    qStm = QString("INSERT INTO finalproject.word (word, language_id, definition, pronunciation, sound, image) VALUES(:word, :language_id, :definition, :pronunciation, :image, :sound)");
    qDebug() << qStm;
    QSqlQuery query;
    query.prepare(qStm);
    query.bindValue(":word", word);
    query.bindValue(":language_id", lang_id);
    query.bindValue(":definition", def);
    query.bindValue(":pronunciation", pronun);
    query.bindValue(":sound", soundByte);
    query.bindValue(":image", picByte);

    if(query.exec()){
        QMessageBox::information(this, "Word Added to Database",
            QString("The entry '%1 - %2' is added to the language ").arg(word).arg(def));
    } else {
        QMessageBox::warning(this, "Entry Error",
            QString("There's a problem adding entry. %1 is probably added already.").arg(word));
    }
}

/* This updates the definition of a word. lang_id is required to determine
 * the language of the word.
*/
void DBQeurier::updateDefinition(int lang_id, QString word, QString newDef){
    qStm = QString("UPDATE word SET definition = '%1' WHERE language_id = %2 AND word = '%3'").arg(newDef).arg(lang_id).arg(word);
    qDebug() << word << " newDef is: " << newDef ;
    QSqlQuery query;
    query.prepare(qStm);

    if(query.exec()){
        QMessageBox::information(this, "Database Updated",
                                 QString("The definition of %1' is updated").arg(word));
    } else {
        QMessageBox::warning(this, "Warning",
                                 QString("The word %1' is not in database. Add it first.").arg(word));
    }
    /*qry.bindValue(":id","4");
    qry.bindValue(":first","Heather");
    qry.bindValue(":last","SuperMan");*/
}




// ***************   THESAURUS FUNCTIONS ***************
/* Adds a word and its first synonym into the synonyms table
*/
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

/* Adds a word into the synonyms table, words are still yet to be
 * associated to it. This func is invoked by addWord, every time
 * a word is added to the word table that word must also be added to
 * the synonyms table.s
*/
void DBQeurier::addSynEntry(QString word){
    qStm = QString("INSERT INTO finalproject.synonyms (word) VALUES(:word)");

    QSqlQuery query;
    query.prepare(qStm);
    query.bindValue(":word", word);

    if(query.exec()){
        qDebug() << word << " is Added!";
    }
}

/* Gets the synoynms of the word supplied as argument
*/
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

/* Updates an entry in the synonyms table. Adds a word as a
 * sysnoynm to a word.
*/
void DBQeurier::associateWord(QString word, QString wordToAdd){
    qStm = QString("UPDATE synonyms SET synonyms = CONCAT(synonyms, '|%1') WHERE word = '%2'").arg(wordToAdd.toLower()).arg(word.toLower());
    QSqlQuery query;
    query.prepare(qStm);
    if(query.exec()){
        QMessageBox::information(this, "Synonym Added",
                                 QString("The word '%1' now has the synonym '%2'").arg(word).arg(wordToAdd));
    } else {
        QMessageBox::warning(this, "Error",
                                 QString("The word '%1' doesn't exist.").arg(word));
    }
}

/* Creates a new row in the language table. Each language has an id, name
 * and ISO code.
*/
bool DBQeurier::addNewLanguage(int lang_id, QString language, QString iso_code){
    qStm = QString("INSERT INTO finalproject.language (language_id, language_name, iso_code) VALUES(:language_id, :language_name, :iso_code)");
    QSqlQuery query;
    query.prepare(qStm);
    query.bindValue(":language_id", lang_id);
    query.bindValue(":language_name", language);
    query.bindValue(":iso_code", iso_code);

    return query.exec();
}

// *************** GAMES FUNCTIONS ***************
/* Returns a list of random words(8 words). They are needed in the games
 * section. The domain of possible words increases as the level increases
*/
QStringList DBQeurier::getWordsSet(int lang_id, int level){
    /* SELECT word, definition FROM (
     *     SELECT word, definition FROM word
     *     WHERE language_id = %1
     *     LIMIT 1, %2
     * ) AS TEMP
     * AS TEMP ORDER BY RAND() LIMIT 8
    */
    qStm = QString("SELECT word, definition FROM ( SELECT word, definition FROM word WHERE language_id = %1  LIMIT 1, %2  ) AS TEMP ORDER BY RAND() LIMIT 8").arg(lang_id).arg(level);
    QSqlQuery query(qStm,db);
    pairSet.clear();
    while(query.next()){
        // word and the definition or synonyms
        output = query.value(0).toString() + " : " +  query.value(1).toString();
        pairSet << output;
    }

    query.finish();
    return pairSet;
}

/* Returns a list set of word:synonyms. This function is required
 * by the Synonyms game
*/
QStringList DBQeurier::getSynSet(int lang_id, int level){
    /*  SELECT word, synonyms  FROM(
     *      SELECT word.word, synonyms FROM synonyms
     *      INNER JOIN word ON synonyms.word=word.word
     *      WHERE word.language_id = %1
     *      LIMIT 1, %2
     *  ) AS TEMP
     *  ORDER BY RAND() LIMIT 8
    */
    synonyms.clear();
    qStm = QString("SELECT word, synonyms FROM(SELECT word.word, synonyms FROM synonyms INNER JOIN word ON synonyms.word=word.word WHERE word.language_id = %1 LIMIT 1, %2 ) AS TEMP  ORDER BY RAND() LIMIT 8").arg(lang_id).arg(level);
    QSqlQuery query(qStm,db);
    while(query.next()){
        output = query.value(0).toString() + " : " +  query.value(1).toString();
        synonyms << output;
    }
    query.finish();
    return synonyms;
}

/* This function gets the byteArray of a word's picture or sound
*/
QByteArray DBQeurier::getSoundOrPicBytes(QString word, QString fileType){
    byteArray.clear();
    qStm = QString("SELECT %1 FROM word WHERE word = '%2'").arg(fileType).arg(word);
    QSqlQuery query(qStm,db);
    query.next();
    byteArray = query.value(0).toByteArray();

    query.finish();
    return byteArray;
}

/* Function needed when a teacher decides to change the picture/sound of an word
*/
void DBQeurier::updateMediaFile(QString word){
    qStm = QString("UPDATE word image = 'ByteArray' WHERE word = '%1'").arg(word);
    QSqlQuery query;
    query.prepare(qStm);
    qDebug() << "Byte Stored " << query.exec();

}

/* This queries the database if the username and password entered
 * by the user is in the database. Returns 1 if it is
*/
int DBQeurier::login(QString username, QString password){
    int count = 0;
    qStm = QString("SELECT * FROM teacher WHERE username = '%1'  AND password = '%2' ").arg(username).arg(password);
    QSqlQuery query(qStm,db);

    while(query.next()){
        count++;
    }

    return count;
}
