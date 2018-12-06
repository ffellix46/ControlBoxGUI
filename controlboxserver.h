#ifndef CONTROLBOXSERVER_H
#define CONTROLBOXSERVER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>
#include <QObject>

//! [0]
class ControlBoxServer : public QThread{
    Q_OBJECT

public:
    ControlBoxServer(QObject *parent=0);
    ~ControlBoxServer();
    void transaction(const QString &portName, int waitTimeout, const QString &request);
    void run();
    void stopServer();

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    QString portName;
    QString request;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;


};
//! [0]

#endif // CONTROLBOXSERVER_H
