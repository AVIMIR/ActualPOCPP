#include <stdio.h>
#include <stdlib.h>


const int SUCCESSFUL_EXECUTION = 0;

const int BOARD_WIDTH = 7;
const int BOARD_HEIGHT = 6;

void displayMainMenu() {
	printf("Welcome to the Connect four!\n\n");
	printf("What would you like to do:\n");
	printf("1) Play a new game\n");
	printf("2) Load an already saved game\n");
	printf("3) Exit the game\n");
}

int getProperInput() {
	// This function asks a user to enter a number from 1 to 3 and if the input is not valid, repeats the request.

	int input = -1;

	do {
		printf("Please enter a number from 1 to 3: ");
		scanf("%d", &input);
		printf("\n");
	} while (input < 1 || input > 3);

	return input;
}

void playGame() {
	printf("We played a game. You won!\n");
}

void loadGame() {
	printf("We loaded a game. Now what?\n");
}

void exitGame() {
	printf("Thank you for playing! See you next time.\n");
}

void handleChoice(int input) {
	switch (input) {
		case 1:
			playGame();
			break;
		case 2:
			loadGame();
			break;
		case 3:
			exitGame();
			exit(SUCCESSFUL_EXECUTION);
	}
}

int main(void) {
	displayMainMenu();
	int input = getProperInput();
	printf("You entered: %d\n", input);
	handleChoice(input);
	for (;;) {
		printf("Would you like to play again? (y/n) : ");
	}
	return SUCCESSFUL_EXECUTION;
}