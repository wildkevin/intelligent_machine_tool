#ifndef RULESETTINGWIDGET_H
#define RULESETTINGWIDGET_H

#include <QTabWidget>
#include <mainwindow.h>

namespace Ui {
class RuleSettingWidget;
}

class RuleSettingWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit RuleSettingWidget(QWidget *parent = 0);
    ~RuleSettingWidget();
    void readXML(QString read_filename);

private slots:
    void on_buttonAddCom_clicked();

    void on_buttonRemoveCom_clicked();

    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_addButton2_clicked();

    void on_btn_do_clicked();

    void on_pushButton_clicked();

    void on_removeButton2_clicked();

private:
    Ui::RuleSettingWidget *ui;
    int writeXML(QString write_filename);

};

#endif// RULESETTINGWIDGET_H
