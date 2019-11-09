#include <QDebug>
#include <QJsonObject>
#include <QJsonObject>
#include <QJsonParseError>

#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    quint16 port = 1234;

    if(!this->listen(QHostAddress::Any, port)){
        qDebug() << "Не удалось запустить сервер!";
    }
    else{
        qDebug() << "Прослушиваем порт: " << port << "...";
    }
    db.connectToDataBase();
}

Server::~Server()
{

}


void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor)){
        qDebug() << "Неудачное подключение!: " << socketDescriptor << " client";
        return;
    }
    qDebug() << socketDescriptor << " соеденился!";
    socket->write("Hello");

            connect(socket, SIGNAL(readyRead()), this, SLOT(readSock()));
            connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}


void Server::readSock()
{
    QTcpSocket *socket = (qobject_cast<QTcpSocket*>(sender()));
    socket->waitForReadyRead(150);
    QByteArray data = socket->readAll();
    QJsonDocument jsonDoc;
    QJsonParseError jsonErr;
    jsonDoc = QJsonDocument::fromJson(data, &jsonErr);
    if (jsonErr.error == QJsonParseError::NoError)
    {
        QString value = jsonDoc.object().value("type").toString();
        if(value == "auth"){

        }
        else if (value == "register") {
            db.insertIntoTable(jsonDoc.object().value("nick").toString(),
                               jsonDoc.object().value("pass").toString(),
                               jsonDoc.object().value("email").toString()
                        );
            socket->write("register!");
        }
    }
    else {
        qDebug() << "Ошибка c форматом передачи данных! " << jsonErr.errorString();
    }

}


void Server::disconnected()
{
    QTcpSocket *socket = (qobject_cast<QTcpSocket*>(sender()));
    if (socket == nullptr){
        return;
    }
    qDebug() << socket->socketDescriptor() <<  " отсоеденился";
    socket->deleteLater();
}
