#include "inputitem.h"
#include "ui_inputitem.h"
#include <ruleobject.h>
#include <iostream>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <util.h>
#include <QTimer>
#include <QDateTime>

using namespace std;

InputItem::InputItem(RuleObject *rObject, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputItem)
{
    ui->setupUi(this);
    this->rObject = rObject;
    this->inputs = new QList<QWidget*>;
    this->read_inputs = new QList<QWidget*>;
    this->TimerList = new QList<QTimer*>;
    init();
}

InputItem::~InputItem()
{
    delete ui;
}

void InputItem::init(){
    ui->gridLayoutWidget->setFixedHeight(44);
    ui->gridLayout->setMargin(1);
    //    ui->gridLayoutWidget->setStyleSheet("background:white;");

    QWidget *contentArea = new QWidget(ui->gridLayoutWidget);
    contentArea->setLayout(ui->gridLayout);

    QString *lbl = this->rObject->getLabel();

    QLabel *qLbl = new QLabel();
    qLbl->setFixedHeight(36);
    qLbl->setMinimumWidth(140);
    qLbl->setAlignment(Qt::AlignRight);
    //    qLbl->setStyleSheet("background:blue;");
    qLbl->setText(*lbl);
    //    ui->gridLayout->setRowStretch(0,0.9);
    ui->gridLayout->addWidget(qLbl,0,0);
    QList<RuleLine*> *rLines = this->rObject->getLines();

    for(int i=0;i<rLines->size();i++){
        RuleLine *rl = rLines->at(i);

        QMap<QString,QString> *opMap = rl->OptionMap;
        QString defaultVal = rl->defaultVal;
        bool isTimeVar = false;//是否时间变量
        QString timeExpr;
        //表达式
        if(!defaultVal.isEmpty() && defaultVal.startsWith("${") && defaultVal.endsWith("}")){
            //QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            timeExpr = defaultVal.mid(defaultVal.indexOf("${")+2,defaultVal.indexOf("}")-2);
            if(timeExpr == "yyyy" || timeExpr == "MM" || timeExpr == "dd" || timeExpr == "hh" || timeExpr == "mm" || timeExpr == "ss"){
                isTimeVar = true;
            }
        }
        QString suff = rl->suffix;

        if(!suff.isEmpty()){
            //有后缀的 需要组合两个widget  lineedit/combobox+label
            //组合
            QWidget *widget1 = new QWidget(contentArea);

            widget1->setContentsMargins(0,0,0,0);
            widget1->setMinimumWidth(600);
            widget1->setFixedHeight(40);
            //            widget1->setStyleSheet("background:yellow;");
            QHBoxLayout *layout1 = new QHBoxLayout(contentArea);
            layout1->setMargin(0);
            widget1->setLayout(layout1);

            if(!opMap->isEmpty()){
                //设置了值选项集的 需要显示为下拉框
                QComboBox *cBox = new QComboBox;
                cBox->setMinimumWidth(150);
                cBox->setFocusPolicy(Qt::NoFocus);
                cBox->installEventFilter(this);

                QMap<QString,QString>::iterator it = opMap->begin(); //遍历map

                for(int index = 0; it != opMap->end(); ++it,index++){
                    cBox->insertItem(index, it.key(), it.value());
                    if(it.value() == defaultVal){
                        cBox->setCurrentIndex(index);
                    }
                }


                layout1->addWidget(cBox);
                this->inputs->append(cBox);

                QLabel *lblSuff = new QLabel(widget1);
                lblSuff->setFixedHeight(36);
                lblSuff->setMinimumWidth(150);
                lblSuff->setText(suff);
                layout1->addWidget(lblSuff);
                ui->gridLayout->addWidget(widget1,0,(i+1),Qt::AlignLeft);


                //read value widget
                QComboBox *cBox_read = new QComboBox;
                cBox_read->setMinimumWidth(150);
                cBox_read->setFocusPolicy(Qt::NoFocus);
                cBox_read->installEventFilter(this);

                QMap<QString,QString>::iterator it2 = opMap->begin(); //遍历map

                cBox_read->insertItem(0, QString::fromLocal8Bit("未读取"), "");
                for(int index = 1; it2 != opMap->end(); ++it2,index++){
                    cBox_read->insertItem(index, it2.key(), it2.value());
                    //                    if(it2.value() == defaultVal){
                    //                        cBox_read->setCurrentIndex(index);
                    //                    }
                }


                layout1->addWidget(cBox_read);
                this->read_inputs->append(cBox_read);

                QLabel *lblSuff_2 = new QLabel(widget1);
                lblSuff_2->setMinimumWidth(150);
                lblSuff_2->setFixedHeight(36);
                lblSuff_2->setText(suff);
                layout1->addWidget(lblSuff_2);
                ui->gridLayout->addWidget(widget1,0,(i+1),Qt::AlignLeft);



            }else{
                //没有设置值选项集的 显示为lineedit
                QLineEdit *lEdit = new QLineEdit(widget1);
                lEdit->setMinimumWidth(150);
                lEdit->setFixedHeight(36);
                lEdit->setText(defaultVal);


                if(isTimeVar){
                    //时间变量实时刷新
                    QTimer *timer = new QTimer(this);
                    defaultVal = QDateTime::currentDateTime().toString(timeExpr);
                    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeFieldText(lEdit,defaultVal)));
                    timer->start(1000);
                    this->TimerList->append(timer);
                }

                //            lEdit->setStyleSheet("background:green;");
                layout1->addWidget(lEdit);
                this->inputs->append(lEdit);

                QLabel *lblSuff = new QLabel(widget1);
                lblSuff->setMinimumWidth(150);
                lblSuff->setFixedHeight(36);
                lblSuff->setText(suff);
                layout1->addWidget(lblSuff);
                ui->gridLayout->addWidget(widget1,0,(i+1),Qt::AlignLeft);



                //read value widget
                QLineEdit *lEdit_read = new QLineEdit(widget1);
                lEdit_read->setMinimumWidth(150);
                lEdit_read->setFixedHeight(36);
                //                lEdit_read->setText(defaultVal);
                //            lEdit->setStyleSheet("background:green;");
                layout1->addWidget(lEdit_read);
                this->read_inputs->append(lEdit_read);

                QLabel *lblSuff_2 = new QLabel(widget1);
                lblSuff_2->setMinimumWidth(150);
                lblSuff_2->setFixedHeight(36);
                lblSuff_2->setText(suff);
                layout1->addWidget(lblSuff_2);
                ui->gridLayout->addWidget(widget1,0,(i+1),Qt::AlignLeft);
            }



        }else{
            //无后缀
            if(!opMap->isEmpty()){
                QComboBox *cBox = new QComboBox;
                cBox->setMinimumWidth(300);
                cBox->setFocusPolicy(Qt::NoFocus);
                cBox->installEventFilter(this);
                QMap<QString,QString>::iterator it = opMap->begin(); //遍历map
                for(int index = 0; it != opMap->end(); ++it,index++){
                    cBox->insertItem(index, it.key(), it.value());
                    if(it.value() == defaultVal){
                        cBox->setCurrentIndex(index);
                    }
                }
                ui->gridLayout->addWidget(cBox,0,(i+1),Qt::AlignLeft);
                this->inputs->append(cBox);


                //read value widget
                QComboBox *cBox_read = new QComboBox;
                cBox_read->setMinimumWidth(300);
                cBox_read->setFocusPolicy(Qt::NoFocus);
                cBox_read->installEventFilter(this);

                QMap<QString,QString>::iterator it2 = opMap->begin(); //遍历map
                cBox_read->insertItem(0, QString::fromLocal8Bit("未读取"), "");
                for(int index = 1; it2 != opMap->end(); ++it2,index++){
                    cBox_read->insertItem(index, it2.key(), it2.value());
                    //                    if(it2.value() == defaultVal){
                    //                        cBox_read->setCurrentIndex(index);
                    //                    }
                }
                ui->gridLayout->addWidget(cBox_read,0,(i+2),Qt::AlignLeft);
                this->read_inputs->append(cBox_read);
            }else{
                QLineEdit *lEdit = new QLineEdit(contentArea);
                lEdit->setMinimumWidth(300);
                lEdit->setFixedHeight(38);
                //            lEdit->setStyleSheet("background:green;");
                lEdit->setText(defaultVal);

                if(isTimeVar){
                    //时间变量实时刷新
                    QTimer *timer = new QTimer(this);
                    connect(timer, &QTimer::timeout, [lEdit,timeExpr](){
                        //                        this->updateTimeFieldText(lEdit, timeExpr);
                        QString text = QDateTime::currentDateTime().toString(timeExpr);
                        lEdit->setText(text);
                        //                        cout<<" update text : "<<text.toStdString()<<endl;
                    });
                    timer->start(1000);
                    this->TimerList->append(timer);
                }
                ui->gridLayout->addWidget(lEdit,0,(i+1),Qt::AlignLeft);
                this->inputs->append(lEdit);

                //read value widget
                QLineEdit *lEdit_read = new QLineEdit(contentArea);
                lEdit_read->setMinimumWidth(300);
                lEdit_read->setFixedHeight(38);
                //            lEdit->setStyleSheet("background:green;");
                //                lEdit_read->setText(defaultVal);
                ui->gridLayout->addWidget(lEdit_read,0,(i+2),Qt::AlignLeft);
                this->read_inputs->append(lEdit_read);
            }
        }
    }
}


//过滤combox的滚轮事件，防止误选
bool InputItem::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::Wheel)
    {
        QComboBox* combo = qobject_cast<QComboBox*>(obj);
        if(combo && !combo->hasFocus()){
            return true;
        }
    }

    return false;
}

void InputItem::updateTimeFieldText(QWidget *widget, QString expr){
    if(widget->metaObject()->className() == QStringLiteral("QLineEdit")){
        QLineEdit *edit = qobject_cast<QLineEdit*>(widget);
        QString text = QDateTime::currentDateTime().toString(expr);
        edit->setText(text);
    }
}

void InputItem::SetReadValue(QString readValue){
    for(int i=0;i<this->read_inputs->size();i++){
        QWidget *w = this->read_inputs->at(i);
        if (w->metaObject()->className() == QStringLiteral("QLineEdit")){
            QLineEdit *lEdit = qobject_cast<QLineEdit*>(w);
            lEdit->setText(readValue);
        }else if(w->metaObject()->className() == QStringLiteral("QComboBox")){
            QComboBox *cBox = qobject_cast<QComboBox*>(w);
            for(int j=0;j<cBox->count();j++){
                QVariant qv = cBox->itemData(j);
                if(qv.toString().toLower() == readValue.toLower()){
                    cBox->setCurrentIndex(j);
                }
            }
        }

        QWidget *w2 = this->inputs->at(i);
        bool valEq = false;
        if (w2->metaObject()->className() == QStringLiteral("QLineEdit")){
            QLineEdit *lEdit = qobject_cast<QLineEdit*>(w2);
            QString valueSetting = lEdit->text();
            valEq = (valueSetting.toLower() == readValue.toLower());
            cout<<" setting value: "<<valueSetting.toStdString()<<"  read value:"<<readValue.toStdString()<<endl;
        }else if(w2->metaObject()->className() == QStringLiteral("QComboBox")){
            QComboBox *cBox = qobject_cast<QComboBox*>(w2);
            QVariant qv = cBox->itemData(cBox->currentIndex());
            valEq = (qv.toString().toLower() == readValue.toLower());
            cout<<" setting value: "<<qv.toString().toStdString()<<"  read value:"<<readValue.toStdString()<<endl;
        }

        if(!valEq){
            ui->gridLayoutWidget->setStyleSheet("background:red;");
        }
    }
}

QString* InputItem::getHexString(){
    //根据ruleline list中的规则，将数据反转译成16进制字符串
    QList<RuleLine*> *rLines = this->rObject->getLines();
    QString *result = new QString;
    for(int i=0;i<rLines->size();i++){
        RuleLine *rL = rLines->at(i);
        QWidget *qW = this->inputs->at(i);
        QString txt;

        if (qW->metaObject()->className() == QStringLiteral("QLineEdit")){
            QLineEdit *lEdit = qobject_cast<QLineEdit*>(qW);
            txt = lEdit->text();
        }else if(qW->metaObject()->className() == QStringLiteral("QComboBox")){
            QComboBox *cBox = qobject_cast<QComboBox*>(qW);
            QVariant qv = cBox->itemData(cBox->currentIndex());
            txt = qv.toString();
        }

        cout<<"txt=="<<txt.toStdString()<<endl;

        switch (rL->ValueSystem) {
        case BIN:

            break;
        case DEC:
        {
            txt = txt.replace(" ","");
            switch (rL->rDataSystem) {
            case DEC:
                cout<<"dec to dec"<<endl;
                break;
            case HEX:
            {
                int decVal = txt.toInt();
                cout<<"txt = "<<txt.toStdString();

                txt = QString("%L1").arg(decVal,rL->length*2,16,QLatin1Char('0'));
                cout<<",txt after = "<<txt.toStdString()<<" rL.length="<<QString::number(rL->length).toStdString()<<endl;
                result->append(txt.toUpper());
            }
                break;
            case BIN:
                cout<<"dec to bin"<<endl;
                break;
            default:
                cout<<" unknown ruledatasystem"<<endl;
                break;
            }
        }
            break;
        case HEX:
        {
            txt = txt.replace(QRegularExpression("\\s+"),"");
            switch (rL->rDataSystem) {
            case DEC:
                cout<<"hex to dec"<<endl;
                break;
            case HEX:
            {
                //                bool ok = false;
                //                cout<<"txt = "<<txt.toStdString();
                //                int decVal = txt.toInt(&ok,16);

                //                txt = QString("%L1").arg(txt.,rL->length*2,16,QLatin1Char('0'));

                QString tmp;
                //长度补0
                for(int i=0;i<(rL->length*2-txt.length());i++){
                    tmp.append("0");
                }
                txt = tmp.append(txt);
                cout<<",txt after = "<<txt.toStdString()<<" rL.length="<<QString::number(rL->length).toStdString()<<endl;
                result->append(txt.toUpper());
            }
                break;
            case BIN:
                break;
            default:
                cout<<" unknown ruledatasystem"<<endl;
                break;
            }
        }
            break;
        case ASC:
        {
            //一个Ascii码是一个字节的16进制
            QString tmp;
            for(int i=0;i<txt.size();i++){
                QChar ch = txt.at(i);

                QString hex = QString("%L1").arg(ch.unicode(),2,16,QLatin1Char('0'));
                tmp.append(hex);
            }
            for(int i=0;i<(rL->length*2-tmp.size());i++){
                tmp = QString("0").append(tmp);
            }
            result->append(tmp.toUpper());
        }
            break;
        default:
            break;
        }
    }
    return result;
}
