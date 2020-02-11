#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <Qdir>
#include <QFileDialog>
#include <QLabel>

using namespace std;

QString MainWindow::filename = QString(); // By default, it will be initialized to QString() though.
QLabel* MainWindow::configStatus = nullptr;
QLabel* MainWindow::comStatus = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::configStatus = new QLabel(this);

    ui->statusBar->addWidget(configStatus);//显示正常信息

    ui->statusBar->setSizeGripEnabled(true);//去掉状态栏右下角的三角

    MainWindow::comStatus = new QLabel(this);

    ui->statusBar->addPermanentWidget(comStatus);//显示永久信息

//    ui->statusBar->showMessage(QString::fromLocal8Bit("临时信息!"),20000);//显示临时信息2000ms 前面的正常信息被覆盖 当去掉后一项时，会一直显示

//    QLabel *permanent=new QLabel(this);
//    permanent->setFrameStyle(QFrame::Box|QFrame::Sunken);
//    permanent->setText(QString::fromLocal8Bit("<a href=\"http://tengweitw.ueuo.com\">永久信息</a>"));
//    permanent->setOpenExternalLinks(true);//设置可以打开网站链接
//    ui->statusBar->addPermanentWidget(permanent);//显示永久信息

    UpdateStatusBar();
    initActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::GetFileName(){
    return filename;
}

void MainWindow::SetFileName(QString f){
    filename = f;
}

void MainWindow::initActions(){
    ui->ruleSettingWidget->hide();
    ui->inputForm->hide();

    //在ui_mainwindow中使用QMetaObject::connectSlotsByName(MainWindow);实现信号槽连接

//    connect(ui->actionopen,SIGNAL(triggered()),SLOT(on_actionopen_triggered()));
//    connect(ui->actionsetting,SIGNAL(triggered()),SLOT(on_actionsetting_triggered()));
}


void MainWindow::on_actionsetting_triggered(){
//    if(filename.isEmpty()){
//        filename = QFileDialog::getOpenFileName(this,
//            tr("Open Xml"), QDir::currentPath(), tr("Xml Files (*.xml)"));
//        SetFileName(filename);
//    }
    ui->ruleSettingWidget->show();
    ui->inputForm->hide();
    cout<<"设置 被按了"<<endl;
}

void MainWindow::on_actionopen_triggered()
{
//    if(filename.isEmpty()){
//        filename = QFileDialog::getOpenFileName(this,
//            tr("Open Xml"), QDir::currentPath(), tr("Xml Files (*.xml)"));
//        SetFileName(filename);
//    }
    ui->inputForm->show();
    ui->ruleSettingWidget->hide();
    cout<<"参数输入 被按了"<<endl;
}

void MainWindow::on_actionloadxml_triggered(){
    QString oldFile = filename;
    filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("加载配置文件"), QDir::currentPath(), tr("Xml Files (*.xml)"));
    if (filename == ""){return;}
    ui->inputForm->reBuildFormWidget();
    ui->ruleSettingWidget->readXML(filename);
    UpdateStatusBar();
}

void MainWindow::UpdateStatusBar(){

    if(MainWindow::GetFileName().isEmpty()){
        if(configStatus != nullptr){
            configStatus->setText(QString::fromLocal8Bit("未选择配置文件!!!"));
        }
    }else{
        if(configStatus != nullptr){
            configStatus->setText(QString::fromLocal8Bit("当前配置: ").append(GetFileName()));
        }
    }

    if (InputForm::returnComState() == CLOSE)
    {
        comStatus->setText(QString::fromLocal8Bit("串口状态：已关闭"));
    }else{
        comStatus->setText(QString::fromLocal8Bit("串口状态：已开启"));
    }

}
