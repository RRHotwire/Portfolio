//Include guard 
#pragma once

#include "Game_Master.h"
#include "Number_Game.h"
#include "Card_Game.h"
#include "Nim.h"

//Create Game_Master object and run conversation with user.
//Line count = 2
int main(){

	Game_Master Game;
	Game.Conversation();

	return 0;
}
