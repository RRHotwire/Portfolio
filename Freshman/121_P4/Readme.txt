CPSC 121

Project 4 - Nim
This project is to create a Nim game board. We will generate a random number of rows from 3 to 5, 
then we will generate a random number of pins in each row. The number of pins will be inserted into 
each element on the Nim array. The program will then randomly choose a player to go first, the user 
or computer. Each player will take turns choosing a row and number of pins to take away from the board. 
Once all the rows are empty the last player to have taken pins from a row will be declared the winner. 
Games master will then prompt if you want to play another game. If yes the program restarts, and if no 
then the program exits.


PROTOTYPE

Conversation(){
	Setup();
	Hello();
	do{
		Listen();
		Respond();
	} while (Endcheck());
	//Cleanup();
}
