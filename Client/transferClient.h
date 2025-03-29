#pragma once

#include <Windows.h>
#include <queue>
#include <iostream>
#include <SDL.h>
#include <SDL_syswm.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui_c.h> 


void receiveScreen(int server, SOCKET ConnectSocket);
bool transfer(int row, int col);

