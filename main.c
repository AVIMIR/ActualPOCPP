#include <stdio.h>
#include <stdlib.h>


typedef struct{
	int x;
	int y;
} Cell;

const int SUCCESSFUL_EXECUTION = 0;

const int BOARD_WIDTH = 7;
const int BOARD_HEIGHT = 6;

const int MAX_PLAYER_NAME_LENGTH = 31;
const int PLAYER_COUNT = 2;

const int WIN = 1, NO_WIN = 0;
const int DRAW = 1, NO_DRAW = 0;

const int ILLEGAL_MOVE_FULL_COLUMN = -1;
Cell winningSequence[4];

int SimGdisplayBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	printf("-------\n");
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
	printf("-------\n");
	return SUCCESSFUL_EXECUTION;
}

char PDOFchooseSignBasedOnTurn(int turn) {
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

int SGfindFirstEmptyCellFromBottomOfColumn(char board[BOARD_HEIGHT][BOARD_WIDTH], int column) {
	/* This function returns number of the first line from the bottom with empty space in a column
	 * If there is no such a line, then it returns -1
	 */
	int cellIsEmpty = 0;;
	for (int i = BOARD_HEIGHT-1; i >= 0 ; i--) {
		cellIsEmpty = CICHECcheckIfCellIsEmpty(board[i][column]);
		if (cellIsEmpty)
			return i;
	}
	return -1;
}

int SimGputDotOnField(char board[BOARD_HEIGHT][BOARD_WIDTH], int line, int column, int turn) {
	char sign = PDOFchooseSignBasedOnTurn(turn);
	board[line][column] = sign;

	return SUCCESSFUL_EXECUTION;
}
int SimGsaveGame() {}
int PGgetPlayerName(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], int playerNumeral) {
	// Gets name of only one player per call

	printf("Please enter player#%d name (max 30 symbols): \n", playerNumeral);
	scanf("%s", playersNames[playerNumeral]);

	return SUCCESSFUL_EXECUTION;
}
int IGgetPlayersNames(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH]) {
	// Gets names of many players

	for (int i = 0; i < PLAYER_COUNT; i++) {
		PGgetPlayerName(playersNames, i);
	}

	return SUCCESSFUL_EXECUTION;
}

int IGinitializeBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// Fills the board with periods to avoid unsupported behavior

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

int PGsaveResult() {
	printf("The game has been successfully saved!\n");


	return SUCCESSFUL_EXECUTION;
}
int SimGgetPlayerProperInput() {
	int input = -1;

	do {
		printf("Please enter a number of a column to put a dot there (or 0 to save the game): ");
		scanf("%d", &input);
		printf("\n");
	} while (input < 0 || input > 7);

	return input;
}


int PGinitializeGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// Prepares the game to play

	IGgetPlayersNames(playersNames);
	IGinitializeBoard(board);
	IGdisplayPlayersNames(playersNames);
	// PGdisplayBoard(board);

	return SUCCESSFUL_EXECUTION;
}


int checkConnectedFourHorizontally(char board[BOARD_HEIGHT][BOARD_WIDTH], char sign, int startPointY, int startPointX) {

	char cellInLine = board[startPointY][startPointX];
	for (int i = 0; i < 3; i++) {
		cellInLine = board[startPointY][startPointX+i];
		if (cellInLine != sign)
			return NO_WIN;

		winningSequence[i].x = startPointX+i;
		winningSequence[i].y = startPointY;
	}

	return WIN;
}

int checkConnectedFourVertically(char board[BOARD_HEIGHT][BOARD_WIDTH], char sign) {}
int checkConnectedFourDiagonallyTangentNegative(char board[BOARD_HEIGHT][BOARD_WIDTH], char sign) {}
int checkConnectedFourDiagonallyTangentPositive(char board[BOARD_HEIGHT][BOARD_WIDTH], char sign) {}


int PGcheckWinCondition(char board[BOARD_HEIGHT][BOARD_WIDTH], int turn) {
	char sign = PDOFchooseSignBasedOnTurn(turn);




	return SUCCESSFUL_EXECUTION;
}
int PGcheckDrawCondition(char board[BOARD_HEIGHT][BOARD_WIDTH]) {


	int cellIsEmpty = 0;
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			cellIsEmpty = CICHECcheckIfCellIsEmpty(board[i][j]);
			if (!cellIsEmpty)
				return DRAW;
		}
	}
	return NO_DRAW;
}

int SGcheckGameEndConditions(char board[BOARD_HEIGHT][BOARD_WIDTH], int turn) {
	PGcheckWinCondition(board, turn);
	PGcheckDrawCondition(board);

	return SUCCESSFUL_EXECUTION;
}

int PGsimulateGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// Game loop

	int turn = 1, playerInput = 0, gameEnd = 0, column = -1;
	int emptyCellLineNumber = -1;
	while (!gameEnd) {

		SimGdisplayBoard(board);

		playerInput = SimGgetPlayerProperInput();

		if (playerInput == 0) {
			SimGsaveGame(turn);
			continue;
		}

		column = playerInput-1;

		emptyCellLineNumber = SGfindFirstEmptyCellFromBottomOfColumn(board, column);
		if (emptyCellLineNumber == ILLEGAL_MOVE_FULL_COLUMN) {
			printf("Illegal move: there is no free space in this column\n");
			continue;
		}

		SimGputDotOnField(board, emptyCellLineNumber, column, turn);

		gameEnd = SGcheckGameEndConditions(board, turn);
		if (gameEnd) {
			break;
		}
		// printf("Game has not been finished yet\n");
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