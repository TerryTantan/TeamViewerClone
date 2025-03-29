#pragma once
#include <vector>
#include <WinSock2.h>
#include "MyForm.h"

#define PORTSCREEN 27016
#define PORTKM 19156
#define PORTQUIT 32012
#define PORTSIGNAL 14341
#define TRANSFER 1
#define STOP 2


extern std::vector<SOCKET> socketList;

	void WSAInit();
	SOCKET sockCreate(int af, int type, int protocol);
	void closeSocketList();
	void sockBind(int af, SOCKET* socketBind, char* IP, u_short port);
	void sockListen(SOCKET* sock);
	SOCKET sockAccp(SOCKET* sock);
	void WSAStartupPrompt(WSADATA* wsaData, int err);
	void sockCreatePrompt(SOCKET* socket);
	void sockBindPrompt(SOCKET* socket, int bindErr);
	void listenPrompt(int listenErr);
	void acceptPrompt(SOCKET* socket);
	void sendPrompt(int byteCount);
	void recvPrompt(int byteCount);

	void closeTransfer(SOCKET closeSocket, bool* isQuit, bool* isStopSender);
	void initTransfer(bool* isQuit, bool* isStopSender, bool* isStopSharing);
	void waitQuit(SOCKET quitSocket);