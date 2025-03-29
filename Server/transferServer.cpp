#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include "transferServer.h"
#include "kmtransfer.h"
#include "socketServer.h"

std::mutex mutexImageBufferList;
std::vector<SOCKET> socketList;
bool isQuit = false;
bool isStopSender = false;
bool isStopSharing = false;
short signaling;

void initBitmapHeader(HDC& hScreen, HDC& hdcMem, HBITMAP& hBitmap, BITMAPINFOHEADER& bi, int width, int height, int scrLeft, int scrTop)
{
    //Capture sreen shot
    hScreen = GetDC(NULL);
    hdcMem = CreateCompatibleDC(hScreen);
    hBitmap = CreateCompatibleBitmap(hScreen, width, height);
    SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, width, height, hScreen, scrLeft, scrTop, SRCCOPY);

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  // Đảo chiều cao để đúng chiều
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
}

void releaseBitmapHeader(HDC& hScreen, HDC& hdcMem, HBITMAP& hBitmap, BITMAPINFOHEADER& bi)
{
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hScreen);
}

void sendImage(SOCKET ClientSocket)
{
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    int screenLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screenTop = GetSystemMetrics(SM_YVIRTUALSCREEN);
    HDC hScreen, hdcMem;
    HBITMAP hBitmap;
    BITMAPINFOHEADER bi;
    std::thread close; //close transfering signal

    while (!isQuit)
    {
        initBitmapHeader(hScreen, hdcMem, hBitmap, bi, width, height, screenLeft, screenTop);
        cv::Mat image(-bi.biHeight, bi.biWidth, CV_8UC4);
        GetDIBits(hdcMem, hBitmap, 0, bi.biHeight, image.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
        releaseBitmapHeader(hScreen, hdcMem, hBitmap, bi);

        std::vector<uchar> image_buffer;
        cv::imencode(".jpg", image, image_buffer, { cv::IMWRITE_JPEG_QUALITY, 90 });

        //Send datasize first
        size_t datasize = image_buffer.size();
        size_t datasizeNetworkOrder = htonl(datasize); //Change into byte order
        if (isQuit) break;
        sendPrompt(send(ClientSocket, (const char*)&datasizeNetworkOrder, sizeof(datasizeNetworkOrder), 0));
        if (isQuit) break;
        //Send image
        sendPrompt(send(ClientSocket, (const char*)image_buffer.data(), image_buffer.size(), 0));
        if (isStopSharing)
        {
            close = std::thread(closeTransfer, socketList[2], &isQuit, &isStopSender);
            break;
        }
    }
    if (close.joinable())
        close.join();
}

void transfer()
{
    initTransfer(&isQuit, &isStopSender, &isStopSharing); //initialization for tranfering

    //call thread
    std::vector<std::thread> captureThreads;
    std::thread quitThread(waitQuit, socketList[2]);
    std::thread sendThread(sendImage, socketList[0]);

    //receive key
    Point desktop = GetDesktopResolution();
    Point cliWin;

    recvPrompt(recv(socketList[1], (char*)&cliWin, sizeof(cliWin), 0)); //get client window size for scaling
    Point system = { (double)GetSystemMetrics(SM_CXSCREEN), (double)GetSystemMetrics(SM_CYSCREEN) }; //get server desktop size for scacling
    while (!isQuit)
    {
        INPUT input;
        recvPrompt(recv(socketList[1], (char*)&input, sizeof(INPUT), 0));

        if (input.type == INPUT_MOUSE)
        {
            input.mi.dx = ceil(1.0 * input.mi.dx / cliWin.x * desktop.x) / system.x * 65535;
            input.mi.dy = ceil(1.0 * input.mi.dy / cliWin.y * desktop.y) / system.y * 65535;
        }

        if (input.type == INPUT_WINDOW_CHANGED_SIZE)
            memcpy(&cliWin, (char*)&input + sizeof(input.type), sizeof(cliWin));
        if (isQuit) 
            break;
        SendInput(1, &input, sizeof(INPUT)); //send event into OS
    }

    //for close transfering
    char dummy[10000];
    while (recv(socketList[1], dummy, sizeof(dummy), 0) == 10000);

    //thread join
    quitThread.join();
    sendThread.join();
}

void acceptClient() {
    
    for (int i = 0; i < 4; i++)
    {
        SOCKET tempSock = sockAccp(&socketList[i]);
        closesocket(socketList[i]);
        socketList[i] = tempSock;
    }

    do
    {
        recv(socketList[3], (char*)&signaling, sizeof(signaling), 0);
        if (signaling == TRANSFER)
            transfer();

    } while (signaling != STOP);
    remote_desktop_server::MessageBox::Show("The Client has been disconnected, press the \"Disconnect\" button to exit");
}