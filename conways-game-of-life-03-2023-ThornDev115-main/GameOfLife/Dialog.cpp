#include "Dialog.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "Settings.h"

wxBEGIN_EVENT_TABLE(Dialog, wxDialog)
EVT_COLOURPICKER_CHANGED(20002, Dialog::Living)
EVT_COLOURPICKER_CHANGED(20004, Dialog::Dead)
EVT_SPINCTRL(20006, Dialog::GridSize)
EVT_SPINCTRL(20008, Dialog::Timer)
wxEND_EVENT_TABLE()

Dialog::Dialog(wxWindow* parent, Settings* Settings) : wxDialog(parent, wxID_ANY, "Settings"), settings(Settings)
{
	//Living
	boxSizerVertical = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(boxSizerVertical);

	boxSizerHorizontalLiving = new wxBoxSizer(wxHORIZONTAL);

	staticTextLiving = new wxStaticText(this, 20001, "Change Living Color: ");
	staticTextLiving->SetMinSize(wxSize(100, GetMinSize().y));
	colorCtrlLiving = new wxColourPickerCtrl(this, 20002);

	boxSizerHorizontalLiving->Add(staticTextLiving);
	boxSizerHorizontalLiving->Add(colorCtrlLiving);
	boxSizerVertical->Add(boxSizerHorizontalLiving);

	//Dead
	boxSizerHorizontalDead = new wxBoxSizer(wxHORIZONTAL);

	staticTextDead = new wxStaticText(this, 20003, "Change Dead Color: ");
	staticTextDead->SetMinSize(wxSize(100, GetMinSize().y));
	colorCtrlDead = new wxColourPickerCtrl(this, 20004);

	boxSizerHorizontalDead->Add(staticTextDead);
	boxSizerHorizontalDead->Add(colorCtrlDead);
	boxSizerVertical->Add(boxSizerHorizontalDead);

	//Grid Size
	boxSizerPtr = new wxBoxSizer(wxHORIZONTAL);
	staticTextPtr = new wxStaticText(this, 20005, "Change Grid Size: ");
	staticTextPtr->SetMinSize(wxSize(100, GetMinSize().y));
	spinCtrlPtr = new wxSpinCtrl(this, 20006);

	boxSizerPtr->Add(staticTextPtr);
	boxSizerPtr->Add(spinCtrlPtr);
	boxSizerVertical->Add(boxSizerPtr);

	//Timer
	timerBox = new wxBoxSizer(wxHORIZONTAL);
	timerText = new wxStaticText(this, 20007, "Change Timer: ");
	timerText->SetMinSize(wxSize(100, GetMinSize().y));
	timeSpin = new wxSpinCtrl(this, 20008);

	timerBox->Add(timerText);
	timerBox->Add(timeSpin);
	boxSizerVertical->Add(timerBox);

	//Button
	button = CreateButtonSizer(wxOK | wxCANCEL);
	boxSizerVertical->Add(button);

	colorCtrlLiving->SetColour(settings->GetLiving());
	colorCtrlDead->SetColour(settings->GetDead());
	spinCtrlPtr->SetValue(settings->GetGridSize());
	timeSpin->SetValue(settings->GetTimeInterval());
	Fit();
}

void Dialog::Living(wxColourPickerEvent& event)
{
	settings->SetLiving(event.GetColour());
}

void Dialog::Dead(wxColourPickerEvent& event)
{
	settings->SetDead(event.GetColour());
}

void Dialog::GridSize(wxSpinEvent& event)
{
	settings->gridSize = event.GetInt();
}

void Dialog::Timer(wxSpinEvent& event)
{
	settings->interval = event.GetExtraLong();
}
