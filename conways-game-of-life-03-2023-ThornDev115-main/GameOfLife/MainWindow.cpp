#include "MainWindow.h"
#include "wx/wx.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "Settings.h"
#include "Dialog.h"
#include "wx/numdlg.h"
#include "wx/filedlg.h"
#include <fstream>

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_SIZE(MainWindow::WindowResize)
EVT_MENU(10000, MainWindow::PlayEventHandler)
EVT_MENU(10001, MainWindow::PauseEventHandler)
EVT_MENU(10002, MainWindow::NextEventHandler)
EVT_MENU(10003, MainWindow::TrashEventHandler)
EVT_MENU(10005, MainWindow::ClickButton)
EVT_MENU(10006, MainWindow::ViewMenu)
EVT_MENU(10007, MainWindow::NormalRandomize)
EVT_MENU(10008, MainWindow::RandomizeWithSeed)
EVT_MENU(10009, MainWindow::New)
EVT_MENU(10010, MainWindow::Open)
EVT_MENU(10011, MainWindow::Save)
EVT_MENU(10012, MainWindow::SaveAs)
EVT_MENU(10013, MainWindow::Exit)
EVT_MENU(10014, MainWindow::Finite)
EVT_MENU(10015, MainWindow::Toroidal)
EVT_MENU(10016, MainWindow::ResetSettings)
EVT_MENU(10017, MainWindow::ImportGameBoard)
EVT_TIMER(10004, MainWindow::TimerEventHandler)
wxEND_EVENT_TABLE()


MainWindow::MainWindow():wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(200, 200))
{
	settingsReference.LoadData();

	statusBar = CreateStatusBar();
	SetStatusText();

	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);
	toolBar = CreateToolBar();
	toolBar->AddTool(10000, "Play", playIcon);
	toolBar->AddTool(10001, "Pause", pauseIcon);
	toolBar->AddTool(10002, "Next", nextIcon);
	toolBar->AddTool(10003, "Trash", trashIcon);
	timer = new wxTimer(this, 10004);
	toolBar->Realize();

	drawingPanelPtr = new DrawingPanel(this, gameBoard, &settingsReference, isNeighborCount);
	boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(drawingPanelPtr, 1, wxEXPAND | wxALL);

	this->SetSizer(boxSizer);
	this->Bind(wxEVT_SIZE, &MainWindow::WindowResize, this);

	menuBar = new wxMenuBar;
	SetMenuBar(menuBar);
	optionsMenu = new wxMenu();

	wxMenu* file = new wxMenu();
	file->Append(10009, "New");
	file->Append(10017, "Import");
	file->Append(10010, "Open");
	file->Append(10011, "Save");
	file->Append(10012, "Save As");
	file->Append(10013, "Exit");
	
	menuBar->Append(file, "File");

	wxMenu* menuPtr = new wxMenu();
	finite = new wxMenuItem(menuPtr, 10014, "Finite", wxEmptyString, wxITEM_CHECK);
	finite->SetCheckable(true);
	menuPtr->Append(finite);
	torodial = new wxMenuItem(menuPtr, 10015, "Torodial", wxEmptyString, wxITEM_CHECK);
	torodial->SetCheckable(false);
	menuPtr->Append(torodial);;
	menuPtr->Append(10016, "Reset Settings");
	
	

	menuBar->Append(menuPtr, "View");

	optionsMenu->Append(10005, "Customize");
	optionsMenu->Append(10006, "Neighbor Count", "Show Neighbor Count", true);
	menuBar->Append(optionsMenu, "Options");

	wxMenu* randoPtr = new wxMenu();
	randoPtr->Append(10007, "Randomize");
	randoPtr->Append(10008, "Randomize With Seed");
	menuBar->Append(randoPtr, "Randomize");

	InitializeGrid();
	this->Layout();
}

MainWindow::~MainWindow()
{
	settingsReference.SaveData();
}

void MainWindow::WindowResize(wxSizeEvent& size)
{
	wxSize wxsizer = wxWindow::GetSize();
	Refresh();
	if (drawingPanelPtr != nullptr)
	{
		drawingPanelPtr->SetSize(wxsizer);
	}
	size.Skip();
}

void MainWindow::SetStatusText()
{
	statusBar->SetStatusText("Generations: " + std::to_string(generations) + " \t Living Cells: " + std::to_string(numOfLivingCells));
}

void MainWindow::InitializeGrid()
{
	drawingPanelPtr->SetGrid(settingsReference.gridSize);
	gameBoard.resize(settingsReference.gridSize);
	isNeighborCount.resize(settingsReference.gridSize);

	for (int i = 0; i < settingsReference.gridSize; i++)
	{
		gameBoard[i].resize(settingsReference.gridSize);
		isNeighborCount[i].resize(settingsReference.gridSize);
	}
}

void MainWindow::PlayEventHandler(wxCommandEvent& event)
{
	timer->Start(milliseconds);
}
void MainWindow::PauseEventHandler(wxCommandEvent& event)
{
	timer->Stop();
}
void MainWindow::NextEventHandler(wxCommandEvent& event)
{
	MainWindow::RunAlgorithm();
}

void MainWindow::Trash()
{
	for (int i = 0; i < settingsReference.gridSize; i++)
	{
		for (int j = 0; j < settingsReference.gridSize; j++)
		{
			gameBoard[i][j] = false;
			numOfLivingCells = 0;
			generations = 0;
		}
	}
	SetStatusText();
	Refresh();
}
void MainWindow::TrashEventHandler(wxCommandEvent& event)
{
	Trash();
}

void MainWindow::TimerEventHandler(wxTimerEvent& event)
{
	MainWindow::RunAlgorithm();
}

int MainWindow::NeighborCount(int row, int collumn)
{
	int count = 0;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}
			else if (row + i < 0 || collumn + j < 0)
			{
				continue;
			}
			else if (row + i >= settingsReference.gridSize || collumn + j >= settingsReference.gridSize)
			{
				continue;
			}
			else if (gameBoard[row + i][collumn + j] == true)
			{
				count++;
			}
		}
	}
	neightborCount = count;

	return neightborCount;
}

void MainWindow::RunAlgorithm()
{
	std::vector<std::vector<bool>> sandbox;
	int newNeighborCount = 0;

	sandbox.resize(settingsReference.gridSize);

	for (int i = 0; i < settingsReference.gridSize; i++)
	{
		sandbox[i].resize(settingsReference.gridSize);
	}

	for (int i = 0; i < settingsReference.gridSize; i++)
	{
		for (int j = 0; j < settingsReference.gridSize; j++)
		{
			newNeighborCount = NeighborCount(i, j);

			if (gameBoard[i][j] == true && newNeighborCount < 2)
			{
				sandbox[i][j] = false;
			}
			else if (gameBoard[i][j] == true && newNeighborCount > 3)
			{
				sandbox[i][j] = false;
			}
			else if (gameBoard[i][j] == true && newNeighborCount == 2)
			{
				sandbox[i][j] = true;
			}
			else if (gameBoard[i][j] == true && newNeighborCount == 3)
			{
				sandbox[i][j] = true;
			}
			else if(gameBoard[i][j] == false && newNeighborCount == 3)
			{
				sandbox[i][j] = true;
			}
		}
	}
	generations++;
	neightborCount = 0;
	numOfLivingCells = 0;
	gameBoard.swap(sandbox);
	CalculateLivingCells();
	for (int i = 0; i < settingsReference.gridSize; i++)
	{
		for (int j = 0; j < settingsReference.gridSize; j++)
		{
			isNeighborCount[i][j] = NeighborCount(i, j);
		}
	}
	SetStatusText();
	Refresh();
}

void MainWindow::CalculateLivingCells()
{
	int calc = 0;

	for (int i = 0; i < settingsReference.gridSize; i++)
	{
		for (int j = 0; j < settingsReference.gridSize; j++)
		{
			if (gameBoard[i][j] == true)
			{
				calc++;
			}
		}
	}
	numOfLivingCells = calc;
}

void MainWindow::ClickButton(wxCommandEvent& event)
{
	Dialog* dialogSetting = new Dialog(this, &settingsReference);

	if (dialogSetting->ShowModal() == wxID_OK)
	{
		InitializeGrid();
		Refresh();
	}
}

void MainWindow::ViewMenu(wxCommandEvent& event)
{
	settingsReference.NeighborCount = event.IsChecked();
	Refresh();
}

void MainWindow::Randomize(int num)
{
	srand(num);
	for (int i = 0; i < settingsReference.gridSize; i++)
	{
		for (int j = 0; j < settingsReference.gridSize; j++)
		{
			int rando = rand();

			if (rando % 2 == 0)
			{
				gameBoard[i][j] = true;
			}
			else
			{
				gameBoard[i][j] = false;
			}
		}
	}
	Refresh();
}

void MainWindow::NormalRandomize(wxCommandEvent& event)
{
	Randomize(time(NULL));
}

void MainWindow::RandomizeWithSeed(wxCommandEvent& event)
{
	Randomize(wxGetNumberFromUser("Insert Seed", "Seed", "Randomize With Seed", 0, LONG_MIN, LONG_MAX));
}

void MainWindow::New(wxCommandEvent& event)
{
	Trash();

	settingsReference.DeleteFilePath();
	Refresh();
}

void MainWindow::Open(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, "Open Game File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	for (int i = 0; i < gameBoard.size(); i++)
	{
		gameBoard[i].clear();
		gameBoard[i].resize(0);
	}
	gameBoard.clear();
	gameBoard.resize(0);

	std::string buffer;
	std::ifstream fileStream;
	int index = 0;
	fileStream.open((std::string)fileDialog.GetPath());

	if (fileStream.is_open())
	{
		while (!fileStream.eof())
		{
			std::getline(fileStream, buffer);
			if (buffer.size() == 0)
			{
				break;
			}
			if (gameBoard.size() == 0)
			{
				gameBoard.resize(buffer.size());
			}
			gameBoard[index].resize(buffer.size());
			for (int i = 0; i < buffer.size(); i++)
			{
				if (buffer[i] == '*')
				{
					gameBoard[index][i] = true;
				}
				else
				{
					gameBoard[index][i] = false;
				}
			}
			index++;
		}

		fileStream.close();
	}

	Refresh();
}

void MainWindow::Save(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, "Save Game File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	std::ofstream fileStream;
	fileStream.open((std::string)fileDialog.GetPath());
	if (fileStream.is_open())
	{
		for (int i = 0; i < gameBoard.size(); i++)
		{
			for (int j = 0; j < gameBoard.size(); j++)
			{
				if (gameBoard[i][j])
				{
					fileStream << '*';
				}
				else
				{
					fileStream << ".";
				}
			}
			fileStream << '\n';
		}

		fileStream.close();
	}

	Refresh();
}

void MainWindow::SaveAs(wxCommandEvent& event)
{
	wxFileDialog fileDialog(this, "Save Game File", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	std::ofstream fileStream;
	fileStream.open((std::string)fileDialog.GetPath());
	if (fileStream.is_open())
	{
		for (int i = 0; i < gameBoard.size(); i++)
		{
			for (int j = 0; j < gameBoard.size(); j++)
			{
				if (gameBoard[i][j])
				{
					fileStream << '*';
				}
				else
				{
					fileStream << ".";
				}
			}
			fileStream << '\n';
		}

		fileStream.close();
	}

	Refresh();
}

void MainWindow::Exit(wxCommandEvent& event)
{
	Close();
}

void MainWindow::Finite(wxCommandEvent& event)
{
	MainWindow::InitializeGrid();
}

void MainWindow::Toroidal(wxCommandEvent& event)
{

}

void MainWindow::ResetSettings(wxCommandEvent& event)
{
	for (int i = 0; i < settingsReference.gridSize; i++)
	{
		for (int j = 0; j < settingsReference.gridSize; j++)
		{
			gameBoard[i][j] = false;
			settingsReference.gridSize = 15;
			settingsReference.livingRed = 128;
			settingsReference.livingGreen = 128;
			settingsReference.livingBlue = 128;
			settingsReference.livingAlpha = 255;
			settingsReference.deadRed = 255;
			settingsReference.deadGreen = 255;
			settingsReference.deadBlue = 255;
			settingsReference.deadAlpha = 255;
			numOfLivingCells = 0;
			generations = 0;
		}
	}
	SetStatusText();
	Refresh();
}

void MainWindow::ImportGameBoard(wxCommandEvent& event)
{
	wxFileDialog* mFileDialog = new wxFileDialog(this, "Open Game of Life", wxEmptyString, wxEmptyString, "Game of Life Files (*.cells) | *.cells", wxFD_OPEN);

	if (mFileDialog->ShowModal() == wxID_CANCEL)
	{
		return;
	}


	for (int i = 0; i < settingsReference.gridSize; i++)
	{
		gameBoard[i].clear();
	}
	gameBoard.clear();


	std::ifstream fileStream;
	std::string buffer;
	int index = 0;
	fileStream.open((std::string)mFileDialog->GetPath());

	if (fileStream.is_open())
	{
		while (!fileStream.eof())
		{
			std::getline(fileStream, buffer);
			if (buffer.size() == 0)
			{
				break;
			}

			if (gameBoard.size() == 0)
			{
				gameBoard.resize(settingsReference.gridSize);
			}
			gameBoard[index].resize(settingsReference.gridSize);

			for (int i = 0; i < buffer.size(); i++)
			{
				if (buffer[i] == '*')
				{
					gameBoard[index][i] = true;
				}
				else
				{
					gameBoard[index][i] = false;
				}
			}
			index++;
		}
		fileStream.close();
	}
	InitializeGrid();
	Refresh();
}