#ifndef ADDSYNONYM_H
#define ADDSYNONYM_H

#include <QDialog>
#include "dbqeurier.h"

namespace Ui {
class AddSynonym;
}

class AddSynonym : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddSynonym(QWidget *parent = 0);
    ~AddSynonym();

private slots:
    void addClicked();
    void cancelClicked();


private:
    Ui::AddSynonym *ui;

    DBQeurier *dbqe;
    QString word;
    QString newSyn;
};

#endif // ADDSYNONYM_H
