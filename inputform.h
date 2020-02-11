#ifndef INPUTFORM_H
#define INPUTFORM_H

#include <QWidget>
#include <ruleobject.h>
#include <QGridLayout>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <inputitem.h>
#include <mainwindow.h>
#include "console.h"
#include <QTimer>
namespace Ui {
class InputForm;
}
enum ComState{
    CLOSE,OPEN
};

class InputForm : public QWidget
{
    Q_OBJECT

public:
    QGridLayout *gridLayout;
    QString length;
    QString beginFlag = QString("FFFF");
    QString command = QString("97");
    QString seq = QString("01");
    QString flags = QString("0000");
    QString chk;
    Console *m_console = nullptr;
    explicit InputForm(QWidget *parent = 0);
    ~InputForm();
    void buildFormWidget();
    void reBuildFormWidget();
    static ComState returnComState();


private slots:
    void readData();
    void on_btn_write_clicked();

    void on_btn_read_clicked();

    void on_comSelect_currentTextChanged(const QString &arg1);
    void timeUpdate();

    void on_btnComState_clicked();

    private:
    Ui::InputForm *ui;
    QSerialPort *com = nullptr;
    static ComState comState;
    QList<RuleObject*>* ruleObjects;
    QList<InputItem*>* inputItems;
    QByteArray buf;
    QTimer *timer = nullptr;

    bool readRuleConfig();
    bool closeCom();
    bool openCom();
    void buildLeftFormPart();
    void buildRightFormPart();
    void deParseReadData(QString data);
};

#endif // INPUTFORM_H
