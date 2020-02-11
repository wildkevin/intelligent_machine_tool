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

    ui->statusBar->addWidget(configStatus);//��ʾ������Ϣ

    ui->statusBar->setSizeGripEnabled(true);//ȥ��״̬�����½ǵ�����

    MainWindow::comStatus = new QLabel(this);

    ui->statusBar->addPermanentWidget(comStatus);//��ʾ������Ϣ

//    ui->statusBar->showMessage(QString::fromLocal8Bit("��ʱ��Ϣ!"),20000);//��ʾ��ʱ��Ϣ2000ms ǰ���������Ϣ������ ��ȥ����һ��ʱ����һֱ��ʾ

//    QLabel *permanent=new QLabel(this);
//    permanent->setFrameStyle(QFrame::Box|QFrame::Sunken);
//    permanent->setText(QString::fromLocal8Bit("<a href=\"http://tengweitw.ueuo.com\">������Ϣ</a>"));
//    permanent->setOpenExternalLinks(true);//���ÿ��Դ���վ����
//    ui->statusBar->addPermanentWidget(permanent);//��ʾ������Ϣ

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

    //��ui_mainwindow��ʹ��QMetaObject::connectSlotsByName(MainWindow);ʵ���źŲ�����

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
    cout<<"���� ������"<<endl;
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
    cout<<"�������� ������"<<endl;
}

void MainWindow::on_actionloadxml_triggered(){
    QString oldFile = filename;
    filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("���������ļ�"), QDir::currentPath(), tr("Xml Files (*.xml)"));
    if (filename == ""){return;}
    ui->inputForm->reBuildFormWidget();
    ui->ruleSettingWidget->readXML(filename);
    UpdateStatusBar();
}

void MainWindow::UpdateStatusBar(){

    if(MainWindow::GetFileName().isEmpty()){
        if(configStatus != nullptr){
            configStatus->setText(QString::fromLocal8Bit("δѡ�������ļ�!!!"));
        }
    }else{
        if(configStatus != nullptr){
            configStatus->setText(QString::fromLocal8Bit("��ǰ����: ").append(GetFileName()));
        }
    }

    if (InputForm::returnComState() == CLOSE)
    {
        comStatus->setText(QString::fromLocal8Bit("����״̬���ѹر�"));
    }else{
        comStatus->setText(QString::fromLocal8Bit("����״̬���ѿ���"));
    }

}
