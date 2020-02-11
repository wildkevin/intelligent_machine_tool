#include "rulelistwidget.h"
#include "ui_rulelistwidget.h"

RuleListWidget::RuleListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleListWidget)
{
    ui->setupUi(this);
}

RuleListWidget::~RuleListWidget()
{
    delete ui;
}
