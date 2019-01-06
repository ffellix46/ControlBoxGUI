#ifndef DIGITALINPUTMODULE_H
#define DIGITALINPUTMODULE_H

#include "open62541.h"
#include "/usr/include/wiringPi.h"
#include "/usr/include/wiringPiI2C.h"
#include "/usr/include/pcf8574.h"
//#include "/usr/include/time.h"
#include "/usr/include/signal.h"
#include <QDebug>

#define ADDRESS_DIGITAL_INPUT 0x21
#define DIGITAL_INPUT_BASE 150

enum inPortName{
    E1 = 0,
    E2 = 1,
    E3 = 2,
    E4 = 3,
    E5 = 4,
    E6 = 5
};

class DigitalInputModule
{
public:
    DigitalInputModule();
    static bool readInputState(int port);
    static void addDigitalInputVariable(UA_Server *server);
    static void updateInput(UA_Server *server, char *status);

private:
    int fd_digital_input = 0;
    void digitalInputSetUp();
    static UA_StatusCode readCurrentDigitalInput(UA_Server *server,
                    const UA_NodeId *sessionId, void *sessionContext,
                    const UA_NodeId *nodeId, void *nodeContext,
                    UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                    UA_DataValue *dataValue);
    static UA_StatusCode writeCurrentDigitalInput(UA_Server *server,
                     const UA_NodeId *sessionId, void *sessionContext,
                     const UA_NodeId *nodeId, void *nodeContext,
                     const UA_NumericRange *range, const UA_DataValue *data);

};

#endif // DIGITALINPUTMODULE_H
