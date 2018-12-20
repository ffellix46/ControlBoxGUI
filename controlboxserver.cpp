/*
 * ControlBoxServer handles the OPC UA Server through the open62541 library
 *
 * @author  Fernando Felix, Kevin Martin, Sathwik Rongala
 * @version 1.2, 19/11/18
 * @see      controlboxserver.cpp
 */

#include "controlboxserver.h"
#include <QTime>
#include <QDateTime>
#include "/usr/include/signal.h"

QT_USE_NAMESPACE

UA_Boolean running=true;

ControlBoxServer::ControlBoxServer(QObject *parent)
    : QThread(parent), waitTimeout(0), quit(false)
{

    config=UA_ServerConfig_new_default();
    server=UA_Server_new(config);

//    UA_ServerConfig *config = UA_ServerConfig_new_default();
//    UA_Server *server = UA_Server_new(config);

//    UA_StatusCode retval = UA_Server_run(server,&running);
//    UA_Server_delete(server);
//    UA_ServerConfig_delete(config);


}

//! [0]

ControlBoxServer::~ControlBoxServer()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

//! [0]

//! [1] //! [2]
void ControlBoxServer::startServer()
{
//    //! [1]
//    QMutexLocker locker(&mutex);
//    this->portName = portName;
//    this->waitTimeout = waitTimeout;
//    this->request = request;
//    //! [3]
    if (!isRunning())
        start();
    else
        cond.wakeOne();
}
//! [2] //! [3]

//! [4]
void ControlBoxServer::run()
{
    running=true;

    while (!quit) {
        //![6] //! [7]

        if (running) {
            emit response("["+QDateTime::currentDateTime().toString()+"] TCP network layer listening on opc.tcp://raspberrypi:4840/");

            UA_StatusCode retval = UA_Server_run(server,&running);
            UA_Server_delete(server);
            UA_ServerConfig_delete(config);

            if (retval != UA_STATUSCODE_GOOD) {
                emit error("Can't open server, error code 1");
                return;
            }
            //! [7] //! [8]
            // send log info


//            QString response(retval);

//            emit this->response(response);
//            emit timeout(tr("Wait read response timeout %1")
//                         .arg(QTime::currentTime().toString()));
        }


    }
    //! [13]
}

void ControlBoxServer::stopServer()
{
    running=false;
    emit response("["+QDateTime::currentDateTime().toString()+"] Shutting down the TCP network layer");

}

//Digital Output Module

void ControlBoxServer::addDigitalOutputModule(DigitalOutputModule *digitalOutputModule){
    digitalOutputModule->addDigitalOutputVariable(server);
}
