//Include guard
#pragma once

#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

class Number_Game{

//public://for dev

	int g_secret_num;
	int gx;
	int g_guess;
	int delta;
	int g_prev_delta = 101;
	int g_more = 1;
	char m_choice;
	int m_quit = 1;

public://for release

	void Setup(){
		//cout << "Setup\n";
		srand(static_cast<unsigned int>(time(0)));
		//Get secret num  in [1..100]
		g_secret_num = 1 + (rand() % 99);
	}

	void Prompt(){
		cout << "Guess a number: ";
	}

	void Hello(){
		cout << "I picked a secret number from 1 to 100.\n";
		Prompt();
	}

	void Listen(){
		cin >> gx;
		g_guess = gx;

	}

	void Respond(){
		//Compare guess with secret number to tell user whether the guess was closer or farther away
		delta = abs(g_secret_num - g_guess);
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

	int Endcheck(){
		//Loop to make sure correct input is entered
		do{
			cout << "Would you like to play again?\n" <<
				"Yes(Y) or No(N)\n";
			cin >> m_choice;
			cout << endl;
			m_choice = toupper(m_choice);
		} while ('Y' != m_choice && 'N' != m_choice);

		if ('Y' == m_choice){
			m_quit = 1;
		}
		else if ('N' == m_choice){
			m_quit = 0;
		}
		return m_quit;
	}

	void Conversation(){
		//do{
		Setup();
		Hello();
		//Conv. Loop
		do{
			Listen();
			Respond();
		} while (g_more);
		//} while (Endcheck());
	}
};