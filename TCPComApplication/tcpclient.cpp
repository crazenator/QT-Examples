#include "tcpclient.h"


/*==============================*/
/* Class Contructors Definition */
/*==============================*/
TCPClient::TCPClient(QObject *parent) : QObject(parent)
{
    reset();
}

TCPClient::TCPClient(QObject *parent, QTcpSocket *pSocket) : QObject(parent)
{
    if (pSocket == nullptr)
    {
        return;
    }

    /* Initialize socket */
    _pSocket = nullptr;

    /* Configure client parameters */
    configure(pSocket->peerName(),
              pSocket->peerPort(),
              m_TCP_CLIENT_DEFAULT_TIMEOUT);

    /* Assign accepted socket */
    _pSocket = pSocket;

    _pData = new QByteArray(m_TCP_CLIENT_MAX_DATA_SIZE, '\0');

    /* Connect signal for data read */
    connect(_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    /* Connect signal for socket disconnection */
    connect(_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

TCPClient::TCPClient(QObject *parent,
                     const QString &host,
                     const unsigned short &port,
                     const unsigned short &timeout) : QObject(parent)
{
    _pSocket = nullptr;

    /* Configure client parameters */
    configure(host, port, timeout);

    /* Connect to host */
    connectHost();
}


/*==============================*/
/* Class Destructor Definition  */
/*==============================*/
TCPClient::~TCPClient()
{
    /* Disconnect client from host */
    disconnectHost();
}


/*==========================*/
/* Private Function Members */
/*==========================*/


/*==========================*/
/* Public Function Members  */
/*==========================*/
bool TCPClient::configure(const QString &host,
                          const unsigned short &port,
                          const unsigned short &timeout)
{
    if (_pSocket != nullptr)
    {
        /* Already connected or un-initialized client */
        return false;
    }

    /* Populate client parameters */
    _cHost           = host;
    _cPortNum        = port;
    _activityTimeout = timeout;

    return true;
}

bool TCPClient::connectHost()
{
    if (_pSocket != nullptr)
    {
        /* Already connected or un-initialized client */
        return false;
    }

    _pSocket = new QTcpSocket(this);
    if (_pSocket == nullptr)
    {
        return false;
    }

    /* Allocate memory for data */
    _pData = new QByteArray(m_TCP_CLIENT_MAX_DATA_SIZE, '\0');
    if (_pData == nullptr)
    {
        delete _pSocket;
        return false;
    }

    /* Connect signal for data read */
    connect(_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    /* Connect signal for socket disconnection */
    connect(_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    /* Connect to host */
    _pSocket->connectToHost(_cHost, _cPortNum);

    return true;
}

bool TCPClient::disconnectHost()
{
    if (_pSocket == nullptr)
    {
        return false;
    }

    /* Disconnect signal for data read */
    disconnect(_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    /* Disconnect signal for disconnection */
    disconnect(_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    /* Disconnect from host */
    _pSocket->close();

    delete _pData; _pData = nullptr;
    delete _pSocket; _pSocket = nullptr;

    return true;
}

void TCPClient::reset()
{
    _pSocket = nullptr;
    _pData   = nullptr;

    /* Initialize server parameters */
    _cHost           = m_TCP_CLIENT_DEFAULT_HOST;
    _cPortNum        = m_TCP_CLIENT_DEFAULT_PORT;
    _activityTimeout = m_TCP_CLIENT_DEFAULT_TIMEOUT;
}

qint64 TCPClient::write(const QByteArray & data)
{
    return _pSocket->write(data);
}


/*=============================*/
/* Operator Overload Functions */
/*=============================*/
TCPClient & TCPClient::operator = (const TCPClient & ref_obj)
{
    if (this != &ref_obj)
    {
        /* Populate server parameters */
        _cHost           = ref_obj.get_connection_host();
        _cPortNum        = ref_obj.get_connection_port();
        _activityTimeout = ref_obj.get_activity_timeout();
    }

    return *this;
}


/*==========================*/
/*     Slots Definition     */
/*==========================*/
void TCPClient::readyRead()
{
    if (_pData != nullptr)
    {
        _pSocket->read(_pData->data(), m_TCP_CLIENT_MAX_DATA_SIZE);
        qDebug() << _pData->data() << endl;
    }
}

void TCPClient::disconnected()
{
    disconnectHost();

    /* Signal for the disconnection */
    emit removeClient(this);
}
