#include "socketClient.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "MyForm.h"

#define WSA_VERSION_MAJOR 2
#define WSA_VERSION_MINOR 2
#define BACKLOG 24

void WSAInit()
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(WSA_VERSION_MAJOR, WSA_VERSION_MINOR);
    int wsaErr = WSAStartup(wVersionRequested, &wsaData);
    WSAStartupPrompt(&wsaData, wsaErr);
}

SOCKET sockCreate(int server, int af, int type, int protocol)
{
    SOCKET sock = INVALID_SOCKET;
    sock = socket(af, type, protocol);
    sockCreatePrompt(server, &sock);
    return sock;
}

void closeSocketList(int server, int& nServer)
{
    for (SOCKET& socket : socketList[server])
        closesocket(socket);
    socketList.erase(socketList.begin() + server);
    nServer--;
}

void sockConnect(int server, SOCKET* sock, wchar_t IP[], int af, u_short port)
{
    sockaddr_in clientService;
    clientService.sin_family = af;
    clientService.sin_port = htons(port);
    InetPtonW(af, IP, &clientService.sin_addr.S_un);
    int connectErr = connect(*sock, (sockaddr*)&clientService, sizeof(clientService));
    connectPrompt(server, sock, connectErr);
}

void clearWindow()
{
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
}

void WSAStartupPrompt(WSADATA* wsaData, int err)
{
    using namespace remote_desktop_client;
    if (err)
    {
        MessageBox::Show("Winsock library initialization failed!");
    }
}

void sockCreatePrompt(int server, SOCKET* socket)
{
    using namespace remote_desktop_client;
    if (*socket == SOCKET_ERROR)
    {
        MessageBox::Show("Error creating socket!");
        clearWindow();
        closeSocketList(server, nServer);
    }
}

void connectPrompt(int server, SOCKET* socket, int connectErr)
{
    using namespace remote_desktop_client;
    if (connectErr == SOCKET_ERROR)
    {
        MessageBox::Show("Connection failed!");
        clearWindow();
        closeSocketList(server, nServer);
    }
}

extern bool isErrorTransfer;
extern bool isQuit;
extern bool isStopSender;

void sendPrompt(int server, int byteCount)
{
    using namespace remote_desktop_client;
    if (byteCount == SOCKET_ERROR)
    {
        isErrorTransfer = true;
        isQuit = true;
        MessageBox::Show("Sending failed!");
    }
}

void recvPrompt(int server, int byteCount)
{
    using namespace remote_desktop_client;
    if (byteCount == SOCKET_ERROR)
    {
        isErrorTransfer = true;
        isQuit = true;
        MessageBox::Show("Accept failure!");
    }
}

void createWindowPrompt(int server, int err)
{
    using namespace remote_desktop_client;
    if (err == -1)
    {
        clearWindow();
        closeSocketList(server, nServer);
    }
}

void closeTransfer(int server, SOCKET closeSocket, bool* isQuit, bool* isStopSender)
{
    *isQuit = true;
    *isStopSender = true;

    u_long iMode = 1;
    for (int i = 0; i < 2; i++)
        ioctlsocket(socketList[server][i], FIONBIO, &iMode);

    sendPrompt(server, send(closeSocket, (char*)isQuit, sizeof(bool), 0));
}

void initTransfer(int server, bool* isQuit, bool* isStopSender, bool* isStopSharing)
{
    *isQuit = false;
    *isStopSender = false;
    *isStopSharing = false;

    u_long iMode = 0;
    for (int i = 0; i < 2; i++)
        ioctlsocket(socketList[server][i], FIONBIO, &iMode);
}

void waitQuit(int server, SOCKET quitSocket)
{
    if (recv(quitSocket, (char*)&isQuit, sizeof(isQuit), 0) == SOCKET_ERROR)
        return;
    u_long iMode = 1;
    for (int i = 0; i < 2; i++)
        ioctlsocket(socketList[server][i], FIONBIO, &iMode);

    for (int i = 0; i < 2; i++)
    {
        char dummy;
        send(socketList[server][i], &dummy, sizeof(dummy), 0);
    }

    if (!isStopSender)
        sendPrompt(server, send(quitSocket, (char*)&isQuit, sizeof(isQuit), 0));
}

bool clientConnect(int& nServer, wchar_t IP[])
{
    nServer++;
    int server = nServer - 1;
    socketList.push_back(std::vector<SOCKET>(4));
    for (int i = 0; i < 4; i++)
    {
        socketList[server][i] = sockCreate(server, AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socketList.size() == server) //error occur
            return 0;
    }
    sockConnect(server, &socketList[server][0], IP, AF_INET, PORTSCREEN); //screen transfer
    if (socketList.size() == server) //error occur
        return 0;
    sockConnect(server, &socketList[server][1], IP, AF_INET, PORTKM); //key transfer
    if (socketList.size() == server) //error occur
        return 0;
    sockConnect(server, &socketList[server][2], IP, AF_INET, PORTQUIT); //quit transfer
    if (socketList.size() == server) //error occur
        return 0;
    sockConnect(server, &socketList[server][3], IP, AF_INET, PORTSIGNAL);
    if (socketList.size() == server) //error occur
        return 0;
    return 1;
}

bool sendSignal(SOCKET socket, short signal)
{
    if (send(socket, (char*)&signal, sizeof(signal), 0) == SOCKET_ERROR) return false;
    return true;
}