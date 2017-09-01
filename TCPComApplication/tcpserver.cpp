#include "tcpserver.h"


/*==============================*/
/* Class Contructors Definition */
/*==============================*/
TCPServer::TCPServer(QObject *parent) : QObject(parent)
{
    reset();
}

TCPServer::TCPServer(QObject *parent,
                     const QString &ip,
                     const unsigned short &port,
                     const unsigned short &timeout) : QObject(parent)
{
    _pServer = nullptr;

    /* Clear clients list */
    _clients.clear();

    /* Configure server parameters */
    configure(ip, port, timeout);

    /* Start server */
    startServer();
}


/*==============================*/
/* Class Destructor Definition  */
/*==============================*/
TCPServer::~TCPServer()
{
    /* Stop server */
    stopServer();

    /* Destroy all clients */
    while ( !_clients.isEmpty() )
    {
        shutdownClient(_clients.takeFirst());
    }
}


/*==========================*/
/* Private Function Members */
/*==========================*/
bool TCPServer::setupClient(QTcpSocket *pSocket)
{
    /* Create client */
    TCPClient *pClient = new TCPClient(this, pSocket);
    if (pClient == nullptr)
    {
        return false;
    }

    /* Add client to the list */
    _clients.append(pClient);

    /* Connect signal for client removal */
    connect(pClient, SIGNAL(removeClient(TCPClient*)), this, SLOT(removeClient(TCPClient*)));

    return true;
}

bool TCPServer::shutdownClient(TCPClient *pClient)
{
    if (pClient == nullptr)
    {
        return false;
    }

    /* Disconnect signal for client removal */
    disconnect(pClient, SIGNAL(removeClient(TCPClient*)), this, SLOT(removeClient(TCPClient*)));

    /* Destroy client */
    delete pClient;

    return true;
}


/*==========================*/
/* Public Function Members  */
/*==========================*/
bool TCPServer::configure(const QString &ip,
                          const unsigned short &port,
                          const unsigned short &timeout)
{
    if (_pServer != nullptr)
    {
        /* Already started or un-initialized server */
        return false;
    }

    /* Populate server parameters */
    _lInterfaceIP    = ip;
    _lPortNum        = port;
    _activityTimeout = timeout;

    return true;
}

void TCPServer::reset()
{
    _pServer = nullptr;

    /* Clear clients list */
    _clients.clear();

    /* Initialize server parameters */
    _lInterfaceIP    = m_TCP_SERVER_DEFAULT_IP;
    _lPortNum        = m_TCP_SERVER_DEFAULT_PORT;
    _activityTimeout = m_TCP_SERVER_DEFAULT_TIMEOUT;
}

bool TCPServer::startServer()
{
    if (_pServer != nullptr)
    {
        /* Already started or un-initialized server */
        return false;
    }

    /* Create TCP listening server */
    _pServer = new QTcpServer(this);
    if (_pServer == nullptr)
    {
        return false;
    }

    /* Connect signal for new connections */
    connect(_pServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    /* Start listenig for in-coming connections */
    _pServer->listen(QHostAddress(_lInterfaceIP), _lPortNum);

    return true;
}

bool TCPServer::stopServer()
{
    if (_pServer == nullptr)
    {
        /* Already stopped or not started */
        return false;
    }

    /* Disconnect signal for new connections */
    disconnect(_pServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    /* Close server */
    _pServer->close();
    delete _pServer;
    _pServer = nullptr;

    return true;
}


/*=============================*/
/* Operator Overload Functions */
/*=============================*/
TCPServer & TCPServer::operator = (const TCPServer & ref_obj)
{
    if (this != &ref_obj)
    {
        /* Populate server parameters */
        _lInterfaceIP    = ref_obj.get_listen_ip();
        _lPortNum        = ref_obj.get_listen_port();
        _activityTimeout = ref_obj.get_activity_timeout();
    }

    return *this;
}


/*==========================*/
/*     Slots Definition     */
/*==========================*/
void TCPServer::newConnection()
{
    QTcpSocket *pSocket = _pServer->nextPendingConnection();
    if (pSocket != nullptr)
    {
        qDebug() << pSocket->peerName() << ":" << pSocket->peerPort() << endl;
        if (setupClient(pSocket) == false)
        {
            pSocket->close();
            delete pSocket;
        }
    }
}

void TCPServer::removeClient(TCPClient *pClient)
{
    if (pClient != nullptr)
    {
        for (int idx = 0; idx < _clients.count(); idx++)
        {
            if (_clients.at(idx) == pClient)
            {
                _clients.removeAt(idx);
                shutdownClient(pClient);
                break;
            }
        }
    }
}
