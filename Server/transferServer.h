#pragma once

#include <Windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <mutex>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "MyForm.h"


#pragma comment (lib, "Ws2_32.lib")

void initBitmapHeader(HDC& hScreen, HDC& hdcMem, HBITMAP& hBitmap, BITMAPINFOHEADER& bi, int width, int height, int scrLeft, int scrTop);
void releaseBitmapHeader(HDC& hScreen, HDC& hdcMem, HBITMAP& hBitmap, BITMAPINFOHEADER& bi);
void sendImage(SOCKET ClientSocket);
void transfer();
void acceptClient();