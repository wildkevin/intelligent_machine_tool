#ifndef DATARULELINEWIDGET_H
#define DATARULELINEWIDGET_H

#include <QWidget>
#include <ruleobject.h>

namespace Ui {
    class RuleWidget;
}

class RuleWidget : public QWidget
{
    Q_OBJECT
private:
    int linesGroupHeight = 100;
public:
    QString* name;
    int index;
    explicit RuleWidget(QWidget *parent = 0, RuleObject *rObject = new RuleObject);
    ~RuleWidget();
    void ReSaveToRuleObject();//将控件上的值转成ruleDataLine的属性值
private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_tableWidget_cellChanged(int row, int column);
    void on_btn_insert_click();
    void on_new_rule_line_added(int row, QString text);

    void on_destoryButton_clicked();

    void on_createButton_clicked();

signals:
    void new_rule_line_added(int row,QString text);
    void btn_remove_clicked(int index);
    void btn_create_clicked(int index);

private:
    Ui::RuleWidget *ui;
    RuleObject* ruleObject;
    void initUi();
    void build();
    void showTableWidget(bool show);

    void addRow(int row);//添加新行
    void removeRow(int row);//移除行

public:
    RuleObject* GetRuleObject();
};

#endif // DATARULELINEFORM_H
