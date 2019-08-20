//include guard
#pragma once

#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

class Card_Game{

private:

	int rank;
	int suit_num;
	char suit;
	int g_secret_num;
	int gx_rank;
	char gx_suit;
	int g_guess;
	int delta;
	int g_prev_delta = 101;
	int g_more = 1;
	int m_quit = 1;
	char m_repeat;

public:

	void Setup(){
		srand(static_cast<unsigned int>(time(0)));
		//Get rank num  in [1..13]
		rank = 1 + (rand() % 13);
		//Get suit num in [0..3]
		suit_num = (rand() % 4);

		//Convert suit num to char
		switch (suit_num){
		case 0:
			suit = 'C';
			break;
		case 1:
			suit = 'D';
			break;
		case 2:
			suit = 'H';
			break;
		case 3:
			suit = 'S';
			break;
		}

		//Set card ranking(number and suit) from Lowest (2)(Clubs) to Highest (Ace)(Spade) 
		if ('C' == suit)
		{
			if (1 == rank)
			{
				g_secret_num = 0;
			}
			else if (2 == rank)
			{
				g_secret_num = 4;
			}
			else if (3 == rank)
			{
				g_secret_num = 8;
			}
			else if (4 == rank)
			{
				g_secret_num = 12;
			}
			else if (5 == rank)
			{
				g_secret_num = 16;
			}
			else if (6 == rank)
			{
				g_secret_num = 20;
			}
			else if (7 == rank)
			{
				g_secret_num = 24;
			}
			else if (8 == rank)
			{
				g_secret_num = 28;
			}
			else if (9 == rank)
			{
				g_secret_num = 32;
			}
			else if (10 == rank)
			{
				g_secret_num = 36;
			}
			else if (11 == rank)
			{
				g_secret_num = 40;
			}
			else if (12 == rank)
			{
				g_secret_num = 44;
			}
			else if (13 == rank)
			{
				g_secret_num = 48;
			}
		}
		if ('D' == suit)
		{
			if (1 == rank)
			{
				g_secret_num = 1;
			}
			else if (2 == rank)
			{
				g_secret_num = 5;
			}
			else if (3 == rank)
			{
				g_secret_num = 9;
			}
			else if (4 == rank)
			{
				g_secret_num = 13;
			}
			else if (5 == rank)
			{
				g_secret_num = 17;
			}
			else if (6 == rank)
			{
				g_secret_num = 21;
			}
			else if (7 == rank)
			{
				g_secret_num = 25;
			}
			else if (8 == rank)
			{
				g_secret_num = 29;
			}
			else if (9 == rank)
			{
				g_secret_num = 33;
			}
			else if (10 == rank)
			{
				g_secret_num = 37;
			}
			else if (11 == rank)
			{
				g_secret_num = 41;
			}
			else if (12 == rank)
			{
				g_secret_num = 45;
			}
			else if (13 == rank)
			{
				g_secret_num = 49;
			}
		}
		if ('H' == suit)
		{
			if (1 == rank)
			{
				g_secret_num = 4;
			}
			else if (2 == rank)
			{
				g_secret_num = 8;
			}
			else if (3 == rank)
			{
				g_secret_num = 12;
			}
			else if (4 == rank)
			{
				g_secret_num = 16;
			}
			else if (5 == rank)
			{
				g_secret_num = 20;
			}
			else if (6 == rank)
			{
				g_secret_num = 24;
			}
			else if (7 == rank)
			{
				g_secret_num = 28;
			}
			else if (8 == rank)
			{
				g_secret_num = 30;
			}
			else if (9 == rank)
			{
				g_secret_num = 34;
			}
			else if (10 == rank)
			{
				g_secret_num = 38;
			}
			else if (11 == rank)
			{
				g_secret_num = 42;
			}
			else if (12 == rank)
			{
				g_secret_num = 46;
			}
			else if (13 == rank)
			{
				g_secret_num = 50;
			}
		}
		if ('S' == suit)
		{
			if (1 == rank)
			{
				g_secret_num = 3;
			}
			else if (2 == rank)
			{
				g_secret_num = 7;
			}
			else if (3 == rank)
			{
				g_secret_num = 11;
			}
			else if (4 == rank)
			{
				g_secret_num = 15;
			}
			else if (5 == rank)
			{
				g_secret_num = 19;
			}
			else if (6 == rank)
			{
				g_secret_num = 23;
			}
			else if (7 == rank)
			{
				g_secret_num = 27;
			}
			else if (8 == rank)
			{
				g_secret_num = 31;
			}
			else if (9 == rank)
			{
				g_secret_num = 35;
			}
			else if (10 == rank)
			{
				g_secret_num = 39;
			}
			else if (11 == rank)
			{
				g_secret_num = 43;
			}
			else if (12 == rank)
			{
				g_secret_num = 47;
			}
			else if (13 == rank)
			{
				g_secret_num = 51;
			}
		}

	}

	void Prompt(){
		cout << "Guess a rank(1-13) and suit(C,D,H,S): ";
	}

	void Hello(){
		cout << "I picked a secret card from the deck.\n";
		Prompt();
	}

	void Listen(){
		cin >> gx_rank >> gx_suit;
		gx_suit = toupper(gx_suit);
		//Convert rank and suit to a number value to compare with secret card
		if ('C' == gx_suit)
		{
			if (1 == gx_rank)
			{
				g_guess = 0;
			}
			else if (2 == gx_rank)
			{
				g_guess = 4;
			}
			else if (3 == gx_rank)
			{
				g_guess = 8;
			}
			else if (4 == gx_rank)
			{
				g_guess = 12;
			}
			else if (5 == gx_rank)
			{
				g_guess = 16;
			}
			else if (6 == gx_rank)
			{
				g_guess = 20;
			}
			else if (7 == gx_rank)
			{
				g_guess = 24;
			}
			else if (8 == gx_rank)
			{
				g_guess = 28;
			}
			else if (9 == gx_rank)
			{
				g_guess = 32;
			}
			else if (10 == gx_rank)
			{
				g_guess = 36;
			}
			else if (11 == gx_rank)
			{
				g_guess = 40;
			}
			else if (12 == gx_rank)
			{
				g_guess = 44;
			}
			else if (13 == gx_rank)
			{
				g_guess = 48;
			}
		}
		if ('D' == gx_suit)
		{
			if (1 == gx_rank)
			{
				g_guess = 1;
			}
			else if (2 == gx_rank)
			{
				g_guess = 5;
			}
			else if (3 == gx_rank)
			{
				g_guess = 9;
			}
			else if (4 == gx_rank)
			{
				g_guess = 13;
			}
			else if (5 == gx_rank)
			{
				g_guess = 17;
			}
			else if (6 == gx_rank)
			{
				g_guess = 21;
			}
			else if (7 == gx_rank)
			{
				g_guess = 25;
			}
			else if (8 == gx_rank)
			{
				g_guess = 29;
			}
			else if (9 == gx_rank)
			{
				g_guess = 33;
			}
			else if (10 == gx_rank)
			{
				g_guess = 37;
			}
			else if (11 == gx_rank)
			{
				g_guess = 41;
			}
			else if (12 == gx_rank)
			{
				g_guess = 45;
			}
			else if (13 == gx_rank)
			{
				g_guess = 49;
			}
		}
		if ('H' == gx_suit)
		{
			if (1 == gx_rank)
			{
				g_guess = 4;
			}
			else if (2 == gx_rank)
			{
				g_guess = 8;
			}
			else if (3 == gx_rank)
			{
				g_guess = 12;
			}
			else if (4 == gx_rank)
			{
				g_guess = 16;
			}
			else if (5 == gx_rank)
			{
				g_guess = 20;
			}
			else if (6 == gx_rank)
			{
				g_guess = 24;
			}
			else if (7 == gx_rank)
			{
				g_guess = 28;
			}
			else if (8 == gx_rank)
			{
				g_guess = 30;
			}
			else if (9 == gx_rank)
			{
				g_guess = 34;
			}
			else if (10 == gx_rank)
			{
				g_guess = 38;
			}
			else if (11 == gx_rank)
			{
				g_guess = 42;
			}
			else if (12 == gx_rank)
			{
				g_guess = 46;
			}
			else if (13 == gx_rank)
			{
				g_guess = 50;
			}
		}
		if ('S' == gx_suit)
		{
			if (1 == gx_rank)
			{
				g_guess = 3;
			}
			else if (2 == gx_rank)
			{
				g_guess = 7;
			}
			else if (3 == gx_rank)
			{
				g_guess = 11;
			}
			else if (4 == gx_rank)
			{
				g_guess = 15;
			}
			else if (5 == gx_rank)
			{
				g_guess = 19;
			}
			else if (6 == gx_rank)
			{
				g_guess = 23;
			}
			else if (7 == gx_rank)
			{
				g_guess = 27;
			}
			else if (8 == gx_rank)
			{
				g_guess = 31;
			}
			else if (9 == gx_rank)
			{
				g_guess = 35;
			}
			else if (10 == gx_rank)
			{
				g_guess = 39;
			}
			else if (11 == gx_rank)
			{
				g_guess = 43;
			}
			else if (12 == gx_rank)
			{
				g_guess = 47;
			}
			else if (13 == gx_rank)
			{
				g_guess = 51;
			}
		}

	}

	void Respond(){
		//Compare guess with secret number to tell user whether the guess was closer or farther away
		delta = abs(g_secret_num - g_guess);
		if (delta < g_prev_delta){
			cout << "Getting warmer.\n";
		}
		if (delta > g_prev_delta){
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
			cin >> m_repeat;
			cout << endl;
			m_repeat = toupper(m_repeat);
		} while ('Y' != m_repeat && 'N' != m_repeat);

		if ('Y' == m_repeat){
			m_quit = 1;
		}
		else if ('N' == m_repeat){
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