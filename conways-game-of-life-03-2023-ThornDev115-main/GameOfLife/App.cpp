#include "App.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

bool App::OnInit() 
{
	mainPtr = new MainWindow();

	mainPtr->Show();

	return true;
}