#ifndef CONTROLBOXSERVER_H
#define CONTROLBOXSERVER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QObject>
#include "open62541.h"
#include "digitaloutputmodule.h"
#include "digitalinputmodule.h"

//! [0]
class ControlBoxServer : public QThread{
    Q_OBJECT

public:
    ControlBoxServer(QObject *parent=0);
    ~ControlBoxServer();
    void startServer();
    void run();
    void stopServer();
    void addDigitalOutputModule(DigitalOutputModule *digitalOutputModule);
    void addDigitalInputModule(DigitalInputModule *digitalInputModule);
    UA_Server *server;

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    UA_ServerConfig *config;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;


};
//! [0]

#endif // CONTROLBOXSERVER_H
