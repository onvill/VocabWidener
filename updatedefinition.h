#ifndef UPDATEDEFINITION_H
#define UPDATEDEFINITION_H

#include <QDialog>
#include "dbqeurier.h"

namespace Ui {
class UpdateDefinition;
}

class UpdateDefinition : public QDialog
{
    Q_OBJECT
    
public:
    explicit UpdateDefinition(const QStringList& langList, QWidget *parent = 0);
    ~UpdateDefinition();

private slots:
    void updateClicked();
    void cancelClicked();
    
private:
    Ui::UpdateDefinition *ui;

    DBQeurier *dbqe;
    QString word;
    QString newDef;
};

#endif // UPDATEDEFINITION_H
