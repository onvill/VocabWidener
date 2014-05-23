#include "fileexplorer.h"
#include "ui_fileexplorer.h"
#include <QDebug>


FileExplorer::FileExplorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileExplorer)
{
    ui->setupUi(this);

    QString sPath = "C:/";
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    fileModel->setRootPath(sPath);

    ui->treeView->setModel(fileModel);

}

FileExplorer::~FileExplorer()
{
    delete ui;
}

/* Gets the absulote path of the file double clicked and
 * emits the string for the addword class to fetch
*/
void FileExplorer::on_treeView_doubleClicked(const QModelIndex &index){
    QString filePath = fileModel->fileInfo(index).absoluteFilePath();

    emit myQString(filePath);
    accept();
}
