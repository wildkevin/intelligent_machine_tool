#include "rulewidget.h"
#include "ui_rulewidget.h"
#include <iostream>
#include "tablelinepushbutton.h"
#include "datarulelinewidgetitem.h"
#include "QComboBox"

using namespace std;

RuleWidget::RuleWidget(QWidget *parent,RuleObject *rObject) :
    QWidget(parent), ui(new Ui::RuleWidget)
{
    ui->setupUi(this);
    this->ruleObject = rObject;
    this->initUi();
    this->showTableWidget(true);
}

RuleWidget::~RuleWidget()
{
    delete ui;
    delete this->ruleObject;
}

void RuleWidget::showTableWidget(bool show){
    ui->tableGroupBox->setHidden(!show);
}

void RuleWidget::on_lineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
//    reSaveToRuleObject();
}

void RuleWidget::on_tableWidget_cellChanged(int row, int column)
{
    Q_UNUSED(row)
    Q_UNUSED(column)
//    reSaveToRuleObject();
    return;
}

void RuleWidget::initUi(){
    connect(this,SIGNAL(new_rule_line_added(int,QString)),this,SLOT(on_new_rule_line_added(int,QString)));
    build();
}

void RuleWidget::build(){
   QString *lblTxt = this->ruleObject->getLabel();
   if(lblTxt != Q_NULLPTR){
       ui->labelEdit->setText(*lblTxt);
   }
   ui->spinBox_len->setValue(this->ruleObject->GetLen());
   if(this->ruleObject->getLines()->isEmpty()){
       addRow(0);//add first new row
   }else{
       for(int i=0;i<this->ruleObject->getLines()->size();i++){
            addRow(i);
       }
   }
}

void RuleWidget::addRow(int row){
    ui->tableWidget->insertRow(row);

    RuleLine* rl = 0;
    if(row >=0 && this->ruleObject->getLines()->size() > row){
        rl = this->ruleObject->getLines()->at(row);
    }else{
        rl = new RuleLine;
        rl->name = new QString(QString("ruleLine_").append(QString::number(row)));
        this->ruleObject->addLine(rl);
    }

    QTableWidgetItem *cell_0_0  = new QTableWidgetItem(QString::number(row));
    QTableWidgetItem *cell_0_1  = new QTableWidgetItem(QString::number(8));
    QString suffix = rl->suffix;

    QTableWidgetItem *cell_0_2  = new QTableWidgetItem(suffix);

    //值选项集
    QString optionsStr;
    if(!rl->OptionMap->isEmpty()){
        QMap<QString,QString>::iterator it = rl->OptionMap->begin(); //遍历map
        for(; it != rl->OptionMap->end(); ++it){
            optionsStr.append(it.key()).append(":").append(it.value()).append(";");
        }
    }
    QTableWidgetItem *cell_0_3  = new QTableWidgetItem(optionsStr.isEmpty()?QString(""):QString(optionsStr.toLocal8Bit()));
    QTableWidgetItem *cell_0_4  = new QTableWidgetItem(rl->defaultVal.isEmpty()?QString(""):QString(rl->defaultVal.toLocal8Bit()));

    QComboBox *comboxDatasystem = new QComboBox;
    comboxDatasystem->insertItem(0,QString::fromLocal8Bit("10进制"),DEC);
    comboxDatasystem->insertItem(1,QString::fromLocal8Bit("16进制"),HEX);
//    comboxDatasystem->insertItem(2,QString::fromLocal8Bit("ASCII"),ASC);
    comboxDatasystem->insertItem(2,QString::fromLocal8Bit("2进制"),BIN);
    switch(rl->rDataSystem){
    case DEC:
        comboxDatasystem->setCurrentIndex(0);
        break;
    case HEX:
        comboxDatasystem->setCurrentIndex(1);
        break;
    case ASC:
        comboxDatasystem->setCurrentIndex(2);
        break;
    case BIN:
        comboxDatasystem->setCurrentIndex(3);
        break;
    }

    QComboBox *comboxValuesystem = new QComboBox;
    comboxValuesystem->insertItem(0,QString::fromLocal8Bit("10进制"),DEC);
    comboxValuesystem->insertItem(1,QString::fromLocal8Bit("16进制"),HEX);
    comboxValuesystem->insertItem(2,QString::fromLocal8Bit("ASCII"),ASC);
    comboxValuesystem->insertItem(3,QString::fromLocal8Bit("2进制"),BIN);
//    QTableWidgetItem *cell_0_5  = new QTableWidgetItem();
    switch(rl->ValueSystem){
    case DEC:
        comboxValuesystem->setCurrentIndex(0);
        break;
    case HEX:
        comboxValuesystem->setCurrentIndex(1);
        break;
    case ASC:
        comboxValuesystem->setCurrentIndex(2);
        break;
    case BIN:
        comboxValuesystem->setCurrentIndex(3);
        break;
    }



    ui->tableWidget->setItem(row,0,cell_0_0);
    ui->tableWidget->setItem(row,1,cell_0_1);
    ui->tableWidget->setItem(row,2,cell_0_2);
    ui->tableWidget->setItem(row,3,cell_0_3);
    ui->tableWidget->setItem(row,4,cell_0_4);
    ui->tableWidget->setCellWidget(row,5,comboxDatasystem);
    ui->tableWidget->setCellWidget(row,6,comboxValuesystem);
//    ui->tableWidget->setItem(row,5,cell_0_5);

    //向table的最后一列插入按钮
    QWidget *buttonWidget = new QWidget();
    TableLinePushButton* btn_insert = new TableLinePushButton();
    btn_insert->row = row;
    btn_insert->setText(QString::fromLocal8Bit("插入一行").append(QString::number(row)));
    QHBoxLayout* pLayout = new QHBoxLayout(buttonWidget);
    pLayout->addWidget(btn_insert);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0, 0, 0, 0);
    buttonWidget->setLayout(pLayout);
    ui->tableWidget->setCellWidget(row, 7, buttonWidget);

    connect(btn_insert,SIGNAL(clicked(bool)),this,SLOT(on_btn_insert_click()));
}

void RuleWidget::on_new_rule_line_added(int row, QString text){
    addRow(row+1);
    cout<<row<<text.toLocal8Bit().toStdString()<<endl;
}
RuleObject* RuleWidget::GetRuleObject(){
    return this->ruleObject;
}

void RuleWidget::on_btn_insert_click(){
    TableLinePushButton *btn_insert=qobject_cast<TableLinePushButton*>(sender());//使用Qt的类型转换，将指针恢复为QPushButton类型
    cout<<btn_insert->text().toLocal8Bit().toStdString()<<endl;
    emit new_rule_line_added(btn_insert->row,btn_insert->text());
}
/**
 * @brief RuleWidget::refreshToRuleObject
 * 将界面中的规则表格中的行 存为ruleline对象
 */
void RuleWidget::ReSaveToRuleObject(){
    //读主信息
    QString lineEditContent =ui->labelEdit->text();//标签
    QString *labelTxt = new QString;
    *labelTxt = lineEditContent;
    this->ruleObject->SetLabel(labelTxt);
    this->ruleObject->setIndex(this->index);



    //遍历行信息
    int rowcount = ui->tableWidget->rowCount();
    for(int r=0;r<rowcount;r++){
        QTableWidgetItem *cellitem_0 = ui->tableWidget->item(r,0);//起始位置
        QTableWidgetItem *cellitem_1 = ui->tableWidget->item(r,1);//长度
        QTableWidgetItem *cellitem_2 = ui->tableWidget->item(r,2);//后缀
        QTableWidgetItem *cellitem_3 = ui->tableWidget->item(r,3);//值选项集
        QTableWidgetItem *cellitem_4 = ui->tableWidget->item(r,4);//默认值
        QComboBox *cellitem_5 = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(r,5));//解析进制
        QComboBox *cellitem_6 = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(r,6));//结果进制

//begin len suffix options default datasys valuesys

        RuleLine* rl = this->GetRuleObject()->getLines()->at(r);

        QString beginIndex,lenStr,suffix;
        if(cellitem_0 != Q_NULLPTR){
            beginIndex = cellitem_0->text();
            int beginIndexInt = beginIndex.toInt();
            rl->begin = beginIndexInt;
            cout<<"beginIndexInt:"<<beginIndexInt;

        }
        if(cellitem_1 != Q_NULLPTR){
            lenStr = cellitem_1->text();
            int lenInt = lenStr.toInt();
            rl->length = lenInt;
            cout<<"lenInt:"<<lenInt;
        }
        if(cellitem_2 != Q_NULLPTR){
            suffix = cellitem_2->text();
            rl->suffix = suffix;
        }

        if(cellitem_3 != Q_NULLPTR){
            //值为预定义选项集
            rl->OptionMap->clear();
            QString optionsStr = cellitem_3->text();
            optionsStr.replace("\n","");
            if(!optionsStr.isEmpty() && optionsStr.contains(":")){
                QStringList optionList = optionsStr.split(";",QString::SkipEmptyParts);//拆分并跳过空值
                for(int i=0;i<optionList.size();i++){
                    QString s = optionList.at(i);
                    if(s.contains(":")){
                        QStringList ss = s.split(":",QString::SkipEmptyParts);
                        QString k = ss.at(0);
                        QString v = ss.at(1);
                        rl->OptionMap->insert(k,v);
                    }
                }
            }
        }

        if(cellitem_4 != Q_NULLPTR){
            QString defValStr = cellitem_4->text();

            if(!defValStr.isEmpty()){
                rl->defaultVal = defValStr;
            }
        }


        if(cellitem_5 != Q_NULLPTR){
            QVariant qv = cellitem_5->itemData(cellitem_5->currentIndex());
            RuleDataSystem ds = static_cast<RuleDataSystem>(qv.toInt());
            rl->rDataSystem = ds;
            cout<<"1111"<<endl;
//            cout<<cellitem_5->text().toStdString()<<endl;
        }

        if(cellitem_6 != Q_NULLPTR){
            QVariant qv = cellitem_6->itemData(cellitem_6->currentIndex());
            RuleDataSystem vs = static_cast<RuleDataSystem>(qv.toInt());
            rl->ValueSystem = vs;
            cout<<"2222"<<endl;
        }


//        this->ruleObject->addLine(rl);
        //cout<<beginIndex.toStdString()<<" "<<lenStr.toStdString()<<" "<<suffix.toLocal8Bit().data()<<" "<<endl;
    }
}


void RuleWidget::on_destoryButton_clicked()
{
    emit btn_remove_clicked(this->index);
}

void RuleWidget::on_createButton_clicked()
{
    emit btn_create_clicked(this->index);
}
