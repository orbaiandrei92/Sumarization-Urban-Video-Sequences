#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
[STAThread]

void main(array<System::String^>^ argv)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ProjectImgProc::MyForm form;
	Application::Run(%form);
}