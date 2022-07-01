#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Mine Sweeper - SIN.version!", wxPoint(30, 30), wxSize(800, 600))
{
	btn = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer *grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);
	mineField = new int[nFieldWidth * nFieldHeight];
	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	for (int x = 0; x < nFieldWidth; x++) 
	{
		for (int y = 0; y < nFieldHeight; y++) 
		{
			btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
			btn[y * nFieldWidth + x]->SetFont(font);
			grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);

			btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			mineField[nFieldWidth + x] = 0;
		}
	}

	this->SetSizer(grid);
	grid->Layout();
}


cMain::~cMain() 
{
	delete[] btn;
}

void cMain::OnButtonClicked(wxCommandEvent& evt) 
{
	int xPos = (evt.GetId() - 10000) % nFieldWidth;
	int yPos = (evt.GetId() - 10000) / nFieldWidth;

	if (bFirstClick)
	{
		int mines = 30;
		while (mines)
		{
			int randomXPos = rand() % nFieldWidth;
			int randomYPos = rand() % nFieldHeight;

			if (mineField[randomYPos * nFieldWidth + randomXPos] == 0 && randomXPos != xPos && randomYPos != yPos)
			{
				mineField[randomYPos * nFieldWidth + randomXPos] = -1;
				mines--;
			}
		}

		bFirstClick = false;
	}

	btn[yPos * nFieldWidth + xPos]->Enable(false);

	if (mineField[yPos * nFieldWidth + xPos] == -1)
	{
		wxMessageBox("You Hit a Bomb!!! - Game Over");

		bFirstClick = true;
		for (int x = 0; x < nFieldWidth; x++)
		{
			for (int y = 0; y < nFieldHeight; y++)
			{
				mineField[y * nFieldWidth + x] = 0;
				btn[y * nFieldWidth + x]->SetLabel("");
				btn[y * nFieldWidth + x]->Enable(true);
			}
		}
	}
	else
	{
		int mines_left = 0;
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (xPos + i >= 0 && xPos + i < nFieldWidth && yPos + j >= 0 && yPos + j < nFieldHeight)
				{
					if (mineField[(yPos + j) * nFieldWidth + (xPos + i)] == -1)
					{
						mines_left++;
					}
				}
			}
		}
		if (mines_left > 0)
		{
			btn[yPos * nFieldWidth + xPos]->SetLabel(std::to_string(mines_left));
		}
	}
	evt.Skip();
}


