#define WIN32_LEAN_AND_MEAN
#pragma once
#include <string>
#include <iostream>
#include <msclr/marshal.h>
#include <vcclr.h>

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "kmtransfer.h"
#include "socketClient.h"
#include "transferClient.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"



namespace remote_desktop_client {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MyForm : public System::Windows::Forms::Form {
    public:
        MyForm(void);
        ~MyForm();
        bool isScreenSharing = false;
    protected:
        
        System::Windows::Forms::Label^ label1;
        System::Windows::Forms::TextBox^ textBox1;
        System::Windows::Forms::Button^ button1;

        System::Windows::Forms::Panel^ panel1;
        System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel1;
        array<System::Windows::Forms::Button^>^ buttons; // Array to store buttons
        System::Windows::Forms::ContextMenuStrip^ contextMenuStrip1;

    private:
        System::ComponentModel::Container^ components;
        void InitializeComponent(void);
        
    private:
        
        System::Void textBox1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
        System::Void button1_Click(System::Object^ sender, System::EventArgs^ e);
        Void ProcessDataAndCreateButton(String^ data);
        Boolean IsDataAlreadyExists(String^ data);
        void UpdateButtonPositions(int rowIndex, int colIndex);
        System::Void MyForm::ScreenSharingMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void MyForm::StopSharingMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void MyForm::DisconnectMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
    };
}
