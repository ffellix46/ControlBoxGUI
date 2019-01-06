#include "digitalinputmodule.h"

DigitalInputModule::DigitalInputModule()
{
    digitalInputSetUp();
}

void DigitalInputModule::digitalInputSetUp(){

    fd_digital_input=pcf8574Setup(DIGITAL_INPUT_BASE,ADDRESS_DIGITAL_INPUT);

    if(fd_digital_input<0){
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Failed to open the DigitalInput i2c bus.");
        return;
    }

    for(int i=0;i<8;i++){
        pinMode(DIGITAL_INPUT_BASE+i,INPUT);
    }

}

void DigitalInputModule::addDigitalInputVariable(UA_Server *server) {
    UA_NodeId digitalInputId; /* get the nodeid assigned by the server */
    UA_NodeId currentNodeId = UA_NODEID_NULL;
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);


    UA_ObjectAttributes digitalInputAttr = UA_ObjectAttributes_default;
    digitalInputAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Digital Input Node");
    UA_Server_addObjectNode(server, currentNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, "Digital Input Node"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            digitalInputAttr, NULL, &digitalInputId);

    UA_DataSource digitalInputData[]={};
    digitalInputData[0].read = readCurrentDigitalInput;
    digitalInputData[0].write = writeCurrentDigitalInput;

    UA_VariableAttributes statuDI0Attr = UA_VariableAttributes_default;
    UA_NodeId statusNodeId = UA_NODEID_STRING(1, "Status-DI0");
    UA_Boolean statusDI0 = false;
    UA_Variant_setScalar(&statuDI0Attr.value, &statusDI0, &UA_TYPES[UA_TYPES_BOOLEAN]);
    statuDI0Attr.displayName = UA_LOCALIZEDTEXT("en-US", "Status DI0");
    statuDI0Attr.accessLevel = UA_ACCESSLEVELMASK_READ;
//    UA_Server_addVariableNode(server, statusNodeId, digitalInputId,
//                                        parentReferenceNodeId, UA_QUALIFIEDNAME(1, "Status-DI0"),
//                                        variableTypeNodeId, statuDI0Attr, NULL, NULL);

//    updateInput(server);
    UA_Server_addDataSourceVariableNode(server, currentNodeId, digitalInputId,
                                        parentReferenceNodeId, UA_QUALIFIEDNAME(1, "Status-DI0"),
                                        variableTypeNodeId, statuDI0Attr,
                                        digitalInputData[0], NULL, NULL);
}

void DigitalInputModule::updateInput(UA_Server *server) {
    qDebug()<<"update";
    UA_Boolean statusDI0 = digitalRead(DIGITAL_INPUT_BASE);
    UA_Variant value;
    UA_Variant_setScalar(&value, &statusDI0, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Status-DI0");;
    UA_Server_writeValue(server, currentNodeId, value);
}

bool DigitalInputModule::readInputState(int port){
    if(digitalRead(DIGITAL_INPUT_BASE+port)){
        //qDebug()<<"Off";
        return false; //OFF
    }else{
        //qDebug()<<"On";
        return true;  //ON
    }

}

UA_StatusCode DigitalInputModule::readCurrentDigitalInput(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {

    UA_Boolean statusDI0;
    if(digitalRead(DIGITAL_INPUT_BASE+E1))
        statusDI0=false;
    else
        statusDI0=true;

    UA_Variant_setScalarCopy(&dataValue->value, &statusDI0, &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}


UA_StatusCode DigitalInputModule::writeCurrentDigitalInput(UA_Server *server,
                 const UA_NodeId *sessionId, void *sessionContext,
                 const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {


    //Not implemented


    return UA_STATUSCODE_GOOD;
}
