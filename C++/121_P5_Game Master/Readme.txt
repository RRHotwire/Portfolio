CPSC 121

Project 5 - Coding Standards
This project is to update our games master program to the proper standards. Code has been 
revised to fix the previous issues. Games included are Number Guessing, Card Guessing, and Nim.

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
