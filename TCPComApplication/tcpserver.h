#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>

#include <QTcpServer>
#include <QTcpSocket>

#include "tcpclient.h"


/*==========================*/
/* Global Macro Definitions */
/*==========================*/
#define m_TCP_SERVER_DEFAULT_IP      "127.0.0.1"
#define m_TCP_SERVER_DEFAULT_PORT    8192
#define m_TCP_SERVER_DEFAULT_TIMEOUT 60             // Seconds


/*==========================*/
/*    Class Declaration     */
/*==========================*/
class TCPServer : public QObject
{
    Q_OBJECT

    QList<TCPClient *>  _clients;
    QTcpServer         *_pServer;

    QString             _lInterfaceIP;
    unsigned short      _lPortNum;

    unsigned short      _activityTimeout;

    /* TCP Server Utility Functions */
    bool setupClient(QTcpSocket *);
    bool shutdownClient(TCPClient *);

public:
    /* TCP Server Constructors */
    explicit TCPServer(QObject *parent = nullptr);
    explicit TCPServer(QObject *parent,
                       const QString        & ip      = m_TCP_SERVER_DEFAULT_IP,
                       const unsigned short & port    = m_TCP_SERVER_DEFAULT_PORT,
                       const unsigned short & timeout = m_TCP_SERVER_DEFAULT_TIMEOUT);

    /* TCP Server Destrutor */
    virtual ~TCPServer();

    /* TCP Server Data Members Accessors */
    const QString        & get_listen_ip()        const { return _lInterfaceIP; }
    const unsigned short & get_listen_port()      const { return _lPortNum; }
    const unsigned short & get_activity_timeout() const { return _activityTimeout; }

    /* TCP Server Utility Functions */
    bool configure(const QString        & ip      = m_TCP_SERVER_DEFAULT_IP,
                   const unsigned short & port    = m_TCP_SERVER_DEFAULT_PORT,
                   const unsigned short & timeout = m_TCP_SERVER_DEFAULT_TIMEOUT);
    void reset();

    bool startServer();
    bool stopServer();

    /* TCP Server Operator Overload Functions */
    TCPServer & operator = (const TCPServer &);


signals:


public slots:
    void newConnection();           // Accepts new connections
    void removeClient(TCPClient *); // Close and remove client from the list
};

#endif // TCPSERVER_H
