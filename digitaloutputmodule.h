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
    DO0 = 0,
    DO1 = 1,
    DO2 = 2,
    DO3 = 3,
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
    void digitalOutputSetUp();
    static UA_StatusCode readCurrentDigitalOutputZero(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);
    static UA_StatusCode writeCurrentDigitalOutputZero(UA_Server *server,
                     const UA_NodeId *sessionId, void *sessionContext,
                     const UA_NodeId *nodeId, void *nodeContext,
                     const UA_NumericRange *range, const UA_DataValue *data);

};

#endif // DIGITALOUTPUTMODULE_H
