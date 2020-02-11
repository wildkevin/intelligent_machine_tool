#ifndef RULELISTWIDGET_H
#define RULELISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <rulewidget.h>

namespace Ui {
class RuleListWidget;
}

class RuleListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RuleListWidget(QWidget *parent = 0);
    ~RuleListWidget();
    void AddRuleWidget(int index, RuleObject *rObject = new RuleObject);
    QVBoxLayout *scrollAreaLayout;
    QList<RuleObject *> *GetRuleObjects();
    void SetRuleObjects(QList<RuleObject *> *objects);
    void BuildFromRuleObjects();

    void RemoveRuleWidget(int index);
    void IndexUpdate();
    void AddRuleObject(int index);
private:
    Ui::RuleListWidget *ui;
    QList<RuleWidget*> *ruleWidgetList;
    QList<RuleObject*> *ruleObjects;
    int count;

    void init();

private slots:
    void on_ruleWidget_remove(int index);
    void on_ruleWidget_create(int index);
};

#endif // RULELISTWIDGET_H
