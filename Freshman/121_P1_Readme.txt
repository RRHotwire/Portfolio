CPSC 121

Project 1 - Card Guessing Game
This project is to design a card guessing game. We will use a random number generator to assign the secret card.
Once this card is assigned the user will be prompted to make a guess. After the user guesses the program will 
notify the user if their current guess was closer or farther away from the previous guess.

PROTOTYPE

void Setup()

void Prompt()

void Hello()

void Listen()

void Respond()

void Cleanup()

int main(){

do{
	setup();
	hello();
	//conv. loop
	do{
		listen();
		respond();
	} while (endcheck());
}while(cleanup());

}
