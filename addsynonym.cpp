#include "addsynonym.h"
#include "ui_addsynonym.h"
#include <QDebug>

AddSynonym::AddSynonym(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSynonym)
{
    ui->setupUi(this);

    dbqe = DBQeurier::instance();

    connect(ui->addSynButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
}

AddSynonym::~AddSynonym()
{
    delete ui;
}

/* Gets the values in the line edits and pass them as
 * arguments to the function that adds a synonym to a word.
 * Make sure each word has at least a synonym.
*/
void AddSynonym::addClicked(){
    word = ui->lineEdit_word->text();
    newSyn = ui->lineEdit_synToAdd->text();
    if(!word.isEmpty() && !newSyn.isEmpty()){
        dbqe->associateWord(word, newSyn);
    }

    qDebug() << "addClicked " << word << " " << newSyn;
}

/* Closes the dialog
*/
void AddSynonym::cancelClicked(){
    qDebug() << "cancelClicked";
    close();
}
