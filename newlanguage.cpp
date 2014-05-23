#include "newlanguage.h"
#include "ui_newlanguage.h"
#include <QDebug>

NewLanguage::NewLanguage(const QStringList& langList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewLanguage)
{
    ui->setupUi(this);
    dbqe = DBQeurier::instance();
    languagesCount = langList.length();

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

}

NewLanguage::~NewLanguage()
{
    delete ui;
}

/* Gets the values in the line edits and pass them as
 * arguments to the function that adds a new language
 * in the database.
*/
void NewLanguage::addButtonClicked(){
    lang = ui->lineEdit_lang->text();
    iso = ui->lineEdit_iso->text();
    if(!lang.isEmpty() && !iso.isEmpty()){
        languagesCount++;
        dbqe->addNewLanguage(languagesCount, lang, iso);
    }

    qDebug() << "addButtonClicked";
}

/* Closes the dialog
*/
void NewLanguage::cancelClicked(){
    qDebug() << "cancelClicked";
    close();
}
