#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDir>
#include <QList>
#include <QTableView>
#include <QTimer>
#include "UIclass/switch.h"
#include "digitaloutputmodule.h"
#include "digitalinputmodule.h"
#include "controlboxserver.h"
#include "typedefinitions.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startServerButton_clicked();
    void on_stopServerButton_clicked();
    void on_helpButton_clicked();
    void on_DigitalSaveButton_clicked();
    //Outputs
    void on_openDoorButton_pressed();
    void on_openDoorButton_released();
    void on_closeDoorButton_pressed();
    void on_closeDoorButton_released();
    void on_openTensButton_pressed();
    void on_openTensButton_released();
    void on_closeTensButton_pressed();
    void on_closeTensButton_released();
    void on_runProcessButton_pressed();
    void on_runProcessButton_released();
    void on_stopProcessButton_pressed();
    void on_stopProcessButton_released();
    //Inputs
    void on_addInputButton_clicked();
    void on_cancelStackButton_clicked();
    void on_pinSelectionButton_clicked();
    void on_backStackButton_clicked();
    void on_saveSettingsButton_popUP_clicked();
    void on_deleteInputButton_clicked();
    void updateIconStatus();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    DigitalOutputModule *digitalOutputModule;
    DigitalInputModule *digitalInputModule;
    ControlBoxServer *controlBoxServer;
    Colors *colors;
    MachineInputStatus *machineInputStatus;
    IsStatus *isStatus;
    InputStatusItem *inputstatusItem;
    int statusItem;
    bool isServer;

    void setupTable();
    void ReadUserSavedVariable();
    void writeInLog(QString message);
    void addStatus(QString status);
    void updateStatus(QString status);
    void deleteStatus(QString status);

};

#endif // MAINWINDOW_H
