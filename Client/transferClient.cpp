#include "socketClient.h"
#include "kmtransfer.h"
#include "transferClient.h"

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include "MyForm.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define WINDOWNAME "Screen Recorder"

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
bool isQuit = false;
bool isStopSender = false;
bool isStopSharing = false;
std::vector<std::vector<SOCKET>> socketList;
bool isFinishedTransfer;
bool isErrorTransfer;



void receiveScreen(int server, SOCKET ConnectSocket)
{
    while (!isQuit)
    {
        int byteRcv = 0;
        size_t dataSizeNetworkOrder = 0;

        recvPrompt(server, recv(ConnectSocket, (char*)&dataSizeNetworkOrder, sizeof(dataSizeNetworkOrder), 0));
        if (isQuit)
            break;

        size_t dataSize = ntohl(dataSizeNetworkOrder);

        //Receive image
        std::vector<uchar> receivedData(dataSize);
        int totalReceived = 0;

        while (!isQuit && totalReceived < dataSize)
        {
            byteRcv = recv(ConnectSocket, reinterpret_cast<char*>(&receivedData[totalReceived]), dataSize - totalReceived, 0);
            recvPrompt(server, byteRcv);
            totalReceived += byteRcv;
        }

        if (isQuit)
            break;
        cv::Mat imageData = cv::imdecode(receivedData, cv::IMREAD_COLOR);
        SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, imageData.cols, imageData.rows);
        SDL_UpdateTexture(texture, NULL, (void*)imageData.data, imageData.step1());

        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
    }
    char dummy[10000];
    while (recv(ConnectSocket, dummy, sizeof(dummy), 0) == 10000);
}

bool transfer(int row, int col)
{

    int server = row * 3 + col;
    isFinishedTransfer = false;
    isErrorTransfer = false;
    //call thread
    initTransfer(server, &isQuit, &isStopSender, &isStopSharing);
    std::thread quitThread(waitQuit, server, socketList[server][2]);
    std::thread close;
    //set window

    Point desktop = GetDesktopResolution();
    int err = SDL_CreateWindowAndRenderer((int)desktop.x * 0.8, (int)desktop.y * 0.8, SDL_WINDOW_RESIZABLE, &window, &renderer);
    createWindowPrompt(server, err);

    SDL_SetWindowResizable(window, SDL_FALSE);

    SDL_SetWindowTitle(window, WINDOWNAME);

    Point wnd = GetWindowResolutionSDL(window);
    std::thread recvThread(receiveScreen, server, socketList[server][0]);

    //send window size
    if (!isQuit)
        sendPrompt(server, send(socketList[server][1], (char*)&wnd, sizeof(wnd), 0));
    while (!isQuit)
    {
        bool isInput = false;
        INPUT input = getEventSDL2AndConvertWin(window, isQuit, isInput);

        if (isStopSharing)
        {
            close = std::thread(closeTransfer, server, socketList[server][2], &isQuit, &isStopSender);
            break;
        }

        if (isInput)
        {
            if (isQuit)
            {
                close = std::thread(closeTransfer, server, socketList[server][2], &isQuit, &isStopSender);
                break;
            }
            sendPrompt(server, send(socketList[server][1], (char*)&input, sizeof(INPUT), 0));
        }
    }

    //finish transfering
    quitThread.join();
    recvThread.join();
    if (close.joinable())
        close.join();
    clearWindow();
    isFinishedTransfer = true;
    return isErrorTransfer;
}

