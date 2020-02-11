#include "rulesettingwidget.h"
#include "ui_rulesettingwidget.h"
#include "iostream"
#include "datarulemodel.h"
#include "rulewidget.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QTextStream>
#include <mainwindow.h>
#include <util.h>
#include <QFileDialog>
#include <QMessageBox>

using namespace std;

RuleSettingWidget::RuleSettingWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::RuleSettingWidget)
{
    ui->setupUi(this);
    readXML("rulesetting.xml");
}

RuleSettingWidget::~RuleSettingWidget()
{
    delete ui;
}

void RuleSettingWidget::on_buttonAddCom_clicked()
{
    QString comText = this->ui->lineEdit->text();
    int num = this->ui->spinBox->value();
    QString a = QString::number(num);
    const QString& comItem = comText.append(a);

    bool isExists = false;
    for(int i =0; i<ui->listWidget->count(); i++)//遍历所算的ITEM
    {
        QListWidgetItem* sel = ui->listWidget->item(i);
        if(sel->text() == comItem){
            cout<<"已存在"<<comItem.toStdString()<<endl;
            isExists = true;
            break;
        }
    }

    if(!isExists){
        this->ui->listWidget->addItem(comItem);
        cout<<comItem.toStdString()<<endl;
    }
}

void RuleSettingWidget::on_buttonRemoveCom_clicked()
{
    cout<<"i am here !!"<<endl;
    if(ui->listWidget->currentItem()!=Q_NULLPTR)
    {
        QListWidgetItem * item = ui->listWidget->takeItem(ui->listWidget->currentRow());
        delete item;
    }
}


//移除串口列表中的串口
void RuleSettingWidget::on_listWidget_currentTextChanged(const QString &currentText)
{
    if(currentText==NULL || currentText==""){
        return;
    }
}

void RuleSettingWidget::readXML(QString read_filename){
    QString filename = read_filename;
    QFile* file = new QFile(filename);
    ConfigUtil util;
    QList<RuleObject*>* ruleObjects = util.ParseXMLConfig(file);
    int count = ruleObjects->size();
    ui->dataLenSpin->setValue(count);
    ui->ruleListWidget->SetRuleObjects(ruleObjects);
    ui->ruleListWidget->BuildFromRuleObjects();
}

void RuleSettingWidget::on_addButton2_clicked()
{
    const QString& batText = ui->lineEdit2->text();
    const QString& pattr = QString("^[1-9][0-9]*$");//正则表达式的意思是全数字 且开头不能是0
    const QRegularExpression& qRegExp = QRegularExpression(pattr);

    if(batText!=Q_NULLPTR && batText.trimmed().length()>0){

        if(!batText.contains(qRegExp)){
            cout<<"包含无效数字"<<endl;
            return;
        }


        bool isExists = false;
        for(int i =0; i<ui->listWidget2->count(); i++)//遍历所算的ITEM
        {
            QListWidgetItem* sel = ui->listWidget2->item(i);
            if(sel->text() == batText){
                cout<<"已存在"<<batText.toStdString()<<endl;
                isExists = true;
                break;
            }
        }

        if(!isExists){
            ui->listWidget2->addItem(batText);
            cout<<batText.toStdString()<<endl;
        }
    }
}

void RuleSettingWidget::on_removeButton2_clicked()
{
    cout<<"i am there !!"<<endl;
    if(ui->listWidget2->currentItem()!=Q_NULLPTR)
    {
        QListWidgetItem * item = ui->listWidget2->takeItem(ui->listWidget2->currentRow());
        delete item;
    }
}

void RuleSettingWidget::on_btn_do_clicked()
{
    const int len = ui->dataLenSpin->value();

    //    ui->gridLayout->children().clear();//先清空gridlayout再重新添加
    int ruleListSize = this->ui->ruleListWidget->GetRuleObjects()->size();
    for(int r=ruleListSize;r<(ruleListSize+len);r++){
        ui->ruleListWidget->AddRuleWidget(r);
        ui->ruleListWidget->AddRuleObject(r);
    }
}

void RuleSettingWidget::on_pushButton_clicked()
{
    if (!MainWindow::GetFileName().isEmpty())
    {
        writeXML(MainWindow::GetFileName());
        QMessageBox::about(this, "",QString::fromLocal8Bit("保存成功"));
        return;
    }
    QString filename;
    filename = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存配置文件"), "", tr("Xml Files (*.xml)"));
    writeXML(filename);
    QMessageBox::about(this, "",QString::fromLocal8Bit("保存成功"));
}

int RuleSettingWidget::writeXML(QString write_filename)
{
    QFile file(write_filename);

    QList<RuleObject*>* ruleObjects = this->ui->ruleListWidget->GetRuleObjects();

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("Root");
        writer.writeTextElement("Header","FFFF");

        writer.writeStartElement("Length");
        writer.writeAttribute("exp","count(Command)+count(Seq)+count(Flags)+count(Data)+count(Chk)");
        writer.writeEndElement();

        writer.writeTextElement("Command","97");
        writer.writeTextElement("Seq","01");
        writer.writeTextElement("Flags","0000");
        /*
         *     <Header>FFFF</Header>
         *     <Length exp="count(Command)+count(Seq)+count(Flags)+count(Data)+count(Chk)" />
         *     <Command>97</Command>
         *     <Seq>01</Seq>
         *     <Flags>0000</Flags>
         */

        writer.writeStartElement("Data");
        for(int i=0;i < ruleObjects->size();i++){
            RuleObject* rObject = ruleObjects->at(i);
            writer.writeStartElement("Pos");
            writer.writeAttribute("value",QString::number(rObject->getIndex()));//<Pos value="0">
            writer.writeAttribute("label",*(rObject->getLabel()));//<Pos value="0" label="">



            for(int i=0;i<rObject->getLines()->size();i++){
                RuleLine* rl = rObject->getLines()->at(i);
                writer.writeStartElement("Logic");

                writer.writeAttribute("begin",QString::number(rl->begin));
                writer.writeAttribute("len",QString::number(rl->length));

                if(!rl->suffix.isEmpty()){
                    writer.writeAttribute("suffix",rl->suffix);
                }

                if(!rl->OptionMap->isEmpty()){
                    QString optionStr;
                    QMap<QString,QString>::iterator it = rl->OptionMap->begin(); //遍历map
                    for(; it != rl->OptionMap->end(); ++it){
                        cout<<"key="<<it.key().toLocal8Bit().toStdString()<<" value="<<it.value().toStdString()<<endl;
                        optionStr.append(it.key()).append(":").append(it.value()).append(";");
                        //                    qDebug( "%s: %s", it.key().ascii(), it.data().ascii()); //用key()和data()分别获取“键”和“值”
                    }
                    writer.writeAttribute("options", optionStr);
                }

                if(!rl->defaultVal.isEmpty()){
                    writer.writeAttribute("defaultValue",rl->defaultVal);
                }


                switch (rl->rDataSystem) {
                case BIN:
                    writer.writeAttribute("datasys","bin");
                    break;
                case DEC:
                    writer.writeAttribute("datasys","dec");
                    break;
                case HEX:
                    writer.writeAttribute("datasys","hex");
                    break;
                case ASC:
                    writer.writeAttribute("datasys","asc");
                    break;
                }

                switch (rl->ValueSystem) {
                case BIN:
                    writer.writeAttribute("valuesys","bin");
                    break;
                case DEC:
                    writer.writeAttribute("valuesys","dec");
                    break;
                case HEX:
                    writer.writeAttribute("valuesys","hex");
                    break;
                case ASC:
                    writer.writeAttribute("valuesys","asc");
                    break;
                }


                writer.writeEndElement();//end </Logic>
            }

            writer.writeEndElement();//end </Pos>
        }

        writer.writeEndElement();//end </Data>
        /*
         * <Chk exp="(sum(Len)+sum(Command)+sum(Seq)+sum(Flags)+sum(Data))%256" />
         */
        writer.writeStartElement("Chk");
        writer.writeAttribute("exp","(sum(Len)+sum(Command)+sum(Seq)+sum(Flags)+sum(Data))%256");
        writer.writeEndElement();//end </Chk>

        writer.writeEndElement();//end </Root>
        writer.writeEndDocument();
    }


    file.close();
    return 0;
}
