#ifndef TYPEDEFINITIONS_H
#define TYPEDEFINITIONS_H

#include <QColor>
#include <QDebug>


struct Colors{

    QColor colorGreen=QColor(25,142,20);
    QColor colorRed=QColor(255, 0, 0);
    QColor colorYellow=QColor(237, 151, 10);
    QColor colorGray=QColor(107, 107, 107);


};

struct IsStatus{
    bool isMachineStatus=false;
    bool isProcessStatus=false;

};

struct MachineInputStatus{
    bool _inStatusVector[8];
    int redPort=0;
    int yellowPort=0;
    int greenPort=0;
    bool indicatorRed=false;
    bool indicatorYellow=false;
    bool indicatorGreen=false;

    void readStatusVector(bool statusVector[8]){
        for(int i=0;i<8;i++){
            _inStatusVector[i]=statusVector[i];
        }
    }

    void defineInputPort(QString portName, QString inPort){
        qDebug()<<inPort;

        for(int i=0;i<8;i++){
            QString port="E";
            if(inPort==port+QString::number(i) & portName=="Red"){
                redPort=i-1;
            }else if(inPort==port+QString::number(i) & portName=="Yellow"){
                yellowPort=i-1;
            }else if(inPort==port+QString::number(i) & portName=="Green"){
                greenPort=i-1;
            }
        }

    }

    void updateStatusVector(){
        indicatorRed=_inStatusVector[redPort];
        indicatorYellow=_inStatusVector[yellowPort];
        indicatorGreen=_inStatusVector[greenPort];
    }

};

struct InputStatusItem{
    int machineStatusItem;
    int processStatusItem;

    void adjustItems(int currentSelection){

        if(machineStatusItem>0)
            machineStatusItem--;
        if(processStatusItem>0)
            processStatusItem--;

       qDebug()<<"machineStatusItem: "+QString::number(machineStatusItem);
       qDebug()<<"processStatusItem: "+QString::number(processStatusItem);
    }

};

#endif // TYPEDEFINITIONS_H
