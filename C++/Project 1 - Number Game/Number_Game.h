//include guard
#pragma once

#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

class Number_Game{

public:

	Number_Game();
	~Number_Game();

private:

	int g_secret_num;
	int g_guess;
	int g_prev_delta = 101;
	int g_more = 1;
	char suit;

	void Number_Game::Setup(){
		//cout << "Setup\n";
		srand(static_cast<unsigned int>(time(0)));
		//Get secret num  in [1..100]
		g_secret_num = 1 + (rand() % 99);
	}

	void Number_Game::Prompt(){
		cout << "Guess a number: ";
	}

	void Number_Game::Hello(){
		cout << "I picked a secret number from 1 to 100.\n";
		Prompt();
	}

	void Number_Game::Listen(){
		int gx;
		cin >> gx;
		g_guess = gx;

	}

	void Number_Game::Respond(){
		//Compare guess with secret number to tell user whether the guess was closer or farther away
		int delta = abs(g_secret_num - g_guess);
		if (delta < g_prev_delta){
			cout << "Getting warmer.\n";
		}
		if (delta >= g_prev_delta){
			cout << "Getting colder.\n";
		}
		if (0 == delta){
			cout << "Win!\n";
		}
		g_prev_delta = delta;
		g_more = delta;
		if (g_more)
			Prompt();
	}

	int Number_Game::Cleanup(){
		char choice;
		int ax;
		//Loop to make sure correct input is entered
		do{
			cout << "Would you like to play again?\n" <<
				"Yes(Y) or No(N)\n";
			cin >> choice;
			cout << endl;
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

	int Number_Game::Conversation(){
		//do{
		Setup();
		Hello();
		//Conv. Loop
		do{
			Listen();
			Respond();
		} while (g_more);
		//} while (Cleanup());
		return 0;
	}
};

