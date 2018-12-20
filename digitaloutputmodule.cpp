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

    if(fd_digital_output<0){
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Failed to open the DigitalOutput i2c bus.");
        return;
    }

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

bool DigitalOutputModule::readOutputState(portName enumPort){

    if(digitalRead(DIGITAL_OUTPUT_BASE+enumPort)){
        return false; //OFF
    }else{
        return true;  //ON
    }
}


void DigitalOutputModule::addDigitalOutputVariable(UA_Server *server) {
        UA_NodeId digitalOutputId; /* get the nodeid assigned by the server */
        UA_NodeId currentNodeId = UA_NODEID_NULL;
        UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
        UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);


        UA_ObjectAttributes digitalOutputAttr = UA_ObjectAttributes_default;
        digitalOutputAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Digital Output Node");
        UA_Server_addObjectNode(server, currentNodeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                UA_QUALIFIEDNAME(1, "Digital Output Node"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                digitalOutputAttr, NULL, &digitalOutputId);

        UA_DataSource digitalOutputDataZero;
        digitalOutputDataZero.read = readCurrentDigitalOutputZero;
        digitalOutputDataZero.write = writeCurrentDigitalOutputZero;

        UA_VariableAttributes statuDO0Attr = UA_VariableAttributes_default;
        UA_Boolean statusDO0 = false;
        UA_Variant_setScalar(&statuDO0Attr.value, &statusDO0, &UA_TYPES[UA_TYPES_BOOLEAN]);
        statuDO0Attr.displayName = UA_LOCALIZEDTEXT("en-US", "Status DO0");
        statuDO0Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
        UA_Server_addDataSourceVariableNode(server, currentNodeId, digitalOutputId,
                                            parentReferenceNodeId, UA_QUALIFIEDNAME(1, "Status DO0"),
                                            variableTypeNodeId, statuDO0Attr,
                                            digitalOutputDataZero, NULL, NULL);


}

UA_StatusCode DigitalOutputModule::readCurrentDigitalOutputZero(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue) {

    UA_Boolean statusDO0;
    if(digitalRead(DIGITAL_OUTPUT_BASE+DO0))
        statusDO0=false;
    else
        statusDO0=true;

    UA_Variant_setScalarCopy(&dataValue->value, &statusDO0, &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}


UA_StatusCode DigitalOutputModule::writeCurrentDigitalOutputZero(UA_Server *server,
                 const UA_NodeId *sessionId, void *sessionContext,
                 const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data) {


    if(digitalRead(DIGITAL_OUTPUT_BASE+DO0)){
        digitalWrite(DIGITAL_OUTPUT_BASE+DO0,0); //ON
    }else{
        digitalWrite(DIGITAL_OUTPUT_BASE+DO0,1); //OFF
    }

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                    "Digital Output  0 changed");


    return UA_STATUSCODE_GOOD;
}



