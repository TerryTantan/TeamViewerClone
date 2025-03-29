#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "socketServer.h"
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

SOCKET sockCreate(int af, int type, int protocol)
{
    SOCKET sock = INVALID_SOCKET;
    sock = socket(af, type, protocol);
    sockCreatePrompt(&sock);
    return sock;
}

void closeSocketList()
{
    for (SOCKET& socket : socketList)
        closesocket(socket);
}

void sockBind(int af, SOCKET* socketBind, char* IP, u_short port)
{
    sockaddr_in addr;
    addr.sin_family = af;
    addr.sin_port = htons(port);

    inet_pton(AF_INET, IP, &addr.sin_addr.S_un);
    int bindErr = bind(*socketBind, (sockaddr*)&addr, sizeof(addr));
    sockBindPrompt(socketBind, bindErr);
}

void sockListen(SOCKET* sock)
{
    int listenErr = listen(*sock, BACKLOG);
    listenPrompt(listenErr);
}

SOCKET sockAccp(SOCKET* sock)
{
    SOCKET sockAcp = accept(*sock, nullptr, NULL);
    acceptPrompt(&sockAcp);
    return sockAcp;
}

void WSAStartupPrompt(WSADATA* wsaData, int err)
{
    using namespace remote_desktop_server;
    if (err)
    {
        MessageBox::Show("Initialization of winsock library failed!");
    }
}

void sockCreatePrompt(SOCKET* socket)
{
    using namespace remote_desktop_server;
    if (*socket == SOCKET_ERROR)
    {
        MessageBox::Show("Socket creation error!");
        closeSocketList();
        WSACleanup();
    }
}

void sockBindPrompt(SOCKET* socket, int bindErr)
{
    using namespace remote_desktop_server;
    if (bindErr == SOCKET_ERROR)
    {
        MessageBox::Show("IP address assignment error!");
        closeSocketList();

        WSACleanup();
    }
}

void listenPrompt(int listenErr)
{
    using namespace remote_desktop_server;
    if (listenErr == SOCKET_ERROR)
    {
        MessageBox::Show("Error searching for connection!");
        closeSocketList();

        WSACleanup();
    }
}

void acceptPrompt(SOCKET* socket)
{
    using namespace remote_desktop_server;
    if (*socket == INVALID_SOCKET)
    {
        MessageBox::Show("Accept connection failure!");
        closeSocketList();

        WSACleanup();
    }
}

extern bool isQuit;
extern bool isStopSender;
extern short signaling;

void sendPrompt(int byteCount)
{
    using namespace remote_desktop_server;
    if (byteCount == SOCKET_ERROR)
    {
        isQuit = true;
        signaling = STOP;
    }
}

void recvPrompt(int byteCount)
{
    using namespace remote_desktop_server;
    if (byteCount == SOCKET_ERROR)
    {
        isQuit = true;
        signaling = STOP;
    }
}

void closeTransfer(SOCKET closeSocket, bool* isQuit, bool* isStopSender)
{
    *isQuit = true;
    *isStopSender = true;

    u_long iMode = 1;
    for (int i = 0; i < 2; i++)
        ioctlsocket(socketList[i], FIONBIO, &iMode);

    sendPrompt(send(closeSocket, (char*)isQuit, sizeof(bool), 0));
}

void initTransfer(bool* isQuit, bool* isStopSender, bool* isStopSharing)
{
    *isQuit = false;
    *isStopSender = false;
    *isStopSharing = false;

    u_long iMode = 0;
    for (int i = 0; i < 2; i++)
        ioctlsocket(socketList[i], FIONBIO, &iMode);
}

void waitQuit(SOCKET quitSocket)
{
    if (recv(quitSocket, (char*)&isQuit, sizeof(isQuit), 0) == SOCKET_ERROR)
        return;
    u_long iMode = 1;
    for (int i = 0; i < 2; i++)
        ioctlsocket(socketList[i], FIONBIO, &iMode);

    for (int i = 0; i < 2; i++)
    {
        char dummy;
        send(socketList[i], &dummy, sizeof(dummy), 0);
    }

    if (!isStopSender)
        sendPrompt(send(quitSocket, (char*)&isQuit, sizeof(isQuit), 0));
}