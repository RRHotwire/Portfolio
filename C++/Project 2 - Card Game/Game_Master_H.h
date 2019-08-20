//include guard
#pragma once

#include <iostream>
#include <string>
#include "Number_Game_H.h"
#include "Card_Game_H.h"

using namespace std;

class Game_Master{

private:

	int m_quit = 1;
	int m_choice;
	char m_repeat;
	bool valid = false;
	string m_game_list = "The games available are:\n1. Number Guessing Game\n2. Card Guessing Game\n0. Quit\n";

public:

	void Menu(){
		cout << "Welcome to the Game Center.\n";
	}

	void Listen(){
		//Get user's game input
		do{
			cout << m_game_list
				<< "\nPlease choose a game.\n";
			cin >> m_choice;
			if (m_choice >= 0 && m_choice <= 2)
			{
				valid = true;
			}
			else
			{
				cout << "Please enter a valid choice.\n\n";
			}
		} while (valid == false);
	}

	void Respond(){
		//Select game based upon user's input
		switch (m_choice)
		{
			case 0:
			{
				m_quit = 0;
				break;
			}
			case 1:
			{
				//Run number guessing game
				Number_Game NG;
				NG.Conversation();
				break;
			}
			case 2:
			{
				//Run card guessing game
				Card_Game CG;
				CG.Conversation();
				break;
			}
		}
	}

	int Endcheck(){
		//Loop to make sure correct input is entered
		if (m_quit != 0)
		{
			do{
				cout << "Would you like to play again?\n" <<
					"Yes(Y) or No(N)\n";
				cin >> m_repeat;
				m_repeat = toupper(m_repeat);
			} while ('Y' != m_repeat && 'N' != m_repeat);

			if ('Y' == m_repeat){
				m_quit = 1;
			}
			else if ('N' == m_repeat){
				m_quit = 0;
			}
		}
		return m_quit;
	}

	void Conversation(){
		Menu();
		do
		{
			//Conversation Loop
			Listen();
			Respond();
		} while (Endcheck());
	}
};
