#include <QCoreApplication>
#include <QDebug>

#include "tcpserver.h"
#include "tcpclient.h"


TCPServer Server;
TCPClient Client;


void removeClient(TCPClient*)
{
    qDebug() << "removeClient - main.c" << endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* Setup Server */
    Server.configure("127.0.0.1", 8192, 60);
    Server.startServer();

    /* Setup Client */
    Client.configure("127.0.0.1", 8192, 60);
    //connect(&Client, SIGNAL(removeClient(TCPClient*)), nullptr, SLOT(removeClient(TCPClient*)));

    /* Send Data to Server */
    Client.write("Hello, this is client from main.c");

    /* Receive Echo Response */
    /* Close Client */
    /* Close Server */

    return a.exec();
}
