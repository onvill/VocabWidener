#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class FileExplorer;
}

class FileExplorer : public QDialog
{
    Q_OBJECT
    
public:
    explicit FileExplorer(QWidget *parent = 0);
    ~FileExplorer();
    
private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

signals:
    void myQString(QString path);

private:
    Ui::FileExplorer *ui;

    QFileSystemModel *fileModel;
};

#endif // FILEEXPLORER_H
