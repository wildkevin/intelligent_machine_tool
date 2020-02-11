#include "inputform.h"
#include "ui_inputform.h"
#include <iostream>
#include <QFile>
#include <QXmlStreamReader>
#include <ruleobject.h>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QScrollArea>
#include <inputitem.h>
#include <util.h>
#include <qdir.h>
#include <QFileDialog>
#include <QTimer>

using namespace std;

ComState InputForm::comState = CLOSE;

InputForm::InputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputForm)
{
    ui->setupUi(this);
    this->ruleObjects = new QList<RuleObject*>;
    this->inputItems = new QList<InputItem*>;

    this->com = new QSerialPort;
    this->com->setReadBufferSize(1024);
    connect(this->com, SIGNAL(readyRead()), this, SLOT(readData()));

    this->timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));

    if(!MainWindow::GetFileName().isEmpty()){
        buildFormWidget();
    }else{

    }
}

InputForm::~InputForm()
{
    closeCom();
    delete this->com;
    delete ui;
}


/**
 * @brief InputForm::readData
 * 从串口中读取数据并在界面上显示，和输入值对比，如果值不一致 显示红色产背景
 */
void InputForm::readData(){
    buf.append(this->com->readAll());

    if(!timer->isActive()){
        timer->start(2000);//启动定时器，接收100毫秒数据（根据情况设定）
    }
}

void InputForm::timeUpdate(){
    timer->stop();
    if(!buf.isEmpty())
    {
        cout<<"  buf === "<<buf.toStdString()<<endl;

        QString show="";
        bool isHexR = true;
        if(isHexR)
        {
            for (int i = 0; i < buf.length(); i++)
            {
                qint8 outChar = buf[i];
                QString str = QString("%1").arg(outChar&0xFF, 2, 16, QLatin1Char('0'))+" ";
                show+= str.toUpper();
            }

        }

        ui->console->putData(show.toLocal8Bit());

    //    QString data = QString("FFFF00FE9701000082011F400B5402380BD40B5408660002FF00000005020064003C6239613062363933316137613466356661303462633236313331333161646362055000300070004000500040000012001C00004C3B0004986A001164BA0004986A1E13883C10047809C41E460014010000050000FAFA000A070000000000000000002020202020202020202020202020202020202020202020202020202020202020202020202020202020200000000007E2070A0C39320002015400020401010100C801010704002E000B01F0F00033FEFEFEFEFEFEFE0000353835393939393900003538353939393939000000000000000000000A0000000000000000009C");
        QString data = show.toLocal8Bit();
        QList<QString> *fieldsStr = SerialPortUtil::DeParseReadData(this->ruleObjects, data.replace((QChar)32,""));
        for(int i=0;i<this->inputItems->length();i++){
            InputItem *item = this->inputItems->at(i);
            item->SetReadValue(fieldsStr->at(i));
        }
    }
    buf.clear();
}

/**
 * @brief InputForm::deParseReadData
 * 反向解析出读出的数据
 */
void InputForm::deParseReadData(QString data){

}

void InputForm::reBuildFormWidget(){
//    delete ui->scrollAreaLeft->widget();
//    delete ui->scrollAreaRight->widget();
    this->inputItems->clear();

    buildLeftFormPart();
}
void InputForm::buildFormWidget(){


    ui->rootLayout->setMargin(0);
    buildLeftFormPart();
    buildRightFormPart();
}

void InputForm::buildLeftFormPart(){
    //读取配置文件
    readRuleConfig();

    QWidget *scrollContent = new QWidget(this);
    //    scrollContent->setMinimumSize(400, 400);
    //    scrollContent->setMaximumSize(400, 400);
    QVBoxLayout *scrollAreaLeftLayout = new QVBoxLayout(ui->scrollAreaLeft);

    scrollContent->setLayout(scrollAreaLeftLayout);
    scrollAreaLeftLayout->setMargin(4);
    scrollAreaLeftLayout->setSpacing(2);
    //    scrollContent->setStyleSheet("background:orange");// 背景色

    ui->scrollAreaLeft->setMinimumWidth(400);
    ui->scrollAreaLeft->setWidget(scrollContent);

    for(int i=0;i<this->ruleObjects->size();i++){
        RuleObject* rObj = this->ruleObjects->at(i);
        InputItem *inputItem = new InputItem(rObj, scrollContent);

        inputItem->setStyleSheet("background: white;");
        inputItem->setMinimumHeight(40);
        inputItem->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        scrollAreaLeftLayout->setStretchFactor(inputItem, 10);
        scrollAreaLeftLayout->addWidget(inputItem);
        this->inputItems->append(inputItem);
    }


    scrollAreaLeftLayout->addStretch(10);
}

void InputForm::buildRightFormPart(){
    //right scrollarea
    QWidget *scrollContent2 = new QWidget(this);
    QVBoxLayout *scrollAreaRightLayout = new QVBoxLayout(ui->scrollAreaRight);
    //    ui->scrollAreaRight->setMinimumWidth(400);
    scrollContent2->setLayout(scrollAreaRightLayout);
    scrollAreaRightLayout->setMargin(4);
    scrollAreaRightLayout->setSpacing(2);

    scrollAreaRightLayout->addWidget(ui->comSelect,0,Qt::AlignTop);
    scrollAreaRightLayout->addWidget(ui->btn_write,0,Qt::AlignTop);
    scrollAreaRightLayout->addWidget(ui->btn_read,0,Qt::AlignTop);

    ui->scrollAreaRight->setWidget(scrollContent2);

    scrollAreaRightLayout->addStretch(10);
}
/**
 * @brief InputForm::readRuleConfig
 * 将xml还原成QList<RuleObject*>
 * @return
 */
bool InputForm::readRuleConfig(){
//    QString filename = "rulesetting.xml";
    //    QString filename;
    QFile* file = new QFile(MainWindow::GetFileName());
    ConfigUtil util;
    delete this->ruleObjects;
    this->ruleObjects = util.ParseXMLConfig(file);
    return true;
}

void InputForm::on_btn_write_clicked()
{
    cout<<"写入按钮被点击了!"<<endl;

    if(!this->openCom()){
        return;
    }


    /*
     * <Header>FFFF</Header>
     *<Length exp="count(Command)+count(Seq)+count(Flags)+count(Data)+count(Chk)"/>
     *<Command>97</Command>
     *<Seq>01</Seq>
     *<Flags>0000</Flags>
     */

    QString data;

    for(int i=0;i<this->inputItems->size();i++){
        InputItem *input = this->inputItems->at(i);
        data.append(input->getHexString());
    }
    cout<<"--非完整的data = "<<data.toStdString()<<" data.size()="<<QString::number(data.length()).toStdString()<<endl;

    int lengthVal = (command.length()+seq.length()+flags.length()+data.length()+2)/2;
    length = QString("%L1 ").arg(lengthVal,2,16,QLatin1Char('0')).replace(" ","");
    cout<<"lengthVal = "<<QString::number(lengthVal).toStdString()<<endl;
    QString dataIncludeLen = QString("%L1 ").append(command).append(seq).append(flags).append(data).arg(lengthVal,4, 16,QLatin1Char('0')).replace(" ","");
    int chkVal = 0;
    //计算length的值
    for(int i=0;i<dataIncludeLen.length();i+=2){
        //cout<<"data split:"<<QString(data.at(i)).append(data.at(i+1)).toStdString()<<endl;
        bool ok;
        QString tmpStr = QString(dataIncludeLen.at(i)).append(dataIncludeLen.at(i+1));
        int tmp = tmpStr.toInt(&ok,16);
        cout<<"tmpStr = "<<tmpStr.toStdString()<<"  tmp="<<QString::number(tmp).toStdString()<<endl;
        chkVal+=tmp;
    }

    cout<<"chk = "<<QString::number(chkVal).toStdString();
    chk = QString::number(chkVal%256,16);
    cout<<", chkVal%256 = "<<chk.toStdString()<<endl;
    data = beginFlag.append(dataIncludeLen).append(chk).toUpper();
    cout<<"完整的data = "<<data.toStdString()<<endl;
    /*
     * <Chk exp="(sum(Len)+sum(Command)+sum(Seq)+sum(Flags)+sum(Data))%256"/>
     */


    //    QByteArray senddata =  data.toUtf8();//转换为16进制
    QByteArray senddata = QString2Hex(data);
    QString dataToPrint;
    for(int i=0;i<data.length();i++){
        dataToPrint.append(data.at(i));
        if(i > 0 && i%2==1){
            dataToPrint.append(" ");
        }
    }
    ui->console->putData(QString("<<<<<<<").append(dataToPrint).append("\n").toLocal8Bit());
    this->com->write(senddata);
}

bool InputForm::closeCom(){
    if(this->com != nullptr && this->com->isOpen()){
        this->com->close();
        comState = CLOSE;
    }

    return true;
}

bool InputForm::openCom(){
        if(!com->isOpen()){
            QString comName = ui->comSelect->currentText();
            this->com->setPortName(comName);// 指定端口
            if(!this->com->open(QIODevice::ReadWrite)){
                return false;
            }

            this->com->setBaudRate(QSerialPort::Baud9600); // 设置波特率
            this->com->setDataBits(QSerialPort::Data8); // 设置位数
            this->com->setParity(QSerialPort::EvenParity); // 设置偶校验
            this->com->setStopBits(QSerialPort::OneStop); // 设置一个停止位
            this->com->setFlowControl(QSerialPort::NoFlowControl); // 设置无流控制

            comState = OPEN;
            this->com->clearError();
            this->com->clear();
        }

        return true;
}

void InputForm::on_btn_read_clicked()
{

}

void InputForm::on_comSelect_currentTextChanged(const QString &arg1)
{
    if(this->com != nullptr && this->com->isOpen()){
        this->com->close();
    }
}

ComState InputForm::returnComState()
{
    return comState;
}

void InputForm::on_btnComState_clicked()
{
    cout << "button clicked" << endl;
    if (this->comState == CLOSE)
    {
        this->openCom();
        this->ui->btnComState->setText(QString::fromLocal8Bit("关闭"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon_/icons/disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QStringLiteral(":/icon_/icons/disconnect.png"), QSize(), QIcon::Normal, QIcon::On);
        ui->btnComState->setIcon(icon);
        ui->btnComState->setIconSize(QSize(32, 32));
    }else{
        this->closeCom();
        this->ui->btnComState->setText(QString::fromLocal8Bit("打开"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon_/icons/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QStringLiteral(":/icon_/icons/connect.png"), QSize(), QIcon::Normal, QIcon::On);
        ui->btnComState->setIcon(icon);
        ui->btnComState->setIconSize(QSize(32, 32));
    }

    emit MainWindow::UpdateStatusBar();
}
