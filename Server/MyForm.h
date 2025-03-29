#pragma once
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "socketServer.h"
#include "kmtransfer.h"
#include "transferServer.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

namespace remote_desktop_server {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MyForm : public System::Windows::Forms::Form {
    public:
        MyForm(void);
        ~MyForm();
       
    protected: System::Windows::Forms::Button^ CONNECT;
    protected: System::Windows::Forms::Button^ DISCONNECT;
    public: System::Windows::Forms::TextBox^ textBoxStatus;

    private:
        System::ComponentModel::Container^ components;

        void InitializeComponent(void);
        bool isServerRunning = false;

    private:
        System::Void CONNECT_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void DISCONNECT_Click(System::Object^ sender, System::EventArgs^ e);

    };
}
