#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include <vector>
class MainWindow:public wxFrame
{
public:
	Settings settingsReference;
	DrawingPanel* drawingPanelPtr;
	wxBoxSizer* boxSizer;
	wxStatusBar* statusBar;
	wxToolBar* toolBar;
	wxTimer* timer;
	wxMenuBar* menuBar;
	wxMenu* optionsMenu;
	wxMenuBar* randoBar;
	wxMenu* randoMenu;
	wxMenuItem* finite;
	wxMenuItem* torodial;
	std::vector<std::vector<bool>> gameBoard;
	std::vector<std::vector<int>> isNeighborCount;
	int generations = 0;
	int neightborCount = 0;
	int numOfLivingCells = 0;
	bool isLiving = true;
	int milliseconds = 50;

	MainWindow();
	~MainWindow();
	void WindowResize(wxSizeEvent& size);
	void InitializeGrid();
	void Trash();
	void SetStatusText();
	void PlayEventHandler(wxCommandEvent& event);
	void PauseEventHandler(wxCommandEvent& event);
	void NextEventHandler(wxCommandEvent& event);
	void TrashEventHandler(wxCommandEvent& event);
	void TimerEventHandler(wxTimerEvent& event);
	int NeighborCount(int row, int collumn);
	void RunAlgorithm();
	void CalculateLivingCells();
	void ClickButton(wxCommandEvent& event);
	void ViewMenu(wxCommandEvent& event);
	void Randomize(int num);
	void NormalRandomize(wxCommandEvent& event);
	void RandomizeWithSeed(wxCommandEvent& event);
	void New(wxCommandEvent& event);
	void Open(wxCommandEvent& event);
	void Save(wxCommandEvent& event);
	void SaveAs(wxCommandEvent& event);
	void Exit(wxCommandEvent& event);
	void Finite(wxCommandEvent& event);
	void Toroidal(wxCommandEvent& event);
	void ResetSettings(wxCommandEvent& event);
	void ImportGameBoard(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};

