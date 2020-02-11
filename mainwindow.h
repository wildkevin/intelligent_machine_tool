#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    static QLabel *configStatus;
    static QLabel *comStatus;
    void initActions();

    static QString filename;//≈‰÷√Œƒº˛√˚

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static QString GetFileName();
    static void SetFileName(QString f);
    static void UpdateStatusBar();



private slots:
    void on_actionloadxml_triggered();
    void on_actionsetting_triggered();
    void on_actionopen_triggered();
};

#endif // MAINWINDOW_H
