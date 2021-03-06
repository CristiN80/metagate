#include "ProxyServer.h"

#include "ProxyClient.h"

#include "Log.h"

#include <QThread>
#include <QTimer>

SET_LOG_NAMESPACE("PRX");

namespace proxy
{

const quint16 defaultPort = 1234;

ProxyServer::ProxyServer(QObject *parent)
    : QTcpServer(parent)
    , m_port(defaultPort)
{
}

quint16 ProxyServer::port() const
{
    return m_port;
}

void ProxyServer::setPort(quint16 p)
{
    m_port = p;
}

bool ProxyServer::start()
{
    bool r = listen(QHostAddress::Any, m_port);
    emit listeningChanged(isListening());
    //qDebug() << serverError();
    return r;
}

void ProxyServer::stop()
{
    close();
    emit stopClient();
    emit listeningChanged(isListening());
}

void ProxyServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "!!!!!!!!!!!!!! connect";
    QThread *thread = new QThread(this);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    ProxyClient *client = new ProxyClient();
    connect(this, &ProxyServer::stopClient, client, &ProxyClient::stop);
    clients.append(client);
    connect(client, &ProxyClient::destroyed, [thread, client, this]() {
        qDebug() << "Start disc";
        QTimer::singleShot(60 * 1000, [thread, client, this](){
            thread->quit();
            clients.removeAll(client);
            qDebug() << "Dicsc disc";
            updateClients();
        });
        //milliseconds(1s).count()
    });
    client->setSocketDescriptor(socketDescriptor);
    client->moveToThread(thread);
    thread->start();
    updateClients();
}

void ProxyServer::updateClients()
{
    emit connectedPeersChanged(clients.count());
}
}
