#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <winsock2.h>
#include <string>

class SocketManager {
public:
    SocketManager(int port);
    ~SocketManager();

    bool initialize();
    SOCKET acceptClient();
    std::string receiveData(SOCKET clientSocket);
    void sendData(SOCKET clientSocket, const std::string& data);
    void closeSocket(SOCKET socket);

private:
    int port;
    SOCKET serverSocket;
};

#endif // SOCKET_MANAGER_H
