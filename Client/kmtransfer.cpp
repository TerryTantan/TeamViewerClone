#include "kmtransfer.h"
#include <iostream>
#include <chrono>

Point GetWindowResolutionSDL(SDL_Window* window)
{
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);

    RECT windowRect;
    GetClientRect(wmInfo.info.win.window, &windowRect);
    return { (double)windowRect.right - windowRect.left, (double)windowRect.bottom - windowRect.top };
}

Point GetDesktopResolution()
{
    double horizontal, vertical;
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();

    GetWindowRect(hDesktop, &desktop);
    horizontal = (double)desktop.right;
    vertical = (double)desktop.bottom;

    return { horizontal, vertical };
}

short SDLtoWINKeyCode(SDL_Keycode sdl)
{
    bool isCapslock = (GetKeyState(VK_NUMLOCK) & 0x01);
    switch (sdl)
    {
    case SDLK_0:
        return 0x30;
    case SDLK_1:
        return 0x31;
    case SDLK_2:
        return 0x32;
    case SDLK_3:
        return 0x33;
    case SDLK_4:
        return 0x34;
    case SDLK_5:
        return 0x35;
    case SDLK_6:
        return 0x36;
    case SDLK_7:
        return 0x37;
    case SDLK_8:
        return 0x38;
    case SDLK_9:
        return 0x39;

    case SDLK_a:
        return 0x41;
    case SDLK_b:
        return 0x42;
    case SDLK_c:
        return 0x43;
    case SDLK_d:
        return 0x44;
    case SDLK_e:
        return 0x45;
    case SDLK_f:
        return 0x46;
    case SDLK_g:
        return 0x47;
    case SDLK_h:
        return 0x48;
    case SDLK_i:
        return 0x49;
    case SDLK_j:
        return 0x4A;
    case SDLK_k:
        return 0x4B;
    case SDLK_l:
        return 0x4C;
    case SDLK_m:
        return 0x4D;
    case SDLK_n:
        return 0x4E;
    case SDLK_o:
        return 0x4F;
    case SDLK_p:
        return 0x50;
    case SDLK_q:
        return 0x51;
    case SDLK_r:
        return 0x52;
    case SDLK_s:
        return 0x53;
    case SDLK_t:
        return 0x54;
    case SDLK_u:
        return 0x55;
    case SDLK_v:
        return 0x56;
    case SDLK_w:
        return 0x57;
    case SDLK_x:
        return 0x58;
    case SDLK_y:
        return 0x59;
    case SDLK_z:
        return 0x5A;
    case SDLK_TAB:
        return VK_TAB;
    case SDLK_CAPSLOCK:
        return VK_CAPITAL;
    case SDLK_SPACE:
        return VK_SPACE;
    case SDLK_BACKSPACE:
        return VK_BACK;
    case SDLK_RETURN:
        return VK_RETURN;
    case SDLK_RSHIFT:
        return VK_RSHIFT;
    case SDLK_LSHIFT:
        return VK_LSHIFT;
    case SDLK_RCTRL:
        return VK_RCONTROL;
    case SDLK_LCTRL:
        return VK_LCONTROL;
    case SDLK_RGUI:
        return VK_RWIN;
    case SDLK_LGUI:
        return VK_LWIN;
    case SDLK_RALT:
        return VK_RMENU;
    case SDLK_LALT:
        return VK_LMENU;
    case SDLK_UP:
        return VK_UP;
    case SDLK_DOWN:
        return VK_DOWN;
    case SDLK_LEFT:
        return VK_LEFT;
    case SDLK_RIGHT:
        return VK_RIGHT;
    case SDLK_PAGEUP:
        return VK_PRIOR;
    case SDLK_PAGEDOWN:
        return VK_NEXT;
    case SDLK_HOME:
        return VK_HOME;
    case SDLK_END:
        return VK_END;

    case SDLK_NUMLOCKCLEAR:
        return VK_NUMLOCK;
    case SDLK_KP_DIVIDE:
        return VK_DIVIDE;
    case SDLK_KP_MULTIPLY:
        return VK_MULTIPLY;
    case SDLK_KP_MINUS:
        return VK_SUBTRACT;
    case SDLK_KP_PLUS:
        return VK_ADD;
    case SDLK_KP_ENTER:
        return VK_RETURN;

    case SDLK_MINUS:
        return VK_OEM_MINUS;
    case SDLK_EQUALS:
        return VK_OEM_PLUS;
    case SDLK_LEFTBRACKET:
        return VK_OEM_4;
    case SDLK_RIGHTBRACKET:
        return VK_OEM_6;
    case SDLK_BACKSLASH:
        return VK_OEM_5;
    case SDLK_SEMICOLON:
        return VK_OEM_1;
    case SDLK_QUOTE:
        return VK_OEM_7;
    case SDLK_COMMA:
        return VK_OEM_COMMA;
    case SDLK_PERIOD:
        return VK_OEM_PERIOD;
    case SDLK_SLASH:
        return VK_OEM_2;
    case SDLK_BACKQUOTE:
        return VK_OEM_3;
    case SDLK_ESCAPE:
        return VK_ESCAPE;

    case SDLK_F1:
        return VK_F1;
    case SDLK_F2:
        return VK_F2;
    case SDLK_F3:
        return VK_F3;
    case SDLK_F4:
        return VK_F4;
    case SDLK_F5:
        return VK_F5;
    case SDLK_F6:
        return VK_F6;
    case SDLK_F7:
        return VK_F7;
    case SDLK_F8:
        return VK_F8;
    case SDLK_F9:
        return VK_F9;
    case SDLK_F10:
        return VK_F10;
    case SDLK_F11:
        return VK_F11;
    case SDLK_F12:
        return VK_F12;
    case SDLK_F13:
        return VK_F13;
    case SDLK_F14:
        return VK_F14;
    case SDLK_F15:
        return VK_F15;
    case SDLK_F16:
        return VK_F16;
    case SDLK_F17:
        return VK_F17;
    case SDLK_F18:
        return VK_F18;
    case SDLK_F19:
        return VK_F19;
    case SDLK_F20:
        return VK_F20;
    case SDLK_F21:
        return VK_F21;
    case SDLK_F22:
        return VK_F22;
    case SDLK_F23:
        return VK_F23;
    case SDLK_F24:
        return VK_F24;

    case SDLK_PRINTSCREEN:
        return VK_PRINT;
    case SDLK_DELETE:
        return VK_DELETE;
    case SDLK_HELP:
        return VK_HELP;
    case SDLK_VOLUMEDOWN:
        return VK_VOLUME_DOWN;
    case SDLK_VOLUMEUP:
        return VK_VOLUME_UP;
    case SDLK_AUDIOMUTE:
        return VK_VOLUME_MUTE;
    case SDLK_AUDIOPREV:
        return VK_MEDIA_PREV_TRACK;
    case SDLK_AUDIONEXT:
        return VK_MEDIA_NEXT_TRACK;
    case SDLK_AUDIOPLAY:
        return VK_MEDIA_PLAY_PAUSE;

    case SDLK_INSERT:
        return VK_INSERT;

    case SDLK_KP_0:
    {
        if (isCapslock)
            return 0x30;
        return VK_INSERT;
    }
    case SDLK_KP_1:
    {
        if (isCapslock)
            return 0x31;
        return VK_END;
    }

    case SDLK_KP_2:
    {
        if (isCapslock)
            return 0x32;
        return VK_DOWN;
    }
    case SDLK_KP_3:
    {
        if (isCapslock)
            return 0x33;
        return VK_NEXT;
    }

    case SDLK_KP_4:
    {
        if (isCapslock)
            return 0x34;
        return VK_LEFT;
    }
    case SDLK_KP_5:
    {
        if (isCapslock)
            return 0x35;
        return -1;
    }

    case SDLK_KP_6:
    {
        if (isCapslock)
            return 0x36;
        return VK_RIGHT;
    }
    case SDLK_KP_7:
    {
        if (isCapslock)
            return 0x37;
        return VK_HOME;
    }

    case SDLK_KP_8:
    {
        if (isCapslock)
            return 0x38;
        return VK_UP;
    }
    case SDLK_KP_9:
    {
        if (isCapslock)
            return 0x39;
        return VK_PRIOR;
    }

    case SDLK_KP_PERIOD:
    {
        if (isCapslock)
            return VK_DECIMAL;
        return VK_DELETE;
    }

    default:
        return -1;
    }
}

INPUT getEventSDL2AndConvertWin(SDL_Window* window, bool& isQuit, bool& isInput)
{
    SDL_Event event;
    INPUT input;
    memset(&input, 0, sizeof(input));
    while (SDL_PollEvent(&event) == 1)
    {
        double dx = 0, dy = 0;
        switch (event.type)
        {
        case SDL_QUIT:
        {
            isQuit = true;
            isInput = true;
            break;
        }

        case SDL_WINDOWEVENT:
        {
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                input.type = INPUT_WINDOW_CHANGED_SIZE;
                Point wnd = GetWindowResolutionSDL(window);
                memcpy((char*)&input + sizeof(input.type), &wnd, sizeof(wnd));
                isInput = true;
            }
            break;
        }

        case SDL_KEYDOWN:
        {
            input.type = INPUT_KEYBOARD;
            SDL_Keycode key = event.key.keysym.sym;
            input.ki.wVk = SDLtoWINKeyCode(key);
            isInput = true;
            break;
        }

        case SDL_KEYUP:
        {
            input.type = INPUT_KEYBOARD;
            SDL_Keycode key = event.key.keysym.sym;
            input.ki.wVk = SDLtoWINKeyCode(key);
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            isInput = true;
            break;
        }

        case SDL_MOUSEMOTION:
        {
            input.type = INPUT_MOUSE;
            input.mi.dx = event.motion.x;
            input.mi.dy = event.motion.y;
            isInput = true;
            input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            input.type = INPUT_MOUSE;

            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
            {
                input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                break;
            }

            case SDL_BUTTON_MIDDLE:
            {
                input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
                break;
            }

            case SDL_BUTTON_RIGHT:
            {
                input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                break;
            }
            }
            isInput = true;
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            input.type = INPUT_MOUSE;

            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
            {
                input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                break;
            }

            case SDL_BUTTON_MIDDLE:
            {
                input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
                break;
            }

            case SDL_BUTTON_RIGHT:
            {
                input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                break;
            }
            }
            isInput = true;
            break;
        }

        case SDL_MOUSEWHEEL:
        {
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_WHEEL;

            input.mi.mouseData = event.wheel.y * 60;
            isInput = true;

            break;
        }
        }
    }
    return input;
}