#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDir>
#include <QList>
#include "UIclass/switch.h"
#include "digitaloutputmodule.h"
#include "controlboxserver.h"

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
    void DigitalInput0_Selected();//DigitalInput0_ComboBox
    void on_startServerButton_clicked();
    void on_stopServerButton_clicked();
    void on_DigitalOutput0_switch_clicked();
    void on_DigitalOutput1_switch_clicked();
    void on_helpButton_clicked();
    void on_DigitalSaveButton_clicked();


private:
    Ui::MainWindow *ui;
    DigitalOutputModule *digitalOutputModule;
    ControlBoxServer *controlBoxServer;
    void ReadUserSavedVariable_DigitalTab();
    void writeInLog(QString message);
};

#endif // MAINWINDOW_H
