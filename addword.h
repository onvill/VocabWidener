#ifndef ADDWORD_H
#define ADDWORD_H

#include <QDialog>
#include "dbqeurier.h"
#include "fileexplorer.h"
#include <QFileSystemModel>

namespace Ui {
class AddWord;
}

class AddWord : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddWord(const QStringList& langList, QWidget *parent = 0);
    ~AddWord();

private slots:
    void addClicked();
    void cancelClicked();
    void on_imageButton_clicked();
    void on_soundButton_clicked();
    void picFilePath(QString path);
    void wavFilePath(QString path);

private:
    Ui::AddWord *ui;

    DBQeurier *dbqe;
    QString word;
    QString definition;
    QString pronunciation;
    QString picfilePath;
    QString wavfilePath;
    QFileSystemModel *fileModel;

};

#endif // ADDWORD_H
