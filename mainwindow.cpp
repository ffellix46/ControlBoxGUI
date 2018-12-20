#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    digitalOutputModule = new DigitalOutputModule();

    //The Digital Tab
    ReadUserSavedVariable_DigitalTab();

    connect(ui->DigitalInput0_ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(DigitalInput0_Selected()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeInLog(QString message)
{
    ui->log_textEdit->append("["+QDateTime::currentDateTime().toString()+"] "+message);
}

/*
THIS IS THE SERVER TAB
*/

void MainWindow::on_startServerButton_clicked()
{

    controlBoxServer = new ControlBoxServer(this);

    connect(controlBoxServer,SIGNAL(response(QString)),ui->log_textEdit,SLOT(append(QString)));
    connect(controlBoxServer,SIGNAL(error(QString)),ui->log_textEdit,SLOT(append(QString)));
    connect(controlBoxServer,SIGNAL(timeout(QString)),ui->log_textEdit,SLOT(append(QString)));


    controlBoxServer->startServer();
    controlBoxServer->addDigitalOutputModule(digitalOutputModule);


}

void MainWindow::on_stopServerButton_clicked()
{
    controlBoxServer->stopServer();
    delete controlBoxServer;
}



/*
THIS IS THE DIGITAALLLL TAB
*/

void MainWindow::on_DigitalOutput0_switch_clicked(){

    if(ui->DigitalOutput0_switch->_switch){
        if(digitalOutputModule->setOutputState(true,DO0))
            writeInLog("Digital Output 0 Changed: ON");
        else
            writeInLog("Failed to set Digital Output State");
    }else if(ui->DigitalOutput0_switch->_switch==false){
        if(digitalOutputModule->setOutputState(false,DO0))
            writeInLog("Digital Output 0 Changed: OFF");
        else
            writeInLog("Failed to set Digital Output State");
    }

}

void MainWindow::on_DigitalOutput1_switch_clicked(){

    if(ui->DigitalOutput1_switch->_switch){
        if(digitalOutputModule->setOutputState(true,DO1))
            writeInLog("Digital Output 1 Changed: ON");
        else
            writeInLog("Failed to set Digital Output State");
    }else if(ui->DigitalOutput1_switch->_switch==false){
        if(digitalOutputModule->setOutputState(false,DO1))
            writeInLog("Digital Output 1 Changed: OFF");
        else
            writeInLog("Failed to set Digital Output State");
    }

}

void MainWindow::DigitalInput0_Selected(){

    switch(ui->DigitalInput0_ComboBox->currentIndex()){
    case(0):
        qDebug()<< "Machine Status";
        ui->Digital_0_Indicator->setStyleSheet("image: url(:/Icon/Image/redIndicator.png);");
//        ui->Digital_1_Indicator->setStyleSheet("image: url(:/Icon/Image/yellowIndicator.png);");
//        ui->Digital_2_Indicator->setStyleSheet("image: url(:/Icon/Image/greenIndicator.png);");
//        ui->Digital_3_Indicator->setStyleSheet("image: url(:/Icon/Image/orangeIndicator.png);");
        ui->DigitalInput1_ComboBox->setCurrentIndex(0);
        ui->DigitalInput1_ComboBox->setEnabled(false);

        break;
    case(1):
        qDebug()<< "Process Status";
        ui->Digital_0_Indicator->setStyleSheet("image: url(:/Icon/Image/greenIndicator.png);");
        ui->DigitalInput1_ComboBox->setEnabled(true);
        break;
    case(2):
        qDebug()<< "Door Status";
        ui->Digital_0_Indicator->setStyleSheet("image: url(:/Icon/Image/blueIndicator.png);");
        ui->DigitalInput1_ComboBox->setEnabled(true);
        break;
    case(3):
        qDebug()<< "Clamping Status";
        ui->Digital_0_Indicator->setStyleSheet("image: url(:/Icon/Image/orangeIndicator.png);");

        break;
    case(4):
        qDebug()<< "Spindle Status";
        ui->Digital_0_Indicator->setStyleSheet("image: url(:/Icon/Image/yellowIndicator.png);");
        break;
    case(5):
        qDebug()<< "Coolant Status";
        ui->Digital_0_Indicator->setStyleSheet("image: url(:/Icon/Image/redIndicator.png);");
        break;
    case(6):
        qDebug()<< "Waste Status";
        ui->Digital_0_Indicator->setStyleSheet("image: url(:/Icon/Image/blueIndicator.png);");
        break;


    }

//    if(ui->DigitalInput0_ComboBox->currentText()=="Machine Status"){
//        qDebug()<< "Tab Graph";
//         ui->Digital_0_Indicator->setStyleSheet("image: url(:/Icon/Image/greenIndicator.png);");
//    }else if(ui->DigitalInput0_ComboBox->currentText()=="Machine Status"){

//    }
}

//Save Settings

void MainWindow::ReadUserSavedVariable_DigitalTab()
{
    // Load Application params from ini file
    QDir location = QDir::currentPath() + "/ControlBoxGUI/Settings/DigitalTab.ini";
    QSettings settings( location.absolutePath(), QSettings::IniFormat );
    qDebug()<< "Path : " << location.absolutePath();
    QString output[4];


    settings.beginGroup("OUTPUT");
    output[0] = settings.value("OutputName0").toString();
    output[1] = settings.value("OutputName1").toString();
    output[2] = settings.value("OutputName2").toString();
    output[3] = settings.value("OutputName3").toString();

    ui->DigitalOutput0_LineEdit->setText(output[0]);
    ui->DigitalOutput1_LineEdit->setText(output[1]);
    ui->DigitalOutput2_LineEdit->setText(output[2]);
    ui->DigitalOutput3_LineEdit->setText(output[3]);
    settings.endGroup();


}

void MainWindow::on_DigitalSaveButton_clicked()
{
    // Load Application params from ini file
    QDir location = QDir::currentPath() + "/ControlBoxGUI/Settings/DigitalTab.ini";
    QSettings settings( location.absolutePath(), QSettings::IniFormat );

    settings.beginGroup("OUTPUT");
    settings.setValue("OutputName0", ui->DigitalOutput0_LineEdit->text());
    settings.setValue("OutputName1", ui->DigitalOutput1_LineEdit->text());
    settings.setValue("OutputName2", ui->DigitalOutput2_LineEdit->text());
    settings.setValue("OutputName3", ui->DigitalOutput3_LineEdit->text());
    settings.endGroup();


}

// Help Button

void MainWindow::on_helpButton_clicked()
{
    QMessageBox::information(this,tr(" Machine Status Stack Lights"),
                             tr("Description \n"
                                "\n"
                                "RED. Any critical Event or Machine-State Condition that can cause a shut down.\n"
                                "YELLOW. Any Warning Condition that may not necessarily shut a machine down.\n"
                                "GREEN. Most often used to indicate machine is operting normally.\n"
                                "BLUE. Often used to Request Service from Maintenance or Supervisor."
                                ));
}

