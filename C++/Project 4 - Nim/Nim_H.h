//Include Guard 
#pragma once

#include <iostream>

using namespace std;

const int m_asize = 5;

class Nim
{
public://for Development

	int m_nim[m_asize];
	int m_rand_rows = 0, m_rand_pins = 0;
	int i = 0;
	int j = 0;
	//Chooses a random player to go first for each game
	int turn = (rand() % 1);
	int m_remove_row = 0, m_remove_pin = 0;
	bool row0empty = false,
		row1empty = false, 
		row2empty = false,
		row3empty = false,
		row4empty = false;
	
	char m_choice;
	int m_quit = 1;


//public://for Release

	void Setup(){
		///*DEBUG*/cout << "(Setup\n";
		srand(static_cast<unsigned int>(time(0)));
		//Generate random number of rows(3-5)
		m_rand_rows = 3 + (rand() % 3);
		///*DEBUG*/cout << "Number of rows: " << m_rand_rows << endl;
		//Fill array elements with random number of pins(3-8)
		for (i = 0; i < m_rand_rows; i++)
		{
			m_nim[i] = 3 + (rand() % 6);
			///*DEBUG*/cout << "Row " << i << ": Has " << m_nim[i] << " pins\n";
		}
		///*DEBUG*/cout << "  Setup)\n";
	}

	void Hello(){
		cout << "Welcome to Nim\n\n";
	}

	void Display(){
		for (i = 0; i < m_rand_rows; i++)
		{
			cout << i << ":";
			for (j = 0; j  < m_nim[i]; j ++)
			{
				cout << " |";
			}
			cout << endl;
		}
		cout << endl;
	}

	void PlayerTurn(){
		///*DEBUG*/cout << "(Playerturn\n";
		//Display Board before each players turn
		Display();

		if (turn == 1)
		{
			cout << "Player 1's turn:\n";
			cout << "Choose a row and number of pins to remove: ex(1 3): ";
			cin >> m_remove_row >> m_remove_pin;
			cout << endl;
		}
		else if (turn == 0)
		{
			cout << "Computer's turn:\n";
			m_remove_row = (rand() % m_rand_rows);
			m_remove_pin = 1 + (rand() % 8);
		}

		if (m_rand_rows <= m_remove_row || m_remove_row < 0)
		{
			cout << "Invalid row, please choose a different row.\n\n";
			PlayerTurn();
		}

		if (m_nim[m_remove_row] == 0)
		{
			cout << "That row is empty, please choose a different row.\n\n";
			PlayerTurn();
		}
		///*DEBUG*/cout << "  PlayerTurn)\n";
	}

	void Update(){
		///*DEBUG*/cout << "(Update:\n";
		//If user chose less than 1 pin then remove 1 pin
		if (1 > m_remove_pin)
		{
			m_remove_pin = 1;
		}
		//If row selected has less pins than user chose then remove all pins
		if (m_nim[m_remove_row] < m_remove_pin)
		{
			m_remove_pin = m_nim[m_remove_row];
		}
		m_nim[m_remove_row] -= m_remove_pin;
		if (turn == 1)
		{
			cout << "Player 1 took " << m_remove_pin << " pins from row " << m_remove_row << endl << endl;
		}
		else if (turn == 0)
		{
			cout << "Computer took " << m_remove_pin << " pins from row " << m_remove_row << endl << endl;
		}
		//Switch player turn
		turn = !turn;
		///*DEBUG*/cout << "  Update)\n";
	}

	bool ArrayIsEmpty(){
		if (m_nim[0] < 1 && 
			m_nim[1] < 1 && 
			m_nim[2] < 1 && 
			m_nim[3] < 1 && 
			m_nim[4] < 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int Gameover(){
		if (ArrayIsEmpty() == true)
		{
			cout << "Game Over\n";
			if (turn == 0)
			{
				cout << "Player 1 Wins!\n\n";
			}
			else
			{
				cout << "Computer Wins!\n\n";
			}
			return 0;
		}
		else
		{
			return 1;
		}
	}

/*	int Endcheck(){
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
*/

	void Conversation(){
		//do{
			Setup();
			Hello();
			//Conv. Loop
			do{
				PlayerTurn();
				Update();
			} while (Gameover());
		//} while (Endcheck());
	}
};
