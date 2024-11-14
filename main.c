#include <stdio.h>
#include <stdlib.h>


const int SUCCESSFUL_EXECUTION = 0;

const int BOARD_WIDTH = 7;
const int BOARD_HEIGHT = 6;

void displayMainMenu() {
	// Self-explanatory
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

int putDotOnField(int column, int turn) {}
int saveGame() {}
int getPlayersName() {}
int getPlayersNames() {}
int initializeBoard() {}
int saveResult() {}
int getPlaysersInput() {}
int initializeGame(char *playersName1, char *playersName2) {
	system("clear");
	getPlayersNames();
	initializeBoard();

	return SUCCESSFUL_EXECUTION;
}
int handlePlayersInput(int playersInput, turn) {
	switch (playersInput) {
		case 0:
			saveGame(turn);
		break;
		default:
			putDotOnField(playersInput, turn);
	}
	return SUCCESSFUL_EXECUTION;
}

int displayBoard() {

	return SUCCESSFUL_EXECUTION;
}

int checkWinCondition() {}
int checkDrawCondition() {}
int checkGameEnd() {
	checkWinCondition();
	checkDrawCondition();

	return SUCCESSFUL_EXECUTION;
}

int simulateGame() {
	int turn = 1, playersInput;
	while (1) {
		playersInput = getPlaysersInput();
		handlePlayersInput(playersInput, turn);

		turn *= (-1);
	}
	return SUCCESSFUL_EXECUTION;
}

void playGame() {
	// printf("We played a game. You won!\n");
	char *playerName1;
	char *playerName2;
	initializeGame();


}

void loadGame() {
	printf("We loaded a game. Now what?\n");
}

void exitGame() {
	printf("Thank you for playing! See you next time.\n");
}

void handleChoice(int option) {
	//
	// Encapsulating switch block to make main function less crowded.

	switch (option) {
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
	int inputOption = getProperInput();
	// printf("You entered: %d\n", inputOption);
	handleChoice(inputOption);
	for (;;) {
		printf("Would you like to play again? (y/n) : ");
	}
	return SUCCESSFUL_EXECUTION;
}