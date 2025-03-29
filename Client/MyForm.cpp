#include "MyForm.h"
#define WIN32_LEAN_AND_MEAN

#include "socketClient.h"
#include "transferClient.h"

#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <iostream>

int nServer = 0;
extern bool isStopSharing;
extern bool isFinishedTransfer;
extern bool isErrorTransfer;

namespace remote_desktop_client {


	MyForm::MyForm(void) {
		InitializeComponent();
	}

	MyForm::~MyForm() {
		if (components) {
			delete components;
		}
	}

	void MyForm::InitializeComponent(void) {
		this->label1 = (gcnew System::Windows::Forms::Label());
		this->textBox1 = (gcnew System::Windows::Forms::TextBox());
		this->button1 = (gcnew System::Windows::Forms::Button());
		this->panel1 = (gcnew System::Windows::Forms::Panel());
		this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
		this->SuspendLayout();
		// 
		// label1
		// 
		this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Right));
		this->label1->AutoSize = true;
		this->label1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
		this->label1->Font = (gcnew System::Drawing::Font(L"Open Sans Semibold", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->label1->Location = System::Drawing::Point(776, 45);
		this->label1->Name = L"label1";
		this->label1->Size = System::Drawing::Size(115, 27);
		this->label1->TabIndex = 0;
		this->label1->Text = L"Enter IPv4:";
		// 
		// textBox1
		// 
		this->textBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Right));
		this->textBox1->Font = (gcnew System::Drawing::Font(L"Open Sans", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->textBox1->Location = System::Drawing::Point(781, 75);
		this->textBox1->Name = L"textBox1";
		this->textBox1->Size = System::Drawing::Size(238, 40);
		this->textBox1->TabIndex = 1;
		this->textBox1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::textBox1_KeyDown);
		// 
		// button1
		// 
		this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
		this->button1->BackColor = System::Drawing::Color::Black;
		this->button1->Font = (gcnew System::Drawing::Font(L"Open Sans Semibold", 8, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->button1->ForeColor = System::Drawing::Color::White;
		this->button1->Location = System::Drawing::Point(848, 130);
		this->button1->Name = L"button1";
		this->button1->Size = System::Drawing::Size(106, 47);
		this->button1->TabIndex = 2;
		this->button1->Text = L"Connect";
		this->button1->UseVisualStyleBackColor = false;
		this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
		// 
		// panel1
		// 
		this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Right));
		this->panel1->BackColor = System::Drawing::Color::Black;
		this->panel1->Location = System::Drawing::Point(742, 19);
		this->panel1->Name = L"panel1";
		this->panel1->Size = System::Drawing::Size(1, 506);
		this->panel1->TabIndex = 4;
		// 
		// tableLayoutPanel1
		// 
		this->tableLayoutPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
			| System::Windows::Forms::AnchorStyles::Left)
			| System::Windows::Forms::AnchorStyles::Right));
		this->tableLayoutPanel1->ColumnCount = 3;
		this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
			49.15254F)));
		this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
			50.84746F)));
		this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
			221)));
		this->tableLayoutPanel1->Location = System::Drawing::Point(38, 84);
		this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
		this->tableLayoutPanel1->RowCount = 2;
		this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50.12531F)));
		this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 49.87469F)));
		this->tableLayoutPanel1->Size = System::Drawing::Size(646, 374);
		this->tableLayoutPanel1->TabIndex = 5;
		// 
		// MyForm
		// 
		this->AllowDrop = true;
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
		this->AutoScroll = true;
		this->AutoSize = true;
		this->BackColor = System::Drawing::Color::White;
		this->ClientSize = System::Drawing::Size(1050, 543);
		this->Controls->Add(this->tableLayoutPanel1);
		this->Controls->Add(this->panel1);
		this->Controls->Add(this->button1);
		this->Controls->Add(this->textBox1);
		this->Controls->Add(this->label1);
		this->DoubleBuffered = true;
		this->ForeColor = System::Drawing::Color::Black;
		this->Name = L"MyForm";
		this->Text = L"Remote Desktop - Client";
		this->ResumeLayout(false);
		this->PerformLayout();

	}

	// MyForm.cpp
	System::Void MyForm::textBox1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		//MessageBox::Show("data");
		if (e->KeyCode == System::Windows::Forms::Keys::Enter)
		{
			// Người dùng nhấn Enter, lấy dữ liệu từ textBox1
			String^ data = textBox1->Text;

			// Kiểm tra xem dữ liệu đã tồn tại hay chưa
			if (!IsDataAlreadyExists(data))
			{
				//socket
				::SetProcessDPIAware();
				WSAInit();

				pin_ptr<const wchar_t> pinnedData = PtrToStringChars(data);
				const size_t length = data->Length + 1; // +1 for null terminator
				wchar_t* IP = new wchar_t[length];
				wcscpy_s(IP, length, pinnedData);

				if (clientConnect(nServer, IP) == 1) {
					MessageBox::Show("Successfully connect socket!");

					// Dữ liệu không trùng, tạo button
					ProcessDataAndCreateButton(data);
				}
				else
				{
					MessageBox::Show("Fail to connect socket!");
				}

				delete[] IP;
			}
			else
			{
				// Dữ liệu đã tồn tại, hiển thị thông báo
				MessageBox::Show("This IP address is already connected!");
			}

		}
	}

	System::Void MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		// Người dùng nhấn Enter, lấy dữ liệu từ textBox1
		String^ data = textBox1->Text;

		// Kiểm tra xem dữ liệu đã tồn tại hay chưa
		if (!IsDataAlreadyExists(data))
		{
			//socket
			::SetProcessDPIAware();
			WSAInit();

			pin_ptr<const wchar_t> pinnedData = PtrToStringChars(data);
			const size_t length = data->Length + 1; // +1 for null terminator
			wchar_t* IP = new wchar_t[length];
			wcscpy_s(IP, length, pinnedData);

			if (clientConnect(nServer, IP) == 1) {
				MessageBox::Show("Successfully connect socket!");

				// Dữ liệu không trùng, tạo button
				ProcessDataAndCreateButton(data);
			}
			else
			{
				MessageBox::Show("Fail to connect socket!");
			}

			delete[] IP;
		}
		else
		{
			// Dữ liệu đã tồn tại, hiển thị thông báo
			MessageBox::Show("This IP address is already connected!");
		}
	}

	System::Void MyForm::ScreenSharingMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{

		// Lấy ra MenuItem đã gọi sự kiện
		System::Windows::Forms::ToolStripMenuItem^ menuItem = dynamic_cast<System::Windows::Forms::ToolStripMenuItem^>(sender);

		// Kiểm tra nếu menuItem không phải là null
		if (menuItem != nullptr)
		{
			// Lấy ra ContextMenuStrip chứa menuItem
			System::Windows::Forms::ContextMenuStrip^ contextMenu = dynamic_cast<System::Windows::Forms::ContextMenuStrip^>(menuItem->Owner);


			// Convert the pinned pointer to a standard C++ wchar_t array

			// Kiểm tra nếu contextMenu không phải là null
			if (contextMenu != nullptr)
			{
				// Lấy ra Button mà ContextMenuStrip được gắn vào
				System::Windows::Forms::Button^ currentButton = dynamic_cast<System::Windows::Forms::Button^>(contextMenu->SourceControl);


				// Kiểm tra nếu buttonToRemove không phải là null
				if (currentButton != nullptr)
				{
					// Lấy ra FlowLayoutPanel chứa button và label
					System::Windows::Forms::FlowLayoutPanel^ parentPanel = dynamic_cast<System::Windows::Forms::FlowLayoutPanel^>(currentButton->Parent);

					// Kiểm tra nếu parentPanel không phải là null
					if (parentPanel != nullptr)
					{
						// Lấy ra thông tin về vị trí của parentPanel trong tableLayoutPanel1
						int rowIndex = tableLayoutPanel1->GetRow(parentPanel);
						int colIndex = tableLayoutPanel1->GetColumn(parentPanel);
						int n = rowIndex * 3 + colIndex;
					
						for each (Control ^ control in parentPanel->Controls)
						{
							System::Windows::Forms::Label^ label = dynamic_cast<System::Windows::Forms::Label^>(control);
							if (label != nullptr)
							{
								// Chuyển đổi text và màu của label
								label->Text = "Screen Sharing";
								label->ForeColor = System::Drawing::Color::LimeGreen; // Màu xanh lá
							}
						}
						if (sendSignal(socketList[n][3], TRANSFER) == true)
						{
							isScreenSharing = true;
							if (transfer(rowIndex, colIndex))
							{
								MessageBox::Show(L"Server might have been closed connection");
								int rowIndex = tableLayoutPanel1->GetRow(parentPanel);
								int colIndex = tableLayoutPanel1->GetColumn(parentPanel);
								int n = rowIndex * 3 + colIndex;
								// Xóa parentPanel (FlowLayoutPanel) từ tableLayoutPanel1
								tableLayoutPanel1->Controls->Remove(parentPanel);

								// Update the positions of existing buttons
								UpdateButtonPositions(rowIndex, colIndex);
								closeSocketList(n, nServer);
							}
						}
						else
						{
							MessageBox::Show(L"Server might have been closed connection");
							int rowIndex = tableLayoutPanel1->GetRow(parentPanel);
							int colIndex = tableLayoutPanel1->GetColumn(parentPanel);
							int n = rowIndex * 3 + colIndex;
							// Xóa parentPanel (FlowLayoutPanel) từ tableLayoutPanel1
							tableLayoutPanel1->Controls->Remove(parentPanel);

							// Update the positions of existing buttons
							UpdateButtonPositions(rowIndex, colIndex);
							closeSocketList(n, nServer);
						}
					}
				}
			}
		}
	}

	System::Void MyForm::StopSharingMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{

		// Lấy ra MenuItem đã gọi sự kiện
		System::Windows::Forms::ToolStripMenuItem^ menuItem = dynamic_cast<System::Windows::Forms::ToolStripMenuItem^>(sender);

		// Kiểm tra nếu menuItem không phải là null
		if (menuItem != nullptr)
		{
			// Lấy ra ContextMenuStrip chứa menuItem
			System::Windows::Forms::ContextMenuStrip^ contextMenu = dynamic_cast<System::Windows::Forms::ContextMenuStrip^>(menuItem->Owner);

			// Kiểm tra nếu contextMenu không phải là null
			if (contextMenu != nullptr)
			{
				// Lấy ra Button mà ContextMenuStrip được gắn vào
				System::Windows::Forms::Button^ disconnectButton = dynamic_cast<System::Windows::Forms::Button^>(contextMenu->SourceControl);

				// Kiểm tra nếu buttonToRemove không phải là null
				if (disconnectButton != nullptr)
				{
					// Lấy ra FlowLayoutPanel chứa button và label
					System::Windows::Forms::FlowLayoutPanel^ parentPanel = dynamic_cast<System::Windows::Forms::FlowLayoutPanel^>(disconnectButton->Parent);

					// Kiểm tra nếu parentPanel không phải là null
					if (parentPanel != nullptr)
					{
						isScreenSharing = false;

						// Lấy ra thông tin về vị trí của parentPanel trong tableLayoutPanel1
						int rowIndex = tableLayoutPanel1->GetRow(parentPanel);
						int colIndex = tableLayoutPanel1->GetColumn(parentPanel);
						int n = rowIndex * 3 + colIndex;
						//String^ strN = System::Convert::ToString(n);
						//MessageBox::Show(strN);

						for each (Control ^ control in parentPanel->Controls)
						{
							System::Windows::Forms::Label^ label = dynamic_cast<System::Windows::Forms::Label^>(control);
							if (label != nullptr)
							{
								// Chuyển đổi text và màu của label
								label->Text = "Connecting";
								label->ForeColor = System::Drawing::Color::Blue;
							}
						}
						//sendSignal(socketList[n][3], STOP);
						isStopSharing = true;
					}
				}
			}
		}
	}

	// Hàm xử lý sự kiện khi người dùng nhấn vào nút "Remove" trên ContextMenuStrip
	System::Void MyForm::DisconnectMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		// Lấy ra MenuItem đã gọi sự kiện
		System::Windows::Forms::ToolStripMenuItem^ menuItem = dynamic_cast<System::Windows::Forms::ToolStripMenuItem^>(sender);

		// Kiểm tra nếu menuItem không phải là null
		if (menuItem != nullptr)
		{
			// Lấy ra ContextMenuStrip chứa menuItem
			System::Windows::Forms::ContextMenuStrip^ contextMenu = dynamic_cast<System::Windows::Forms::ContextMenuStrip^>(menuItem->Owner);

			// Kiểm tra nếu contextMenu không phải là null
			if (contextMenu != nullptr)
			{
				// Lấy ra Button mà ContextMenuStrip được gắn vào
				System::Windows::Forms::Button^ buttonToRemove = dynamic_cast<System::Windows::Forms::Button^>(contextMenu->SourceControl);

				// Kiểm tra nếu buttonToRemove không phải là null
				if (buttonToRemove != nullptr)
				{

					// Lấy ra FlowLayoutPanel chứa button và label
					System::Windows::Forms::FlowLayoutPanel^ parentPanel = dynamic_cast<System::Windows::Forms::FlowLayoutPanel^>(buttonToRemove->Parent);

					// Kiểm tra nếu parentPanel không phải là null
					if (parentPanel != nullptr) {
						if (isScreenSharing == false)
						{
							// Lấy ra thông tin về vị trí của parentPanel trong tableLayoutPanel1
							int rowIndex = tableLayoutPanel1->GetRow(parentPanel);
							int colIndex = tableLayoutPanel1->GetColumn(parentPanel);
							int n = rowIndex * 3 + colIndex;
							//// Xóa parentPanel (FlowLayoutPanel) từ tableLayoutPanel1
							tableLayoutPanel1->Controls->Remove(parentPanel);

							// Update the positions of existing buttons
							UpdateButtonPositions(rowIndex, colIndex);
							////while (!isFinishedTransfer);
							sendSignal(socketList[n][3], STOP);
							closeSocketList(n, nServer);

							// Hiển thị thông báo "Success delete"
							MessageBox::Show("Successfully disconnect!", "Info");

							// Dừng việc duyệt các button khác sau khi đã xử lý xóa
							//return;
						}
						else {
							MessageBox::Show("Screen transmission must be interrupted before disconnection.");
						}
					}
				}
			}
		}
	}

	Boolean MyForm::IsDataAlreadyExists(String^ data)
	{

		// Duyệt qua tất cả các ô trong tableLayoutPanel1 để kiểm tra dữ liệu
		for (int row = 0; row < tableLayoutPanel1->RowCount; ++row)
		{
			for (int col = 0; col < tableLayoutPanel1->ColumnCount; ++col)
			{
				// Lấy ra Control tại vị trí (row, col)
				System::Windows::Forms::Control^ control = tableLayoutPanel1->GetControlFromPosition(col, row);

				// Kiểm tra nếu control không phải là null và là một FlowLayoutPanel
				if (control != nullptr && dynamic_cast<System::Windows::Forms::FlowLayoutPanel^>(control) != nullptr)
				{
					// Lấy ra Button từ FlowLayoutPanel
					System::Windows::Forms::Button^ button = dynamic_cast<System::Windows::Forms::Button^>(control->Controls[0]);

					// Kiểm tra nếu dữ liệu của Button trùng với dữ liệu từ textBox1
					if (button != nullptr && button->Text == data)
					{
						return true; // Dữ liệu đã tồn tại, không tạo button mới
					}
				}
			}
		}

		return false; // Dữ liệu không tồn tại, có thể tạo button mới
		return true;
	}

	Void MyForm::ProcessDataAndCreateButton(String^ data)
	{

		// Kiểm tra nếu textBox1 khác rỗng
		if (data != "")
		{
			// Kiểm tra nếu tableLayoutPanel1 đã đầy
			if (tableLayoutPanel1->Controls->Count == tableLayoutPanel1->ColumnCount * tableLayoutPanel1->RowCount)
			{
				// Hiện thông báo "Data Full"
				MessageBox::Show("Data Full", "Info");
			}
			else
			{
				// Tìm ô trống để tạo button và label
				for (int row = 0; row < tableLayoutPanel1->RowCount; ++row)
				{
					for (int col = 0; col < tableLayoutPanel1->ColumnCount; ++col)
					{
						// Check if the cell is empty
						if (tableLayoutPanel1->GetControlFromPosition(col, row) == nullptr)
						{
							// Create a new button
							System::Windows::Forms::Button^ newButton = gcnew System::Windows::Forms::Button();
							this->label1->Font = (gcnew System::Drawing::Font(L"Open Sans Semibold", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
								static_cast<System::Byte>(0)));
							newButton->Text = data;

							//String^ s = System::Convert::ToString(n);

							// Create a new label
							System::Windows::Forms::Label^ newLabel = gcnew System::Windows::Forms::Label();
							newLabel->Text = "Connecting";
							newLabel->ForeColor = System::Drawing::Color::Blue;

							// Create a new FlowLayoutPanel
							System::Windows::Forms::FlowLayoutPanel^ flowLayoutPanel = gcnew System::Windows::Forms::FlowLayoutPanel();
							flowLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
							flowLayoutPanel->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;

							// Add the button and label to the FlowLayoutPanel
							flowLayoutPanel->Controls->Add(newButton);
							flowLayoutPanel->Controls->Add(newLabel);

							// Set the size of the new button
							//newButton->Size = System::Drawing::Size(tableLayoutPanel1->Width / tableLayoutPanel1->ColumnCount / 1.4, tableLayoutPanel1->Height / tableLayoutPanel1->RowCount / 2);
							newButton->Size = System::Drawing::Size(200, 100);

							// Create a new ContextMenuStrip for the new button
							System::Windows::Forms::ContextMenuStrip^ contextMenu = gcnew System::Windows::Forms::ContextMenuStrip();
							System::Windows::Forms::ToolStripMenuItem^ removeMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem("Disconnect", nullptr, gcnew System::EventHandler(this, &MyForm::DisconnectMenuItem_Click));

							contextMenu->Items->Add(removeMenuItem);

							// Create a new ToolStripMenuItem for "Screen Sharing"
							System::Windows::Forms::ToolStripMenuItem^ screenSharingMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem("Screen Sharing", nullptr, gcnew System::EventHandler(this, &MyForm::ScreenSharingMenuItem_Click));
							contextMenu->Items->Add(screenSharingMenuItem);

							System::Windows::Forms::ToolStripMenuItem^ DisconnectMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem("Stop Sharing", nullptr, gcnew System::EventHandler(this, &MyForm::StopSharingMenuItem_Click));
							contextMenu->Items->Add(DisconnectMenuItem);

							// Thêm sự kiện DoubleClick cho button
							//newButton->DoubleClick += gcnew System::EventHandler(this, &MyForm::Button_DoubleClick);

							// Attach the ContextMenuStrip to the new button
							newButton->ContextMenuStrip = contextMenu;

							// Thêm sự kiện Click cho MenuItem "Remove"
							removeMenuItem->Click += gcnew System::EventHandler(this, &MyForm::DisconnectMenuItem_Click);
							removeMenuItem->Click -= gcnew System::EventHandler(this, &MyForm::DisconnectMenuItem_Click);
							screenSharingMenuItem->Click += gcnew System::EventHandler(this, &MyForm::ScreenSharingMenuItem_Click);
							screenSharingMenuItem->Click -= gcnew System::EventHandler(this, &MyForm::ScreenSharingMenuItem_Click);
							// Add the FlowLayoutPanel to the tableLayoutPanel1
							tableLayoutPanel1->Controls->Add(flowLayoutPanel, col, row);

							// Xóa dữ liệu từ textBox1 sau khi đã xử lý
							textBox1->Text = "";

							// Return to exit the function once the button is created
							return;
						}
					}
				}
			}
		}
	}

	Void MyForm::UpdateButtonPositions(int rowIndex, int colIndex)
	{
		if (colIndex < 2) colIndex++;
		else {
			colIndex = 0;
			rowIndex++;
		}
		// Loop through each row and column
		for (int row = rowIndex; row < tableLayoutPanel1->RowCount; ++row)
		{
			for (int col = colIndex; col < tableLayoutPanel1->ColumnCount; ++col)
			{
				// Lấy ra Control tại vị trí (row, col)
				System::Windows::Forms::Control^ control = tableLayoutPanel1->GetControlFromPosition(col, row);

				// Kiểm tra nếu control không phải là null
				if (control != nullptr)
				{
					if (col == 0) {
						if (row != 1) tableLayoutPanel1->SetColumn(control, 0);
						else {
							tableLayoutPanel1->SetRow(control, 0);
							tableLayoutPanel1->SetColumn(control, 2);
						}
					}
					else {
						tableLayoutPanel1->SetColumn(control, col - 1);
					}
				}
				else return;
			}
			// Set colIndex to 0 after the first column is processed
			colIndex = 0;
		}
	}
}
