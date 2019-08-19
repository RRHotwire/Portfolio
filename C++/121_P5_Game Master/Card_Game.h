//Include guard
#pragma once

// CPP Debugging tools.
#define DOUT cout << " DEBUG: "
#define ENDLDOUT << endl
#define DZ(zelt) << #zelt " = " << (zelt) << " "

#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;

class Card_Game{
//public: //For development

	//TOC of locals
	int m_convert_guess = 0; //Convert user's card to number value
	int m_delta; //Difference between user's guess and secret number.
	int m_guess; //User's card as number to compare with secret number.
	int m_gx_rank; //User's rank guess.
	char m_gx_suit; //User's suit guess.
	int m_help = 1; //Displays help to choose a card. Default, 1 so it shows instructions on the first guess.
	int m_prev_delta = 1000; //User's previous difference between guess and secret number. Default, large so first time always says warmer.
	int m_secret_num; //Card generated for user to guess.
	int m_win = 1; //Check if user has won

public: //For release

	//void Cleanup(){}

	//Convert card into number
	//Touches: m_convert_guess
	//Line Count = 5
	//Card ranking is by denomination then suit.
	//Denomination ranking:(2 < 3 < 4 < 5 < 6 < 7 < 8 < 9 < 1 0 < J < Q < K < A)
	//Suit Ranking: (Clubs < Diamonds < Hearts < Spades)
	int Convert_Card(char gsuit, int grank){
		//DOUT << "(Convert_Card:\n";
		switch (grank){ //Converts card by denomination from lowest to highest. (2-10,J,Q,K,A)
			case 2:{
				m_convert_guess = 1; break;
			}
			case 3:{
				m_convert_guess = 5; break;
			}
			case 4:{
				m_convert_guess = 9; break;
			}
			case 5:{
				m_convert_guess = 13; break;
			}
			case 6:{
				m_convert_guess = 17; break;
			}
			case 7:{
				m_convert_guess = 21; break;
			}
			case 8:{
				m_convert_guess = 25; break;
			}
			case 9:{
				m_convert_guess = 29; break;
			}
			case 10:{
				m_convert_guess = 33; break;
			}
			case 11:{
				m_convert_guess = 37; break;
			}
			case 12:{
				m_convert_guess = 41; break;
			}
			case 13:{
				m_convert_guess = 45; break;
			}
			case 14:{
				m_convert_guess = 49; break;
			}
		}
		//DOUT DZ(m_convert_guess) ENDLDOUT;
		switch (gsuit){ //Ranks denomination by suit from lowest to highest. (Clubs, Diamonds, Hearts, Spades)
			case 'C':{
				break;
			}
			case 'D':{
				m_convert_guess += 1; break;
			}
			case'H':{
				m_convert_guess += 2; break;
			}
			case 'S':{
				m_convert_guess += 3; break;
			}
		}
		//DOUT DZ(m_convert_guess) ENDLDOUT;
		//DOUT << "  Convert_Card)\n";
		return m_convert_guess; //Returns numerical value for card.
	}

	//Check for valid suit. (Must be before Check_Suit_Rank() to compile)
	//Touches: m_help, m_win
	//Line Count: 6
	void Check_Suit(char gsuit){
		//DOUT << "(Check_Suit\n";
		switch (gsuit){ //Checks for valid suit.
			case 'Q':{
				cout << "Thanks for playing.\n";
				m_win = 0; //User quits.
				//DOUT DZ(m_win) ENDLDOUT;
				break;
			}
			case 'C':{
				//DOUT << "Valid suit.\n";
				break;
			}
			case 'D':{
				//DOUT << "Valid suit.\n";
				break;
			}
			case 'H':{
				//DOUT << "Valid suit.\n";
				break;
			}
			case 'S':{
				//DOUT << "Valid suit.\n";
				break;
			}
			default:{ //Tells user if invalid suit
				cout << "Invalid suit = " << gsuit << "\n\n";
				m_help = 1; //Tells user valid card choices.
				//DOUT DZ(m_help) ENDLDOUT;
				break;
			}
		}
		//DOUT DZ(m_win) DZ(m_help) ENDLDOUT;
		//DOUT << "  Check_Suit)\n";
	}

	//Check for valid suit and rank.
	//Touches: m_help
	//Line Count: 2 
	void Check_Suit_Rank(char gsuit, int grank){
		//DOUT << "(Check_Suit_Rank\n";
		if (!(2 <= grank && 14 >= grank))
		{
			cout << "Invalid rank = " << grank << endl; //Displays invalid input.
			m_help = 1; //Shows user valid inputs.
			//DOUT DZ(m_help) ENDLDOUT;
		}
		Check_Suit(gsuit); //Check for correct suit.
		//DOUT DZ(m_help) ENDLDOUT;
		//DOUT << "  Check_Suit_Rank)\n";
	}

	//Check whether user has won.
	int Endcheck(){
		//DOUT << "(Endcheck\n";
		//DOUT DZ(m_win) ENDLDOUT;
		//DOUT << "  Endcheck)\n";
		return m_win;
	}

	//Shows user valid inputs.
	//Touches: m_help
	//Line Count: 1
	void Help(){
		//DOUT << "(Help\n";
		cout << "Suits: Clubs, Diamonds, Hearts, Spades (from lowest to highest)\n"
			<< "Ranks: 2-14 (2 - 10, Jack(11), Queen(12), King(13), Ace(14))\n"
			<< "Enter Q to quit.\n\n";
		m_help = 0; //Dont show help until another invalid input.
		//DOUT DZ(m_help) ENDLDOUT;
		//DOUT << "  Help)\n";
	}

	//Greet User
	void Hello(){
		//DOUT << "(Hello\n";
		cout << "Welcome to the secret card game.\n"
			<< "I picked a secret card from the deck.\n\n";
		//DOUT << "  Hello)\n";
	}

	//Ask user to guess a suit and rank. (Must be before Listen() to compile)
	//Line Count: 1
	void Prompt(){
		//DOUT << "(Prompt\n";
		if (m_help){
			Help(); //Shows user valid inputs.
		}
		cout << "Guess a suit and rank: ";
		//DOUT << "  Prompt)\n";
	}

	//Ask user for suit and rank choice
	//Touches: m_gx_suit, m_gx_rank
	//Line Count: 2
	void Listen(){
		//DOUT << "(Listen\n";
		Prompt(); //Asks user for input.
		cin >> m_gx_suit >> m_gx_rank; //User chooses a suit and rank.
		m_gx_suit = toupper(m_gx_suit); //Convert user suit choice to uppercase for comparison.
		//DOUT DZ(m_gx_suit) DZ(m_gx_rank) ENDLDOUT;
		//DOUT << "  Listen)\n";
	}

	//Compares user's guess with secret number to tell whether the guess was closer or farther away
	//Touches: m_gx_suit, m_gx_rank, m_guess, m_delta, m_prev_delta, m_win
	//Line Count: 8
	void Respond(){
		//DOUT << "(Respond\n";
		Check_Suit_Rank(m_gx_suit, m_gx_rank); //Validates user's suit and rank choice.
		//DOUT DZ(m_help) DZ(m_win) ENDLDOUT;
		if (!m_help){ //If user made a valid choice then compare to secret number.
			m_guess = Convert_Card(m_gx_suit, m_gx_rank); //Convert user's card to number.
			//DOUT DZ(m_guess) ENDLDOUT;
			m_delta = abs(m_secret_num - m_guess); //Get difference between secret number and user's guess.
			//DOUT DZ(m_delta) DZ(m_prev_delta) ENDLDOUT;
			if (0 == m_delta){ //User won.
				cout << "Congratulation, you have won!\n\n";
				m_win = m_delta; //Mark user has won.
			}
			else if (m_delta < m_prev_delta){ //User's guess was closer than previous.
				cout << "Getting warmer.\n\n";
			}
			else if (m_delta > m_prev_delta){ //User's guess was farther than previous.
				cout << "Getting colder.\n\n";
			}
			m_prev_delta = m_delta; //Makes previous guess the current one.
			//DOUT DZ(m_prev_delta) DZ(m_win) ENDLDOUT;
		}
		//DOUT << "  Respond)\n";
	}

	//Generate secret card
	//Touches: m_secret_num
	//Line Count: 1
	void Setup(){
		//DOUT << "(Setup\n";
		//srand(static_cast<unsigned int>(time(0))); //Seeded at program start in Games_Master_H.
		m_secret_num = 1 + (rand() % 52); //Generate secret card [1...52]
		//DOUT DZ(m_secret_num) ENDLDOUT;
		//DOUT << "  Setup)\n";
	}

	//Have a conversation with user
	//Line count = 7
	void Conversation(){
		Setup(); //Setup to run program
		Hello(); //Greet user
		do{ //Loop until done
			Listen(); //Listen to user
			Respond(); //Respond to user's input
		} while (Endcheck()); //Until user is done
		//Cleanup(); //Cleanup after user is done
	}
};