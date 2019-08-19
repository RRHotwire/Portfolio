//Include guard
#pragma once

// CPP Debugging tools.
#define DOUT cout << " DEBUG: "
#define ENDLDOUT << endl
#define DZ(zelt) << #zelt " = " << (zelt) << " "

#include <iostream>
using namespace std;

const int KSIZE = 5; //Nim Game array size

class Nim{

//public://for development

	int m_anim_board[KSIZE]; //Nim game's array.
	int m_help = 0; 
	int m_rand_pins = 0; //Random number of pins for each Nim row.
	int m_rand_rows = 0; //Random number of rows for each Nim game.
	int m_remove_pin = 0; //Number of pins to remove from row.
	int m_remove_row = 0; //Number of row to remove pins from.
	int m_turn = (rand() % 1); //Chooses a random player to go first for each game.
	int m_win = 0; //User has won.
	int i = 0; //For loop
	int j = 0; //Nested for loop

public://for release

	//Checks for valid number of pins. (Must be before Check_Row_Pin() to compile)
	//Line Count: 2
	void Check_Pin(int &arraypins, int &removepins){ //m_anim_board[m_remove_row]
		//DOUT << "(Check_Pin\n";
		//DOUT DZ(arraypins) DZ(removepins) ENDLDOUT;
		if (1 > removepins){ //Remove 1 removepins if user chose less than 1 removepins.
			removepins = 1;
		}
		if (arraypins < removepins){ //Remove all pins if row selected has less pins than the user chose.
			removepins = arraypins;
		}
		//DOUT DZ(arraypins) DZ(removepins) ENDLDOUT;
		//DOUT << "  Check_Pin)\n";
	}

	//Check for valid, non-empty row, and number of pins.
	//Touches: m_help
	//Line Count: 2 
	void Check_Row_Pin(int &validrow, int &arraypins, int &removepins){
		//DOUT << "(Check_Row_Pin\n";
		//DOUT DZ(validrow) DZ(arraypins) ENDLDOUT;
		if (m_rand_rows <= validrow || validrow < 0){ //Check for valid row is selected.
			cout << "Invalid row = " << validrow << endl
				<< "Please choose a different row.\n\n";
			m_help = 1; //Show help for invalid input.
		}
		if (arraypins == 0){ //Check for empty row.
			cout << "Empty row = " << validrow << endl
				<< "Please choose a different row.\n\n";
			m_help = 1; //Show help for invalid input.
		}
		//DOUT DZ(validrow) DZ(arraypins) ENDLDOUT;
		//DOUT DZ(m_help) ENDLDOUT;
		//DOUT << "  Check_Row_Pin)\n";
		Check_Pin(arraypins, removepins); //Check for valid number of pins.
	}

	//int Cleanup(){}

	//Displays Nim board. (Must be before Listen() to compile)
	//Line Count: 2
	void Display(){
		//DOUT << "(Display\n";
		for (i = 0; i < m_rand_rows; i++) //Loop to display all rows
		{
			cout << i << ":";
			for (j = 0; j < m_anim_board[i]; j++) //Loop to display all pins
			{
				cout << " |";
			}
			cout << endl;
		}
		cout << endl;
		//DOUT << "  Display)\n";
	}

	//Checks for game over.
	int Endcheck(){
		//DOUT << "(Endcheck\n";
		//DOUT DZ(m_win) ENDLDOUT;
		return !m_win;
		//DOUT << "  Endcheck)\n";
	}

	//Check if each array slot is empty and displays winner if game is over. (Must be before Respond() to compile)
	//Touches: m_win
	//Line Count: 1
	void Game_Over(){
		if (m_anim_board[0] < 1 && m_anim_board[1] < 1 &&
			m_anim_board[2] < 1 && m_anim_board[3] < 1 &&
			m_anim_board[4] < 1){
			m_win = 1; //If all Nim array slots have 0 then game is over.
			cout << "Game Over, ";
			if (m_turn == 0){ //Display winners name.
				cout << "Player 1 Wins!\n\n";
			}
			else{
				cout << "Computer Wins!\n\n";
			}
		}
	}

	//Greets user
	void Hello(){
		//DOUT << "(Hello\n";
		cout << "Welcome to the Nim game\n\n";
		//DOUT << "  Hello)\n";
	}

	//Shows user valid row and pins inputs.
	//Touches: m_help
	//Line Count: 1
	void Help(){
		//DOUT << "(Help\n";
		cout << "Choose a non-empty row between: 0 - " << (m_rand_rows - 1) << endl
			<< "Choose a number of pins between: 1 - 8\n"
			<< "If pins taken is greater than available, you will take all.\n"
			<< "If pins taken is less than 1, you will take one.\n\n";
		m_help = 0; //Dont show help until another invalid input.
		//DOUT DZ(m_help) ENDLDOUT;
		//DOUT << "  Help)\n";
	}

	//Asks user for row and pins.
	//Touches: m_remove_row, m_remove_pin
	//Line Count: 3
	void Prompt(){
		//DOUT << "(Prompt\n";
		if (m_turn == 1){ //Player 1's turn.
			cout << "Player 1's turn:\n";
			cout << "Choose a row and number of pins: ";
			cin >> m_remove_row >> m_remove_pin; //User chooses a row and number of pins.
			cout << endl;
			//DOUT DZ(m_remove_row) DZ(m_remove_pin) ENDLDOUT;
		}
		else if (m_turn == 0){ //Computer's turn.
			cout << "Computer's turn:\n";
			m_remove_row = (rand() % m_rand_rows); //Generate random row to take pins from.
			m_remove_pin = 1 + (rand() % 8); //Generate random number of pins to take from row.
			//DOUT DZ(m_remove_row) DZ(m_remove_pin) ENDLDOUT;
		}
		//DOUT << "  Prompt)\n";
	}

	//Validates user's input and removes pins from row.
	//Touches: m_remove_row, m_anim_board[m_remove_row], m_remove_pin, m_turn
	//Line Count: 6
	void Respond(){
		//DOUT << "(Respond:\n";
		Check_Row_Pin(m_remove_row, m_anim_board[m_remove_row], m_remove_pin);
		//DOUT DZ(m_remove_row) DZ(m_remove_pin) ENDLDOUT DZ(m_help) ENDLDOUT DZ(m_anim_board[m_remove_row]) ENDLDOUT;
		if (!m_help){
			m_anim_board[m_remove_row] -= m_remove_pin; //Remove pins from selected row.
			//DOUT DZ(m_anim_board[m_remove_row]) ENDLDOUT;
			if (m_turn == 1){ //Display how many pins were taken from selected row.
				//DOUT DZ(m_remove_row) DZ(m_remove_pin) ENDLDOUT;
				cout << "Player 1 took " << m_remove_pin << " pins from row " << m_remove_row << endl << endl;
			}
			else if (m_turn == 0){ //Display how many pins were taken from selected row.
				cout << "Computer took " << m_remove_pin << " pins from row " << m_remove_row << endl << endl;
			}
			m_turn = !m_turn; //Switch turns.
			Game_Over(); //Checks if game is over
		}
		//DOUT DZ(m_turn) ENDLDOUT;
		//DOUT << "  Respond)\n";
	}

	//Generate and fill Nim board array.
	//Touches: m_rand_rows, m_anim_board[]
	//Line Count: 2
	void Setup(){
		//DOUT << "(Setup\n";
		//srand(static_cast<unsigned int>(time(0)));//seeded at program start in Games_Master_H
		m_rand_rows = 3 + (rand() % 3); //Generate random number of rows. [3...5]
		//DOUT DZ(m_rand_rows) ENDLDOUT;
		for (i = 0; i < m_rand_rows; i++) //Loop to fill Nim board array
		{
			m_anim_board[i] = 3 + (rand() % 6); //Generate random number of pins to fill each row of Nim array.
			//DOUT DZ(m_anim_board[i]) ENDLDOUT;
		}
		//DOUT << "  Setup)\n";
	}

	//Display game board and prompt user for input.
	//Touches: m_help
	//Line Count: 4
	void Listen(){
		//DOUT << "(Listen\n";
		Display(); //Display Board before each players turn.
		if (0 == m_turn){ //Computer doesnt need input help, turns off.
			m_help = 0;
		}
		if (m_help){  //Shows user valid inputs.
			Help();
		}
		Prompt(); //Ask for users input.		
		//DOUT << "  Listen)\n";
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