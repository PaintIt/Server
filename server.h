#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

#include "database.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr);
    ~Server();

private:
    DataBase db;

protected:
    void incomingConnection(qintptr socketDescriptor);


public slots:
    void readSock();
    void disconnected();


signals:
    void error(QTcpSocket::SocketError socketerror);
};

#endif // SERVER_H
