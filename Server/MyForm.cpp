#include "MyForm.h"
#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "transferServer.h"
#include "socketServer.h"

#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <iostream>

#define IPALL "0.0.0.0"

namespace remote_desktop_server {

    MyForm::MyForm(void) {
        InitializeComponent();
        isServerRunning = false;
    }

    MyForm::~MyForm() {
        if (components) {
            delete components;
        }
    }

    void MyForm::InitializeComponent(void) {
        this->CONNECT = (gcnew System::Windows::Forms::Button());
        this->DISCONNECT = (gcnew System::Windows::Forms::Button());
        this->textBoxStatus = (gcnew System::Windows::Forms::TextBox());
        this->SuspendLayout();
        // 
        // CONNECT
        // 
        this->CONNECT->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
            | System::Windows::Forms::AnchorStyles::Right));
        this->CONNECT->Font = (gcnew System::Drawing::Font(L"Open Sans Semibold", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->CONNECT->Location = System::Drawing::Point(127, 89);
        this->CONNECT->Name = L"CONNECT";
        this->CONNECT->Size = System::Drawing::Size(175, 70);
        this->CONNECT->TabIndex = 0;
        this->CONNECT->Text = L"CONNECT";
        this->CONNECT->UseVisualStyleBackColor = true;
        this->CONNECT->Click += gcnew System::EventHandler(this, &MyForm::CONNECT_Click);
        // 
        // DISCONNECT
        // 
        this->DISCONNECT->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
            | System::Windows::Forms::AnchorStyles::Right));
        this->DISCONNECT->Font = (gcnew System::Drawing::Font(L"Open Sans Semibold", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->DISCONNECT->Location = System::Drawing::Point(127, 176);
        this->DISCONNECT->Name = L"DISCONNECT";
        this->DISCONNECT->Size = System::Drawing::Size(175, 70);
        this->DISCONNECT->TabIndex = 1;
        this->DISCONNECT->Text = L"DISCONNECT";
        this->DISCONNECT->UseVisualStyleBackColor = true;
        this->DISCONNECT->Click += gcnew System::EventHandler(this, &MyForm::DISCONNECT_Click);
        // 
        // textBoxStatus
        // 
        this->textBoxStatus->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
            | System::Windows::Forms::AnchorStyles::Right));
        this->textBoxStatus->Font = (gcnew System::Drawing::Font(L"Open Sans", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
        this->textBoxStatus->ForeColor = System::Drawing::Color::Green;
        this->textBoxStatus->Location = System::Drawing::Point(127, 268);
        this->textBoxStatus->Name = L"textBoxStatus";
        this->textBoxStatus->ReadOnly = true;
        this->textBoxStatus->Size = System::Drawing::Size(175, 29);
        this->textBoxStatus->TabIndex = 2;
        // 
        // MyForm
        // 
        this->AllowDrop = true;
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
        this->AutoScroll = true;
        this->AutoSize = true;
        this->ClientSize = System::Drawing::Size(426, 411);
        this->Controls->Add(this->textBoxStatus);
        this->Controls->Add(this->DISCONNECT);
        this->Controls->Add(this->CONNECT);
        this->DoubleBuffered = true;
        this->Name = L"MyForm";
        this->Text = L"Remote Control - Server";
        this->ResumeLayout(false);
        this->PerformLayout();

    }

    System::Void MyForm::CONNECT_Click(System::Object^ sender, System::EventArgs^ e) {
        
       
        if (!isServerRunning) {
      

            isServerRunning = !isServerRunning;
            //socket
            ::SetProcessDPIAware();
            WSAInit();
            for (int i = 0; i < 4; i++)
                socketList.push_back(sockCreate(AF_INET, SOCK_STREAM, IPPROTO_TCP));
            sockBind(AF_INET, &socketList[0], (char*)IPALL, PORTSCREEN);
            sockBind(AF_INET, &socketList[1], (char*)IPALL, PORTKM);
            sockBind(AF_INET, &socketList[2], (char*)IPALL, PORTQUIT);
            sockBind(AF_INET, &socketList[3], (char*)IPALL, PORTSIGNAL);
            textBoxStatus->AppendText(" Ready to connect..."); 

            for (int i = 0; i < 4; i++)
                sockListen(&socketList[i]);

            std::thread acceptThread(acceptClient);
            acceptThread.detach();
            
        }
        else {
            MessageBox::Show("Server is connecting!");
        }
    }

    System::Void MyForm::DISCONNECT_Click(System::Object^ sender, System::EventArgs^ e) {
        if (isServerRunning) {
            isServerRunning = !isServerRunning;
            //cleanup
            closeSocketList();
            WSACleanup();
            MessageBox::Show("Disconnected successfully");
            textBoxStatus->Text = "";
            this->Close();
        }
        else {
            MessageBox::Show("Server is not connected!");
        }
    }
}


