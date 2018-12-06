#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    digitalOutputModule = new DigitalOutputModule();
    controlBoxServer = new ControlBoxServer(this);

    connect(controlBoxServer,SIGNAL(response(QString)),ui->log_lineEdit,SLOT(setText(QString)));
    connect(controlBoxServer,SIGNAL(error(QString)),ui->log_lineEdit,SLOT(setText(QString)));
    connect(controlBoxServer,SIGNAL(timeout(QString)),ui->log_lineEdit,SLOT(setText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_DigitalOutput0_switch_clicked(){
    if(ui->DigitalOutput0_switch->_switch){
        digitalOutputModule->setOutputState(true,DO0);
    }else if(ui->DigitalOutput0_switch->_switch==false){
        digitalOutputModule->setOutputState(false,DO0);
    }

}

void MainWindow::on_DigitalOutput1_switch_clicked(){
    if(ui->DigitalOutput1_switch->_switch){
        digitalOutputModule->setOutputState(true,DO1);
    }else if(ui->DigitalOutput1_switch->_switch==false){
        digitalOutputModule->setOutputState(false,DO1);
    }

}

void MainWindow::on_startServerButton_clicked()
{
    controlBoxServer->transaction("OPC",100,"Start");

}

void MainWindow::on_stopServerButton_clicked()
{
    controlBoxServer->stopServer();
    //controlBoxServer->stopHandler(1);
}
