#pragma once
#include "wx/wx.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include <vector>
#include "Settings.h"

class DrawingPanel:public wxPanel
{
private:
	std::vector<std::vector<bool>>& gameBoardWindow;
	Settings* settingsPointer;
	std::vector<std::vector<int>>& drawingPanelNeighborCount;
public:
	DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoard, Settings* settings, std::vector<std::vector<int>>& drawingPanelNeighborInParameters);
	~DrawingPanel();
	void OnPaint(wxPaintEvent& paintEvent);
	void SetGrid(int newSize);
	void SettingSize(wxSize& parameter);
	void OnMouseUp(wxMouseEvent& mouse);
};

