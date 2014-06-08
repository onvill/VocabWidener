#ifndef NEWLANGUAGE_H
#define NEWLANGUAGE_H

#include <QDialog>
#include "dbqeurier.h"

namespace Ui {
class NewLanguage;
}

class NewLanguage : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewLanguage(const QStringList& langList, QWidget *parent = 0);
    ~NewLanguage();

signals:
    void newLanguage(QString newlang);

private slots:
    void addButtonClicked();
    void cancelClicked();

private:
    Ui::NewLanguage *ui;

    DBQeurier *dbqe;
    QString lang;
    QString iso;
    int languagesCount;
};

#endif // NEWLANGUAGE_H
