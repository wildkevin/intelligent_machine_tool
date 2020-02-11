#ifndef INPUTITEM_H
#define INPUTITEM_H

#include <QWidget>
#include <QLineEdit>
#include <ruleobject.h>
#include <QTimer>

namespace Ui {
class InputItem;
}

class InputItem : public QWidget
{
    Q_OBJECT

public:
    explicit InputItem(RuleObject *rObject, QWidget *parent = 0);
    ~InputItem();
    RuleObject *rObject;
    QString* getHexString();
    //设置从串口返回的值
    void SetReadValue(QString readValue);
    QList<QTimer *> *TimerList;
    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::InputItem *ui;
    QList<QWidget*>* inputs;
    QList<QWidget*>* read_inputs;
    void init();

private slots:
    void updateTimeFieldText(QWidget *widget, QString expr);

};

#endif // INPUTITEM_H
