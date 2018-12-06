/*
 * DigitalOutputModule handles the communication via I2C between the Digital Output Module and the OPC Server
 *
 * @author  Fernando Felix, Kevin Martin, Sathwik Rongala
 * @version 1.2, 19/11/18
 * @see      controlboxserver.cpp
 */



#include "digitaloutputmodule.h"

DigitalOutputModule::DigitalOutputModule()
{
    digitalOutputSetUp();

}

void DigitalOutputModule::digitalOutputSetUp(){
    wiringPiSetup();

    fd_digital_output=pcf8574Setup(DIGITAL_OUTPUT_BASE,ADDRESS_DIGITAL_OUTPUT);

//    if(fd_digital_output<0){
//        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
//                    "Failed to open the DigitalOutput i2c bus.");
//        return;
//    }

    for(int i=0;i<8;i++){
        pinMode(DIGITAL_OUTPUT_BASE+i,OUTPUT);
        digitalWrite(DIGITAL_OUTPUT_BASE+i,1);
    }

}

bool DigitalOutputModule::setOutputState(bool state, portName enumPort){

    if(state){
        digitalWrite(DIGITAL_OUTPUT_BASE+enumPort,0); //On
        if(digitalRead(DIGITAL_OUTPUT_BASE+enumPort)==0)
            return true;
    }else{
        digitalWrite(DIGITAL_OUTPUT_BASE+enumPort,1); //Off
        if(digitalRead(DIGITAL_OUTPUT_BASE+enumPort)==1)
            return true;
    }

    return false;
}



