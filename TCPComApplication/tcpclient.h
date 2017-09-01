#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>


/*==========================*/
/* Global Macro Definitions */
/*==========================*/
#define m_TCP_CLIENT_DEFAULT_HOST    "127.0.0.1"
#define m_TCP_CLIENT_DEFAULT_PORT    8192
#define m_TCP_CLIENT_DEFAULT_TIMEOUT 60             // Seconds

#define m_TCP_CLIENT_MAX_DATA_SIZE   128


/*==========================*/
/*    Class Declaration     */
/*==========================*/
class TCPClient : public QObject
{
    Q_OBJECT

    QTcpSocket         *_pSocket;
    QByteArray         *_pData;

    QString             _cHost;
    unsigned short      _cPortNum;

    unsigned short      _activityTimeout;


public:
    /* TCP Client Constructors */
    explicit TCPClient(QObject *parent = nullptr);
    explicit TCPClient(QObject *parent,
                       QTcpSocket *pSocket = nullptr);
    explicit TCPClient(QObject *parent,
                       const QString        & host    = m_TCP_CLIENT_DEFAULT_HOST,
                       const unsigned short & port    = m_TCP_CLIENT_DEFAULT_PORT,
                       const unsigned short & timeout = m_TCP_CLIENT_DEFAULT_TIMEOUT);

    /* TCP Client Destructor */
    virtual ~TCPClient();

    /* TCP Server Data Members Accessors */
    const QString        & get_connection_host()  const { return _cHost; }
    const unsigned short & get_connection_port()  const { return _cPortNum; }
    const unsigned short & get_activity_timeout() const { return _activityTimeout; }

    /* TCP Server Utility Functions */
    bool configure(const QString        & host    = m_TCP_CLIENT_DEFAULT_HOST,
                   const unsigned short & port    = m_TCP_CLIENT_DEFAULT_PORT,
                   const unsigned short & timeout = m_TCP_CLIENT_DEFAULT_TIMEOUT);
    bool connectHost();
    bool disconnectHost();
    void reset();
    qint64 write(const QByteArray &);

    /* TCP Server Operator Overload Functions */
    TCPClient & operator = (const TCPClient &);


signals:
    void removeClient(TCPClient *);


public slots:
    void readyRead();           // Read data from socket
    void disconnected();        // Close socket
};

#endif // TCPCLIENT_H
