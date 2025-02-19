#ifndef SERVER_H
#define SERVER_H

#include "SocketManager.h"
#include "RequestHandler.h"
#include <string>

class Server {
public:
    Server(int port);
    ~Server();

    void run();

private:
    int port;
    SocketManager socketManager;
    RequestHandler requestHandler;

    void handleClient(SOCKET clientSocket);
};

#endif // SERVER_H
