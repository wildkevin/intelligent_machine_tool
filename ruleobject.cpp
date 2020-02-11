#include "ruleobject.h"
#include <iostream>
#include <QXmlStreamWriter>
#include <QMap>

using namespace std;

RuleLine::RuleLine(QObject *parent) : QObject(parent){
    this->OptionMap = new QMap<QString,QString>;
}
RuleLine::~RuleLine(){
    delete OptionMap;
}

RuleObject::RuleObject(QObject *parent) : QObject(parent)
{
    this->lines = new QList<RuleLine*>;
}

RuleObject::~RuleObject(){
    delete lines;
}

void RuleObject::setIndex(int index){
    this->index = index;
}

int RuleObject::getIndex(){
    return this->index;
}

void RuleObject::SetLabel(QString *label){
    this->label = label;
}

QString* RuleObject::getLabel(){
    return this->label;
}
QList<RuleLine*>* RuleObject::getLines(){
    return this->lines;
}

void RuleObject::addLine(RuleLine *line){
    this->lines->append(line);
}

void RuleObject::SetLen(int len){
    this->len = len;
}

int RuleObject::GetLen(){
    return this->len;
}
