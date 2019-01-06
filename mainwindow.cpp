#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->popUp_stack->hide();

    digitalOutputModule = new DigitalOutputModule();
    digitalInputModule = new DigitalInputModule();
    colors = new Colors;
    isStatus = new IsStatus;
    inputstatusItem = new InputStatusItem;
    machineInputStatus = new MachineInputStatus;
    timer = new QTimer(this);

    setupTable();


    //The Digital Tab
    //ReadUserSavedVariable_DigitalTab();

    connect(timer, SIGNAL(timeout()), this, SLOT(updateIconStatus()));
    timer->start(1000);





}

MainWindow::~MainWindow()
{
    controlBoxServer->stopServer();
    delete ui;
}

void MainWindow::updateIconStatus() //on_TimerTimeout()
{
    timer->stop();
    //qDebug()<<"Timeout";

    bool inStatusVector[8];
    for(int i=0;i<8;i++){
        inStatusVector[i]=digitalInputModule->readInputState(i);
    }

    machineInputStatus->readStatusVector(inStatusVector);

    if(isStatus->isMachineStatus){
        machineInputStatus->updateStatusVector();
        if(machineInputStatus->indicatorRed)
            ui->inputTable->item(inputstatusItem->machineStatusItem,2)->setBackgroundColor(colors->colorRed);
        else if(machineInputStatus->indicatorYellow)
            ui->inputTable->item(inputstatusItem->machineStatusItem,2)->setBackgroundColor(colors->colorYellow);
        else if(machineInputStatus->indicatorGreen)
            ui->inputTable->item(inputstatusItem->machineStatusItem,2)->setBackgroundColor(colors->colorGreen);
        else
            ui->inputTable->item(inputstatusItem->machineStatusItem,2)->setBackgroundColor(colors->colorGray);
    }

    timer->start(1000);
}

void MainWindow::writeInLog(QString message)
{
    ui->log_textEdit->append("["+QDateTime::currentDateTime().toString()+"] "+message);
}

void MainWindow::setupTable()
{
    statusItem=0;
    QStringList m_TableHeader;
    m_TableHeader<<"Name"<<"Pin"<<"Status";
    ui->inputTable->setHorizontalHeaderLabels(m_TableHeader);
    ui->inputTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->inputTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->inputTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->inputTable->setStyleSheet("QTableView {selection-background-color: blue;}");


        //ui->Digital_0_Indicator->setStyleSheet("image: url(:/Icon/Image/greenIndicator.png);");
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
    controlBoxServer->addDigitalInputModule(digitalInputModule);


}

void MainWindow::on_stopServerButton_clicked()
{
    controlBoxServer->stopServer();
    delete controlBoxServer;
}



/*
THIS IS THE DIGITAALLLL TAB
*/



//Save Settings

void MainWindow::ReadUserSavedVariable()
{
    // Load Application params from ini file
    QDir location = QDir::currentPath() + "/ControlBoxGUI/Settings/PinSelection.ini";
    QSettings settings( location.absolutePath(), QSettings::IniFormat );
    qDebug()<< "Path : " << location.absolutePath();
    QString machineStatus;
    QStringList machineStatusVector;
    QString processStatus;


    settings.beginGroup("PIN_SELECTION");
    machineStatus = settings.value("MachineStatus").toString();
    qDebug()<< machineStatus;
    if(machineStatus=="Deleted,"){

    }else{
        machineStatusVector = machineStatus.split(',');
        machineInputStatus->defineInputPort("Red",machineStatusVector[0]);
        machineInputStatus->defineInputPort("Yellow",machineStatusVector[1]);
        machineInputStatus->defineInputPort("Green",machineStatusVector[2]);
    }

    settings.endGroup();



}

void MainWindow::on_DigitalSaveButton_clicked()
{
    // Load Application params from ini file
    QDir location = QDir::currentPath() + "/ControlBoxGUI/Settings/DigitalTab.ini";
    QSettings settings( location.absolutePath(), QSettings::IniFormat );

    settings.beginGroup("OUTPUT");
//    settings.setValue("OutputName0", ui->DigitalOutput0_LineEdit->text());
//    settings.setValue("OutputName1", ui->DigitalOutput1_LineEdit->text());
//    settings.setValue("OutputName2", ui->DigitalOutput2_LineEdit->text());
//    settings.setValue("OutputName3", ui->DigitalOutput3_LineEdit->text());
    settings.endGroup();


}

/*
OUTPUTS
*/

//Door

void MainWindow::on_openDoorButton_pressed()
{
    digitalOutputModule->setOutputState(true,openDoor);
    writeInLog("Digital Output 0 Changed");
}

void MainWindow::on_openDoorButton_released()
{
    digitalOutputModule->setOutputState(false,openDoor);
}

void MainWindow::on_closeDoorButton_pressed()
{
    digitalOutputModule->setOutputState(true,closeDoor);
    writeInLog("Digital Output 1 Changed");
}

void MainWindow::on_closeDoorButton_released()
{
    digitalOutputModule->setOutputState(false,closeDoor);
}

//Tens

void MainWindow::on_openTensButton_pressed()
{
    digitalOutputModule->setOutputState(true,openTens);
    writeInLog("Digital Output 2 Changed");
}

void MainWindow::on_openTensButton_released()
{
    digitalOutputModule->setOutputState(false,openTens);
}

void MainWindow::on_closeTensButton_pressed()
{
    digitalOutputModule->setOutputState(true,closeTens);
    writeInLog("Digital Output 3 Changed");
}

void MainWindow::on_closeTensButton_released()
{
    digitalOutputModule->setOutputState(false,closeTens);
}

//Process

void MainWindow::on_runProcessButton_pressed()
{
    digitalOutputModule->setOutputState(true,runProcess);
    writeInLog("Digital Output 4 Changed");
}

void MainWindow::on_runProcessButton_released()
{
    digitalOutputModule->setOutputState(false,runProcess);
}

void MainWindow::on_stopProcessButton_pressed()
{
    digitalOutputModule->setOutputState(true,stopProcess);
    writeInLog("Digital Output 5 Changed");
}

void MainWindow::on_stopProcessButton_released()
{
    digitalOutputModule->setOutputState(false,stopProcess);
}

/*
INPUTS
*/

void MainWindow::on_addInputButton_clicked()
{
    ui->popUp_stack->show();
    ui->popUp_stack->setCurrentIndex(0);
}

void MainWindow::on_cancelStackButton_clicked()
{
    ui->popUp_stack->hide();
}

void MainWindow::on_backStackButton_clicked()
{
    ui->popUp_stack->setCurrentIndex(0);
}


void MainWindow::on_pinSelectionButton_clicked()
{
    ui->popUp_stack->setCurrentIndex(1);
    switch(ui->mainStackComboBox->currentIndex()){
    case(0):
        //qDebug()<< "Machine Status";
        ui->statusSettingsStack->setCurrentIndex(0);
        break;
    case(1):
        //qDebug()<< "Process Status";
        ui->statusSettingsStack->setCurrentIndex(1);
        break;
    case(2):
        qDebug()<< "Door Status";
        ui->statusSettingsStack->setCurrentIndex(2);
        break;
    case(3):
        qDebug()<< "Clamping Status";

        break;
    case(4):
        qDebug()<< "Spindle Status";
        break;
    case(5):
        qDebug()<< "Coolant Status";
        break;
    case(6):
        qDebug()<< "Waste Status";
        break;
    }

}

void MainWindow::on_saveSettingsButton_popUP_clicked()
{
    switch(ui->statusSettingsStack->currentIndex()){
    case(0):
        //Machine Status
        if(isStatus->isMachineStatus)
            updateStatus("MachineStatus");
        else
            addStatus("MachineStatus");
        break;
    case(1):
        //Process Status
        if(isStatus->isProcessStatus)
            updateStatus("ProcessStatus");
        else
            addStatus("ProcessStatus");
        break;
//    case(2):
//        //Door Status

//        break;
//    case(3):
//        //Clamping Status

//        break;
//    case(4):
//        //Spindle Status
//        break;
//    case(5):
//        //Coolant Status
//        break;
//    case(6):
//        //Waste Status
//        break;
    }

    ReadUserSavedVariable();
}

void MainWindow::on_deleteInputButton_clicked()
{
    if((ui->inputTable->currentRow()==inputstatusItem->machineStatusItem) & isStatus->isMachineStatus)
        deleteStatus("MachineStatus");
    else if((ui->inputTable->currentRow()==inputstatusItem->processStatusItem) & isStatus->isProcessStatus)
        deleteStatus("ProcessStatus");
}


void MainWindow::addStatus(QString status)
{
    // Load Application params from ini file
    QDir location = QDir::currentPath() + "/ControlBoxGUI/Settings/PinSelection.ini";
    QSettings settings( location.absolutePath(), QSettings::IniFormat );

    settings.beginGroup("PIN_SELECTION");

    if(status=="MachineStatus"){
        QString Pins=""+ui->MachineComboBox_red->currentText()+
                ","+ui->MachineComboBox_yellow->currentText()+
                ","+ui->MachineComboBox_green->currentText();
        ui->inputTable->setItem(statusItem, 0, new QTableWidgetItem("Machine Status"));
        ui->inputTable->setItem(statusItem, 1, new QTableWidgetItem(Pins));
        ui->inputTable->setItem(statusItem, 2, new QTableWidgetItem);
        ui->inputTable->item(statusItem, 2)->setBackgroundColor(colors->colorGray);
        settings.setValue("MachineStatus", Pins);
        isStatus->isMachineStatus=true;
        inputstatusItem->machineStatusItem=statusItem;
        statusItem++;
    }else if(status=="ProcessStatus"){
        QString Pins=""+ui->ProcessStatusComboBox->currentText();
        ui->inputTable->setItem(statusItem, 0, new QTableWidgetItem("Process Status"));
        ui->inputTable->setItem(statusItem, 1, new QTableWidgetItem(Pins));
        ui->inputTable->setItem(statusItem, 2, new QTableWidgetItem);
        ui->inputTable->item(statusItem, 2)->setBackgroundColor(colors->colorGray);
        settings.setValue("ProcessStatus", Pins);
        isStatus->isProcessStatus=true;
        inputstatusItem->processStatusItem=statusItem;
        statusItem++;

    }

    qDebug()<<"statusItem in add: "+QString::number(statusItem);
    settings.endGroup();

}

void MainWindow::updateStatus(QString status)
{
    // Load Application params from ini file
    QDir location = QDir::currentPath() + "/ControlBoxGUI/Settings/PinSelection.ini";
    QSettings settings( location.absolutePath(), QSettings::IniFormat );

    settings.beginGroup("PIN_SELECTION");

    if(status=="MachineStatus"){
        QString Pins=""+ui->MachineComboBox_red->currentText()+
                ","+ui->MachineComboBox_yellow->currentText()+
                ","+ui->MachineComboBox_green->currentText();
        ui->inputTable->setItem(inputstatusItem->machineStatusItem, 0, new QTableWidgetItem("Machine Status"));
        ui->inputTable->setItem(inputstatusItem->machineStatusItem, 1, new QTableWidgetItem(Pins));
        ui->inputTable->setItem(inputstatusItem->machineStatusItem, 2, new QTableWidgetItem);
        ui->inputTable->item(inputstatusItem->machineStatusItem, 2)->setBackgroundColor(colors->colorGray);
        settings.setValue("MachineStatus", Pins);
    }else if(status=="ProcessStatus"){
        QString Pins=""+ui->ProcessStatusComboBox->currentText();
        ui->inputTable->setItem(inputstatusItem->processStatusItem, 0, new QTableWidgetItem("Process Status"));
        ui->inputTable->setItem(inputstatusItem->processStatusItem, 1, new QTableWidgetItem(Pins));
        ui->inputTable->setItem(inputstatusItem->processStatusItem, 2, new QTableWidgetItem);
        ui->inputTable->item(inputstatusItem->processStatusItem, 2)->setBackgroundColor(colors->colorGray);
        settings.setValue("ProcessStatus", Pins);
    }

    settings.endGroup();

}

void MainWindow::deleteStatus(QString status)
{
    // Load Application params from ini file
    QDir location = QDir::currentPath() + "/ControlBoxGUI/Settings/PinSelection.ini";
    QSettings settings( location.absolutePath(), QSettings::IniFormat );

    settings.beginGroup("PIN_SELECTION");

    if(status=="MachineStatus"){
        ui->inputTable->removeRow(inputstatusItem->machineStatusItem);
        inputstatusItem->adjustItems(ui->inputTable->currentRow());
        if(statusItem>0){
            statusItem--;
        }
        ui->inputTable->setRowCount(8);
        isStatus->isMachineStatus=false;
        settings.setValue("MachineStatus", "Deleted,");
    }else if(status=="ProcessStatus"){
        ui->inputTable->removeRow(inputstatusItem->processStatusItem);
        inputstatusItem->adjustItems(ui->inputTable->currentRow());
        if(statusItem>0){
            statusItem--;
        }
        ui->inputTable->setRowCount(8);
        isStatus->isProcessStatus=false;
        settings.setValue("ProcessStatus", "Deleted,");

    }
    settings.endGroup();
}


/*
HELP BUTTON
*/

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
