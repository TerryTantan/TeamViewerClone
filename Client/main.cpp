#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main() {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    remote_desktop_client::MyForm^ mainForm = gcnew remote_desktop_client::MyForm();
    Application::Run(mainForm);

    return 0;
}
