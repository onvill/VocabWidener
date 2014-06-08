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
    if(!lang.isEmpty() && lang.length() >= 4 && !iso.isEmpty()){
        lang = lang.at(0).toUpper() + lang.mid(1);
        languagesCount++;
        if(dbqe->addNewLanguage(languagesCount, lang, iso)){
            QMessageBox::information(this, tr("New Language Added"),
                                     QString(tr("The Language '%1' is added")).arg(lang));
            emit newLanguage(lang);
            this->close();
        } else {
            QMessageBox::warning(this, tr("Error"),
                                     QString(tr("The Language '%1' is already added")).arg(lang));
            languagesCount--;
        }
    }

    qDebug() << "addButtonClicked";
}

/* Closes the dialog
*/
void NewLanguage::cancelClicked(){
    qDebug() << "cancelClicked";
    close();
}
