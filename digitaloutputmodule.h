#ifndef DIGITALOUTPUTMODULE_H
#define DIGITALOUTPUTMODULE_H

#include "open62541.h"
#include "/usr/include/wiringPi.h"
#include "/usr/include/wiringPiI2C.h"
#include "/usr/include/pcf8574.h"
//#include "/usr/include/time.h"
#include <QDebug>


#define ADDRESS_DIGITAL_OUTPUT 0x25
#define DIGITAL_OUTPUT_BASE 100
enum portName{
    openDoor = 0,
    closeDoor = 1,
    openTens = 2,
    closeTens = 3,
    runProcess = 4,
    stopProcess = 5
};

class DigitalOutputModule
{

public:
    DigitalOutputModule();
    bool setOutputState(bool state, portName enumPort);
    bool readOutputState(portName enumPort);
    static void addDigitalOutputVariable(UA_Server *server);


private:
    int fd_digital_output = 0;
    //DigitalOutputModule *digitalOutputModule;
    void digitalOutputSetUp();
    static UA_StatusCode readCurrentDigitalOutput(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);
    static UA_StatusCode writeCurrentDigitalOutput(UA_Server *server,
                     const UA_NodeId *sessionId, void *sessionContext,
                     const UA_NodeId *nodeId, void *nodeContext,
                     const UA_NumericRange *range, const UA_DataValue *data);

};

#endif // DIGITALOUTPUTMODULE_H
