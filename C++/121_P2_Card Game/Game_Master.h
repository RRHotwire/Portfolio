//include guard
#pragma once

#include <iostream>
#include <string>
#include "Number_Game.h"
#include "Card_Game.h"

using namespace std;

class Game_Master{

public:

	Game_Master();
	~Game_Master();

private:

	int gx;
	bool valid = false;
	string game_list = "The games available are:\n1. Number Guessing Game\n2. Card Guessing Game\n";

	void Game_Master::Hello(){
		cout << "Welcome to the Game Center.\n";
	}

	void Game_Master::Listen(){
		//Get user's game input
		do{
			cout << game_list
				<< "\nPlease choose a game.\n";
			cin >> gx;
			if (gx >= 1 && gx <= 2)
			{
				valid = true;
			}
			else
			{
				cout << "Please enter a valid choice.\n\n";
			}
		} while (valid == false);
	}

	void Game_Master::Respond(){
		//Select game based upon user's input
		switch (gx)
		{
		case 1:
			//Run number guessing game
			Number_Game::Number_Game();
			break;
		case 2:
			//Run card guessing game
			Card_Game::Card_Game();
			break;
		}
	}

	int Game_Master::Endcheck(){
		char choice;
		int ax;

		//Loop to make sure correct input is entered
		do{
			cout << "Would you like to play again?\n" <<
				"Yes(Y) or No(N)\n";
			cin >> choice;
			choice = toupper(choice);
		} while ('Y' != choice && 'N' != choice);

		if ('Y' == choice){
			ax = 1;
		}
		else if ('N' == choice){
			ax = 0;
		}
		return ax;
	}

	int Game_Master::Conversation(){
		Hello();
		do
		{
			//Conversation Loop
			Listen();
			Respond();
		} while (Endcheck());
		
		return 0;
	}
};
