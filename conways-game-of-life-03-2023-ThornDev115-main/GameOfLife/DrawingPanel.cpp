#include "DrawingPanel.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

DrawingPanel::DrawingPanel(wxWindow* parent, std::vector<std::vector<bool>>& gameBoard, Settings* settings, std::vector<std::vector<int>>& drawingPanelNeighborInParameters):wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(200, 200)), gameBoardWindow(gameBoard), settingsPointer(settings), drawingPanelNeighborCount(drawingPanelNeighborInParameters)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this); 
	this->Bind(wxEVT_LEFT_UP, &DrawingPanel::OnMouseUp, this);
}

DrawingPanel::~DrawingPanel()
{

}

void DrawingPanel::OnPaint(wxPaintEvent& paintEvent)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	wxGraphicsContext* graphicContext;
	graphicContext = wxGraphicsContext::Create(dc);

	if (!wxGraphicsContext::Create(dc)) 
	{
		return;
	}

	graphicContext->SetPen(*wxBLACK);
	graphicContext->SetBrush(*wxWHITE);
	graphicContext->DrawRectangle(0, 0, 200, 200);

	float setHeight = ((float)GetSize().y / (float)settingsPointer->gridSize);
	float setWidth = ((float)GetSize().x / (float)settingsPointer->gridSize);

	for (int i = 0; i < settingsPointer->gridSize; i++)
	{
		for (int j = 0; j < settingsPointer->gridSize; j++)
		{
			if (gameBoardWindow[i][j] == false)
			{
				graphicContext->SetBrush(settingsPointer->GetDead());
			}
			else
			{
				graphicContext->SetBrush(settingsPointer->GetLiving());
			}

			graphicContext->DrawRectangle(i * setWidth, j * setHeight, setHeight * settingsPointer->gridSize, setWidth * settingsPointer->gridSize);
		}
	}

	if (settingsPointer->NeighborCount == true)
	{
		graphicContext->SetFont(wxFontInfo(16), *wxRED);

		wxString storeText;
		double x;
		double y;

		graphicContext->GetTextExtent(storeText, &x, &y);

		for (int i = 0; i < settingsPointer->gridSize; i++)
		{
			for (int j = 0; j < settingsPointer->gridSize; j++)
			{
				storeText = std::to_string(drawingPanelNeighborCount[i][j]);
				graphicContext->DrawText(storeText, (i * setWidth) + setWidth / 2, (j * setHeight) + setHeight / 2);
			}
		}			
	}
}

void DrawingPanel::SettingSize(wxSize& parameter)
{
	wxPanel::SetSize(parameter);
	Refresh();
}

void DrawingPanel::SetGrid(int newSize)
{
	settingsPointer->gridSize = newSize;
}

void DrawingPanel::OnMouseUp(wxMouseEvent& mouse)
{
	int x_Value = mouse.GetX();
	int y_Value = mouse.GetY();

	float setHeight = ((float)GetSize().y / (float)settingsPointer->gridSize);
	float setWidth = ((float)GetSize().x / (float)settingsPointer->gridSize);

	float newX = x_Value / setWidth;
	float newY = y_Value / setHeight;

	if (gameBoardWindow[newX][newY] == true)
	{
		gameBoardWindow[newX][newY] = false;
	}
	else
	{
		gameBoardWindow[newX][newY] = true;
	}
	Refresh();
}