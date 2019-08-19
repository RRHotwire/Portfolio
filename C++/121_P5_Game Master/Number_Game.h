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

class Number_Game{

	//public://for development

	//TOC of Locals
	int m_delta; //Difference between correct number and user's guess.
	int m_guess; //User's guess.
	int m_prev_delta = 1000; //Previous difference between correct number and user's guess. Default, large so first guess is always warmer.
	int m_quit = 1; //Whether the user wants to quit.
	int m_secret_num; //Secret number generated for user to guess.
	int m_win = 1; //Did the user win? Default, no.

public://for release

	//Generate secret number for user to guess.
	//Touches: m_secret_num
	//Line count = 1
	void Setup(){
		//DOUT << "(Setup\n";
		//srand(static_cast<unsigned int>(time(0))); //seeded at program start in Games_Master_H
		m_secret_num = 1 + (rand() % 99); //Get secret num in [1...100]
		//DOUT DZ(m_secret_num) ENDLDOUT;
		//DOUT << "  Setup)\n";
	}

	//Ask user to guess a number.
	//Line count = 1
	void Prompt(){
		//DOUT << "(Prompt\n";
		cout << "Guess a number: ";
		//DOUT << "  Prompt)\n";
	}

	//Tell user a number was generated.
	//Line count = 2
	void Hello(){
		//DOUT << "(Hello\n";
		cout << "Welcome to the secret number game.\n"
			<< "I picked a secret number from 1 to 100.\n\n";
		//DOUT << "  Hello)\n";
	}

	//Ask for user's guess.
	//Touches: m_guess
	//Line count = 1
	void Listen(){
		//DOUT << "(Listen\n";
		Prompt(); //Ask for user's guess
		cin >> m_guess;
		//DOUT DZ(m_guess) ENDLDOUT;
		//DOUT << "  Listen)\n";
	}

	//Compares guess with secret number to determine if guess was farther or closer.
	//Touches: m_delta, m_prev_delta, m_win
	//Line count = 7
	void Respond(){
		//DOUT << "(Respond\n";
		m_delta = abs(m_secret_num - m_guess); //Determine difference between user guess and secret number.
		//DOUT DZ(m_delta) ENDLDOUT;
		if (0 == m_delta){
			cout << "Congratulations, you have won!\n\n"; //Tell user they guessed the secret number.
			m_win = m_delta; //Determine if user has won. 0 is a win.
		}
		else if (m_delta < m_prev_delta){
			cout << "Getting warmer.\n\n"; //Tell user guess was closer than the previous guess.
		}
		else if (m_delta >= m_prev_delta){
			cout << "Getting colder.\n\n"; //Tell user guess was farther than the previous guess.
		}
		m_prev_delta = m_delta; //Make current guess the previous guess
		//DOUT DZ(m_win) ENDLDOUT;
		//DOUT << "  Respond)\n";
	}

	//Determines if user needs to guess again.
	//Line count = 1
	int Endcheck(){
		//DOUT << "(Endcheck\n";
		return m_win; //To check whether user has won.
		//DOUT << "  Endcheck)\n";
	}

	//void Cleanup(){}

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