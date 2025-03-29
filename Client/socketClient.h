#pragma once
#include <vector>
#include <WinSock2.h>
#include <SDL.h>

#define PORTSCREEN 27016
#define PORTKM 19156
#define PORTQUIT 32012
#define PORTSIGNAL 14341
#define TRANSFER 1
#define STOP 2

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern std::vector<std::vector<SOCKET>> socketList;
extern int nServer;

void WSAInit();
SOCKET sockCreate(int server, int af, int type, int protocol);
void closeSocketList(int server, int& nServer);
void sockConnect(int server, SOCKET* sock, wchar_t IP[], int af, u_short port);
void clearWindow();
void WSAStartupPrompt(WSADATA* wsaData, int err);
void sockCreatePrompt(int server, SOCKET* socket);
void connectPrompt(int server, SOCKET* socket, int connectErr);
void sendPrompt(int server, int byteCount);
void recvPrompt(int server, int byteCount);
void createWindowPrompt(int server, int err);

void closeTransfer(int server, SOCKET closeSocket, bool* isQuit, bool* isStopSender);
void initTransfer(int server, bool* isQuit, bool* isStopSender, bool* isStopSharing);
void waitQuit(int server, SOCKET quitSocket);
bool clientConnect(int& nServer, wchar_t IP[]);
bool sendSignal(SOCKET socket, short signal);
