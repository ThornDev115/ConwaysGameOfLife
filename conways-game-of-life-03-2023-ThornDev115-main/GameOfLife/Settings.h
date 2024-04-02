#pragma once
#include "wx/wx.h"
#include <fstream>

struct Settings
{
	unsigned int livingRed = 128;
	unsigned int livingGreen = 128;
	unsigned int livingBlue = 128;
	unsigned int livingAlpha = 255;

	unsigned int deadRed = 255;
	unsigned int deadBlue = 255;
	unsigned int deadGreen = 255;
	unsigned int deadAlpha = 255;

	bool NeighborCount = false;

	int gridSize = 15;
	int interval = 50;

	std::string filePath = "";

	void SetLiving(wxColor color)
	{
		livingRed = color.GetRed();
		livingGreen = color.GetGreen();
		livingBlue = color.GetBlue();
		livingAlpha = color.GetAlpha();
	}

	wxColor GetLiving()
	{
		return wxColor(livingRed, livingGreen, livingBlue, livingAlpha);
	}

	void SetDead(wxColor color)
	{
		deadRed = color.GetRed();
		deadGreen = color.GetGreen();
		deadBlue = color.GetBlue();
		deadAlpha = color.GetAlpha();
	}

	wxColor GetDead()
	{
		return wxColor(deadRed, deadGreen, deadBlue, deadAlpha);
	}

	int GetGridSize()
	{
		return gridSize;
	}

	int GetTimeInterval()
	{
		return interval;
	}

	void LoadData()
	{
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(Settings));
		file.close();
	}

	void SaveData()
	{
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		file.write((char*)this, sizeof(Settings));
		file.close();
	}

	void DeleteFilePath()
	{
		filePath = "";
	}
};
