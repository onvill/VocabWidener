#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QTabWidget>
#include <QAction>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // setCentralWidget(ui->tabWidget);


}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_actionAbout_triggered(){
    QMessageBox::about(this, "VocabWidener",
                       "This program will help you to enrich your Vocabulary. The Dictionary tab allows you to search definitions of words. The Thesaurus finds synonyms of a word you enter. The Games tests your vocabulary.");
}

void MainWindow::on_actionSpanish_triggered(){
    ui->statusBar->showMessage("Interface language Changed to Spanish",2000);
}

void MainWindow::on_actionCebuano_triggered(){
    ui->statusBar->showMessage("Interface language Changed to Cebuano",2000);
}

void MainWindow::on_actionIrish_triggered(){
    ui->statusBar->showMessage("Interface language Changed to Irish",2000);
}

void MainWindow::on_actionExit_triggered(){
    QApplication::quit();   // EXIT
}

void MainWindow::on_actionNew_Language_triggered(){ // New Language, create two columns

}
