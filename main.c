#include <stdio.h>
#include <stdlib.h>


const int SUCCESSFUL_EXECUTION = 0;

const int BOARD_WIDTH = 7;
const int BOARD_HEIGHT = 6;

const int MAX_PLAYER_NAME_LENGTH = 31;
const int PLAYER_COUNT = 2;


int SimGdisplayBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
	printf("-------\n");
	return SUCCESSFUL_EXECUTION;
}

char PDOFcalculateSign(int turn) {
	switch (turn) {
		case 1:
			return 'X';
		case -1:
			return 'O';
	}
}

int CICHECcheckIfCellIsEmpty(char cell) {
	return !(cell == 'X' || cell == 'O');
}

int PDOFcheckIfColumnHasEmptyCell(char board[BOARD_HEIGHT][BOARD_WIDTH], int column) {
	int cellIsEmpty = CICHECcheckIfCellIsEmpty(board[column]);
	for (int i = BOARD_HEIGHT-1; i >= 0 ; i--) {
		if (cellIsEmpty)
			return i;
	}
	return -1;
}

int SimGputDotOnField(char board[BOARD_HEIGHT][BOARD_WIDTH], int column, int turn) {
	char sign = PDOFcalculateSign(turn);
	int columnHasEmptyCell;
	while (1) {
		columnHasEmptyCell = PDOFcheckIfColumnHasEmptyCell(board, column);
		if (1)
			break;
	}



	return SUCCESSFUL_EXECUTION;
}
int SimGsaveGame() {}
int PGgetPlayerName(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], int playerNumeral) {
	printf("Please enter player#%d name (max 30 symbols): \n", playerNumeral);
	scanf("%s", playersNames[playerNumeral]);

	return SUCCESSFUL_EXECUTION;
}
int IGgetPlayersNames(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH]) {
	for (int i = 0; i < PLAYER_COUNT; i++) {
		PGgetPlayerName(playersNames, i);
	}

	return SUCCESSFUL_EXECUTION;
}

int IGinitializeBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			board[i][j] = '.';
		}
	}
}

int IGdisplayPlayersNames(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH]) {
	for (int i = 0; i < PLAYER_COUNT; i++) {
		printf("Player#%d: %s\n", i, playersNames[i]);
	}

	return SUCCESSFUL_EXECUTION;
}

int PGsaveResult() {}
int SimGgetPlaysersInput() {}
int PGinitializeGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	IGgetPlayersNames(playersNames);
	IGinitializeBoard(board);
	IGdisplayPlayersNames(playersNames);
	// PGdisplayBoard(board);




	return SUCCESSFUL_EXECUTION;
}

int PGcheckWinCondition() {}
int PGcheckDrawCondition() {}
int PGcheckGameEndConditions() {
	PGcheckWinCondition();
	PGcheckDrawCondition();

	return SUCCESSFUL_EXECUTION;
}

int PGsimulateGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	int turn = 1, playersInput, gameEnd;
	while (1) {
		SimGdisplayBoard(board);
		playersInput = SimGgetPlaysersInput();

		switch (playersInput) {
			case 0:
				SimGsaveGame(turn);
			break;
			default:
				SimGputDotOnField(board, playersInput, turn);
		}

		gameEnd = PGcheckGameEndConditions();
		if (!gameEnd) {
			break;
		}
		turn *= (-1);
	}
	return SUCCESSFUL_EXECUTION;
}


void MMdisplayMainMenu() {
	// Self-explanatory
	printf("Welcome to the Connect four!\n\n");
	printf("What would you like to do:\n");
	printf("1) Play a new game\n");
	printf("2) Load an already saved game\n");
	printf("3) Exit the game\n");
}

int MMgetProperInput() {
	// This function asks a user to enter a number from 1 to 3 and if the input is not valid, repeats the request.

	int input = -1;

	do {
		printf("Please enter a number from 1 to 3: ");
		scanf("%d", &input);
		printf("\n");
	} while (input < 1 || input > 3);

	return input;
}

void MMplayGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	PGinitializeGame(playersNames, board);
	PGsimulateGame(playersNames, board);


}

void MMloadGame() {
	printf("We loaded a game. Now what?\n");
}

void MMexitGame() {
	printf("Thank you for playing! See you next time.\n");
}

int main(void) {
	char board[BOARD_HEIGHT][BOARD_WIDTH];
	char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH];

	MMdisplayMainMenu();
	int inputOption = MMgetProperInput();

	switch (inputOption) {
		case 1:
			MMplayGame(playersNames, board);
		break;
		case 2:
			MMloadGame();
		break;
		case 3:
			MMexitGame();
		exit(SUCCESSFUL_EXECUTION);
	}
	return SUCCESSFUL_EXECUTION;
}