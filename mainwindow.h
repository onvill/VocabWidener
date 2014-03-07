#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QTabWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionSpanish_triggered();

    void on_actionCebuano_triggered();

    void on_actionIrish_triggered();

    void on_actionExit_triggered();

    void on_actionNew_Language_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
