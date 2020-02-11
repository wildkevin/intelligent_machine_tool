#include "rulelistwidget.h"
#include "ui_rulelistwidget.h"
#include <QPushButton>
#include <iostream>
using namespace std;
RuleListWidget::RuleListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleListWidget)
{
    ui->setupUi(this);
    init();
}

RuleListWidget::~RuleListWidget()
{
    delete ui;
}

void RuleListWidget::init(){
    this->scrollAreaLayout = new QVBoxLayout(this);
//    this->scrollAreaLayout->setSizeConstraint(QVBoxLayout::SetMinAndMaxSize);
    scrollAreaLayout->setSpacing(2);
    QWidget* widget = new QWidget(this);
//    widget->setMinimumSize(800, 1000);
//    widget->setMaximumSize(800,1000);


    widget->setLayout(this->scrollAreaLayout);
    ui->scrollArea->setWidget(widget);
    this->ruleWidgetList = new QList<RuleWidget*>();
}

void RuleListWidget::AddRuleWidget(int index,RuleObject *rObject){
    RuleWidget *widget = new RuleWidget(ui->scrollArea,rObject);
    QString* name = new QString("DataRuleWidget");
    name->append(QString::number(count++));
    widget->name = name;
    widget->index = index;
    widget->setStyleSheet("DataRuleLineWidget*#ui{border-left:4px solid rgb(0,0,0);background-color: rgb(255, 255, 255);}");
    widget->setMinimumSize(widget->size());
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(1);
    sizePolicy.setVerticalStretch(1);
    widget->setSizePolicy(sizePolicy);

    this->scrollAreaLayout->insertWidget(index,widget); //原来是append
    this->ruleWidgetList->insert(index,widget); //原来是append

    connect(widget, SIGNAL(btn_remove_clicked(int)), this, SLOT(on_ruleWidget_remove(int)));
    connect(widget, SIGNAL(btn_create_clicked(int)), this, SLOT(on_ruleWidget_create(int)));

}

void RuleListWidget::AddRuleObject(int index)
{
    ruleObjects->insert(index,new RuleObject);

}

void RuleListWidget::RemoveRuleWidget(int index)
{
    this->scrollAreaLayout->removeWidget(ruleWidgetList->at(index));
    delete ruleWidgetList->at(index);
    this->ruleObjects->removeAt(index);
    this->ruleWidgetList->removeAt(index);
}

void RuleListWidget::IndexUpdate()
{
    for (int i = 0; i < ruleObjects->size();i++)
    {
        ruleObjects->at(i)->setIndex(i);
    }
    for (int i = 0; i < ruleWidgetList->size();i++)
    {
        ruleWidgetList->at(i)->index = i;
    }
}

void RuleListWidget::on_ruleWidget_remove(int index){
    RemoveRuleWidget(index);
    IndexUpdate();
    cout << "index is " << index << endl;
    cout << "list is " << ruleWidgetList->size() << endl;
    cout << "object is " << ruleObjects->size() << endl;

}

void RuleListWidget::on_ruleWidget_create(int index)
{
    AddRuleWidget(index);
    AddRuleObject(index);
    IndexUpdate();
    cout << "index is " << index << endl;
    cout << "list is " << ruleWidgetList->size() << endl;
    cout << "object is " << ruleObjects->size() << endl;
}

void RuleListWidget::BuildFromRuleObjects(){

    for (int i = 0; i < ruleWidgetList->size(); i++)
    {
        this->scrollAreaLayout->removeWidget(ruleWidgetList->at(i));
        delete ruleWidgetList->at(i);
    }

    ruleWidgetList->clear();

    for(int i=0;i<this->ruleObjects->size();i++){
        AddRuleWidget(i, this->ruleObjects->at(i));
    }
    scrollAreaLayout->addStretch(10);

    //给控件赋值
    for(int i=0;i<this->ruleObjects->size();i++){
        RuleObject *rObj = this->ruleObjects->at(i);
        RuleWidget *widget = this->ruleWidgetList->at(i);
    }
}

void RuleListWidget::SetRuleObjects(QList<RuleObject *> *objects){
    this->ruleObjects = objects;
}

QList<RuleObject*>* RuleListWidget::GetRuleObjects(){
    this->ruleObjects->clear();
    foreach (RuleWidget* widget, *(this->ruleWidgetList)) {
        widget->ReSaveToRuleObject();
        RuleObject* obj = widget->GetRuleObject();
        this->ruleObjects->append(obj);
    }
    return this->ruleObjects;
}
