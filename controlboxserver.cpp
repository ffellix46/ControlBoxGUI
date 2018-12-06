/*
 * ControlBoxServer handles the OPC UA Server through the open62541 library
 *
 * @author  Fernando Felix, Kevin Martin, Sathwik Rongala
 * @version 1.2, 19/11/18
 * @see      controlboxserver.cpp
 */

#include "controlboxserver.h"
#include "open62541.h"
#include <QTime>
#include "/usr/include/signal.h"

QT_USE_NAMESPACE

UA_Boolean running=true;

ControlBoxServer::ControlBoxServer(QObject *parent)
    : QThread(parent), waitTimeout(0), quit(false)
{

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
void ControlBoxServer::transaction(const QString &portName, int waitTimeout, const QString &request)
{
    //! [1]
    QMutexLocker locker(&mutex);
    this->portName = portName;
    this->waitTimeout = waitTimeout;
    this->request = request;
    //! [3]
    if (!isRunning())
        start();
    else
        cond.wakeOne();
}
//! [2] //! [3]

//! [4]
void ControlBoxServer::run()
{
    bool currentPortNameChanged = false;

    mutex.lock();
    //! [4] //! [5]
    QString currentPortName;
    if (currentPortName != portName) {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTimeout;
    QString currentRequest = request;
    mutex.unlock();
    //! [5] //! [6]
    UA_ServerConfig *config = UA_ServerConfig_new_default();
    UA_Server *server = UA_Server_new(config);

    while (!quit) {
        //![6] //! [7]
        if (currentPortNameChanged) {
            emit response("Running...");

            UA_StatusCode retval = UA_Server_run(server,&running);

            if (retval != UA_STATUSCODE_GOOD) {
                emit error("Can't open %1, error code %2");
                return;
            }
            //! [7] //! [8]
            // send log info

            QString response(retval);

//            emit this->response(response);
//            emit timeout(tr("Wait read response timeout %1")
//                         .arg(QTime::currentTime().toString()));
        }


        mutex.lock();
        cond.wait(&mutex);
        if (currentPortName != portName) {
            currentPortName = portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = waitTimeout;
        currentRequest = request;
        mutex.unlock();
    }
    //! [13]
}

void ControlBoxServer::stopServer()
{
    running=false;

}
