#pragma once

#include <Windows.h>
#include <SDL.h>
#include <SDL_syswm.h>

struct Point
{
    double x, y;
};

#define INPUT_WINDOW_CHANGED_SIZE 3

Point GetWindowResolutionSDL(SDL_Window* window);
Point GetDesktopResolution();
short SDLtoWINKeyCode(SDL_Keycode sdl);
INPUT getEventSDL2AndConvertWin(SDL_Window* window, bool& isQuit, bool& isInput);