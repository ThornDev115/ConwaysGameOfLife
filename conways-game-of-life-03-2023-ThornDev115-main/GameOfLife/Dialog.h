#pragma once
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "Settings.h"

class Dialog:public wxDialog
{
public:
	wxBoxSizer* boxSizerVertical;
	wxBoxSizer* boxSizerHorizontalLiving;
	wxStaticText* staticTextLiving;
	wxColourPickerCtrl* colorCtrlLiving;

	wxBoxSizer* boxSizerHorizontalDead;
	wxStaticText* staticTextDead;
	wxColourPickerCtrl* colorCtrlDead;

	wxBoxSizer* boxSizerPtr;
	wxStaticText* staticTextPtr;
	wxSpinCtrl* spinCtrlPtr;

	wxBoxSizer* timerBox;
	wxTimer* timerPtr;
	wxStaticText* timerText;
	wxSpinCtrl* timeSpin;

	wxSizer* button;
	Settings* settings;

	Dialog(wxWindow* parent, Settings* Settings);
	void Living(wxColourPickerEvent& event);
	void Dead(wxColourPickerEvent& event);
	void GridSize(wxSpinEvent& event);
	void Timer(wxSpinEvent& event);
	wxDECLARE_EVENT_TABLE();
};

