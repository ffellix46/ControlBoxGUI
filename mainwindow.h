#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
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
    void on_DigitalOutput0_switch_clicked();
    void on_DigitalOutput1_switch_clicked();
    void on_startServerButton_clicked();
    void on_stopServerButton_clicked();

private:
    Ui::MainWindow *ui;
    DigitalOutputModule *digitalOutputModule;
    ControlBoxServer *controlBoxServer;
};

#endif // MAINWINDOW_H
