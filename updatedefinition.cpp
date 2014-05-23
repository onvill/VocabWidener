#include "updatedefinition.h"
#include "ui_updatedefinition.h"
#include <QDebug>

UpdateDefinition::UpdateDefinition(const QStringList& langList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDefinition)
{
    ui->setupUi(this);
    //this->setWindowTitle(tr("updatedefinition"));

    dbqe = DBQeurier::instance();
    ui->comboBox->addItems(langList);

    connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(updateClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    //dbqe->getDefinition(id, word, new_def);
}

UpdateDefinition::~UpdateDefinition()
{
    delete ui;
}

/* Gets the values in the line edits and pass them as
 * arguments to the function that updates a word definition.
*/
void UpdateDefinition::updateClicked(){
    int lang_index = ui->comboBox->currentIndex() + 1;
    word = ui->lineEdit_word->text();
    newDef = ui->lineEdit_Definition->text();
    if(!word.isEmpty() && !newDef.isEmpty()){
        dbqe->updateDefinition(lang_index, word, newDef);
    }

    qDebug() << "updateClicked " << lang_index << " " << word <<  " " << newDef;
}

/* Closes the dialog
*/
void UpdateDefinition::cancelClicked(){
    qDebug() << "cancelClicked";
    close();
}
