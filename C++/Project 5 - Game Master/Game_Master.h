//Include guard
#pragma once

// CPP Debugging tools.
#define DOUT cout << " DEBUG: "
#define ENDLDOUT << endl
#define DZ(zelt) << #zelt " = " << (zelt) << " "

#include <iostream>
#include <string>
#include "Number_Game.h"
#include "Card_Game.h"
#include "Nim.h"
using namespace std;

class Game_Master{

//public://for development

	//TOC for Locals
	int m_choice; //User's game choice
	string m_game_list = "The games available are:\n1. Number Guessing Game\n2. Card Guessing Game\n3. Nim\n0. Quit\n"; //Game List
	char m_play_again; //User wants to play again
	int m_quit = 1; //Check if user is done playing. Default, 1 to play once.

public://for release

	//void Cleanup(){}

	//Check whether user wants to quit.
	int Endcheck(){
		//DOUT << "(Endcheck\n";
		//DOUT DZ(m_quit) ENDLDOUT;
		//DOUT << "  Endcheck)\n";
		return m_quit;
	}

	//Welcome message.
	void Hello(){
		//DOUT << "(Hello\n";
		cout << "Welcome to the Game Center.\n";
		//DOUT << " Hello)\n";
	}

	//Shows available games to play. (Must be before Listen() to compile)
	void Prompt(){
		//DOUT << "(Prompt\n";
		cout << m_game_list //Shows user available games.
			<< "\nPlease choose a game: ";
		//DOUT << "  Prompt)\n";
	}

	//Asks for user's game choice.
	//Touches: m_choice
	//Line Count: 1
	void Listen(){
		//DOUT << "(Listen\n";
		Prompt();
		cin >> m_choice; //Asks for user's game choice.
		//DOUT DZ(m_choice) ENDLDOUT;
		cout << endl;
		//DOUT << "  Listen)\n";
	}

	//Asks user if they want to play another game
	//Touches: m_play_again, m_win
	//Line Count: 2
	int Play_Again(){
		do{
			cout << "Would you like to play another game?\n"
				 << "	Yes(Y) or No(N)\n";
			cin >> m_play_again; //User chooses to play again.
			cout << endl;
			m_play_again = toupper(m_play_again); //Ensure user choice is uppercase to compare
		} while ('Y' != m_play_again && 'N' != m_play_again);
		if ('N' == m_play_again){ //Thanks user for playing and exits program.
			cout << "Thanks for playing.\n";
			m_quit = 0; //End program.
		}
		return m_quit;
	}

	//Select game based upon user's input.
	//Touches: m_quit
	//Line Count: 5
	void Respond(){
		//DOUT << "(Respond\n";
		switch (m_choice){ //Chooses game user wants to play.
		case 0:{ //Let user quit.
			m_quit = 0;
			cout << "Thanks for playing.\n";
			break;
		}
		case 1:{ //Run number guessing game.
			Number_Game NumG;
			NumG.Conversation();
			Play_Again(); //Asks if user wants to play another game.
			break;
		}
		case 2:{ //Run card guessing game.
			Card_Game CardG;
			CardG.Conversation();
			Play_Again(); //Asks if user wants to play another game.
			break;
		}
		case 3:{ //Run Nim game.
			Nim NimG;
			NimG.Conversation();
			Play_Again(); //Asks if user wants to play another game.
			break;
		}
		default:{ //Tell user their choice was invalid.
			cout << "Invalid choice.\n\n"; break;
		}
		}
		//DOUT DZ(m_quit) ENDLDOUT;
		//DOUT << "  Respond)\n";
	}

	//Seed random number generator at program start.
	void Setup(){
		//DOUT << "(Setup\n";
		srand(static_cast<unsigned int>(time(0))); //Seed RNG and call once per program run
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
