#include "addword.h"
#include "ui_addword.h"
#include <QDebug>


AddWord::AddWord(const QStringList& langList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWord)
{
    ui->setupUi(this);

    picfilePath = "";
    wavfilePath = "";
    ui->comboBox->addItems(langList);
    dbqe = DBQeurier::instance();

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

}

AddWord::~AddWord()
{
    delete ui;
}

/* Gets the values in the line edits and pass them as arguments
 * to the function that inserts new word into the database.
*/
void AddWord::addClicked(){
    int lang_index = ui->comboBox->currentIndex() + 1;
    word = ui->lineEdit_word->text();
    definition  = ui->lineEdit_Definition->text();
    pronunciation = ui->lineEdit_Pronun->text();
    if(word.isEmpty() || definition.isEmpty() || pronunciation.isEmpty()
            || picfilePath.isEmpty() || wavfilePath.isEmpty()){
        qDebug() << "All fields not filled";
    } else {
        QImage image(picfilePath);
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "jpg");  // writes image into byteArray in jpg format

        QFile wavFile(wavfilePath);
        if (wavFile.open(QIODevice::ReadOnly)){
            QByteArray wavBytes(wavFile.readAll());
            dbqe->addEntry(word, lang_index, definition, pronunciation, wavBytes, byteArray);
        }
    }

}

/* Cancel button i clicked and close the window.
*/
void AddWord::cancelClicked(){
    qDebug() << "cancelClicked";
    close();
}

void AddWord::picFilePath(QString path){
    picfilePath = path;
    qDebug() << "picFilePath INVOKED" << " path is " << picfilePath;
}

void AddWord::wavFilePath(QString path){
    wavfilePath = path;
    qDebug() << "wavFilePath INVOKED" << " path is " << wavfilePath;
}

/* The get image button is clicked so open a file explorer for the user
 * to find the image.
*/
void AddWord::on_imageButton_clicked(){
    FileExplorer fexo(this);
    fexo.setModal(true);
    connect(&fexo, SIGNAL(myQString(QString)), SLOT(picFilePath(QString)));
    fexo.exec();
}

/* The get sound button is clicked so open a file explorer for the user
 * to find the sound.
*/
void AddWord::on_soundButton_clicked(){
    FileExplorer fexo(this);
    fexo.setModal(true);
    connect(&fexo, SIGNAL(myQString(QString)), SLOT(wavFilePath(QString)));
    fexo.exec();

    QImage image(picfilePath);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    if(buffer.open(QIODevice::WriteOnly)){
        image.save(&buffer, "jpg");
    }

    QFile wavFile(wavfilePath);
    if(wavFile.open(QIODevice::ReadOnly)){
        QByteArray wavBytes(wavFile.readAll());
    }
}



