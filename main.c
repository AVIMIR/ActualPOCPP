#include <stdio.h>
#include <stdlib.h>


typedef struct{
	char sign;
	int x;
	int y;
} Cell;

const int SUCCESSFUL_EXECUTION = 0;

const int BOARD_WIDTH = 7, BOARD_HEIGHT = 6;

const int MAX_PLAYER_NAME_LENGTH = 30 + 1; // "+1" for null terminator
const int PLAYER_COUNT = 2;

const int WIN = 2, NO_WIN = 0;
const int DRAW = -1, NO_DRAW = 0;
const int CONTINUE = 0;
const int CONNECTED = 1, NOT_CONNECTED = 0;
const int IN_WINNING_SEQUENCE = 1, NOT_IN_WINNING_SEQUENCE = 0;

const int ILLEGAL_MOVE_FULL_COLUMN = -1;
char sequenceOnCheck[4];
Cell winningSequence[4];

int checkIfCellIsInWinningSequence(Cell cell) {
	for (int i = 0; i < 4; i++) {
		if (cell.x == winningSequence[i].x && cell.y == winningSequence[i].y)
			return IN_WINNING_SEQUENCE;
	}
	return NOT_IN_WINNING_SEQUENCE;
}

char calculateSignToDisplay(int x, int y, char onBoard, int displayWin) {
	char result = onBoard;
	int cellIsInWinningSequence;
	if (displayWin) {
		Cell cell;
		cell.x = x;
		cell.y = y;

		cellIsInWinningSequence = checkIfCellIsInWinningSequence(cell);
		if (cellIsInWinningSequence)
			result = 'Y';
	}
	return result;
}

int displayBoard(char board[BOARD_HEIGHT][BOARD_WIDTH], int displayWin) {
	char signToDisplay;
	printf("-------\n");
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			signToDisplay = calculateSignToDisplay(j, i, board[i][j], displayWin);
			printf("%c", signToDisplay);
		}
		printf("\n");
	}
	printf("-------\n");
	return SUCCESSFUL_EXECUTION;
}

char chooseSignBasedOnTurn(int turn) {
	switch (turn) {
		case 1:
			return 'X';
		case -1:
			return 'O';
	}
}

int checkIfCellIsEmpty(char cell) {
	return !(cell == 'X' || cell == 'O');
}

int findFirstEmptyCellFromBottomOfColumn(char board[BOARD_HEIGHT][BOARD_WIDTH], int column) {
	/* This function returns number of the first line from the bottom with empty space in a column
	 * If there is no such a line, then it returns -1
	 */
	int cellIsEmpty = 0;;
	for (int emptyCellLineNumber = BOARD_HEIGHT-1; emptyCellLineNumber >= 0 ; emptyCellLineNumber--) {
		cellIsEmpty = checkIfCellIsEmpty(board[emptyCellLineNumber][column]);
		if (cellIsEmpty)
			return emptyCellLineNumber;
	}
	return ILLEGAL_MOVE_FULL_COLUMN;
}

int putDotOnField(char board[BOARD_HEIGHT][BOARD_WIDTH], int line, int column, int turn) {
	char sign = chooseSignBasedOnTurn(turn);
	board[line][column] = sign;

	return SUCCESSFUL_EXECUTION;
}
int saveGame() {}
int getPlayerName(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], int playerNumeral) {
	// Gets name of only one player per call

	printf("Please enter player#%d name (max 30 symbols): \n", playerNumeral);
	scanf("%s", playersNames[playerNumeral]);

	return SUCCESSFUL_EXECUTION;
}
int getPlayersNames(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH]) {
	// Gets names of many players

	for (int i = 0; i < PLAYER_COUNT; i++) {
		getPlayerName(playersNames, i);
	}

	return SUCCESSFUL_EXECUTION;
}

int initializeBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// Fills the board with periods to avoid unsupported behavior

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			board[i][j] = '.';
		}
	}
}

int displayPlayersNames(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH]) {
	for (int i = 0; i < PLAYER_COUNT; i++) {
		printf("Player#%d: %s\n", i + 1, playersNames[i]);
	}

	return SUCCESSFUL_EXECUTION;
}

int saveResult() {
	printf("The game has been successfully saved!\n");


	return SUCCESSFUL_EXECUTION;
}
int getPlayerProperInput() {
	int input = -1;

	do {
		printf("Please enter a number of a column to put a dot there (or 0 to save the game): ");
		scanf("%d", &input);
		printf("\n");
	} while (input < 0 || input > 7);

	return input;
}


int initializeGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// Prepares the game to play

	getPlayersNames(playersNames);
	initializeBoard(board);
	displayPlayersNames(playersNames);
	// displayBoard(board);

	return SUCCESSFUL_EXECUTION;
}

int checkIfConnectedFour(char sign) {
	char cell;
	for (int i = 0; i < 4; i++) {
		cell = sequenceOnCheck[i];
		if (cell != sign) {
			return NOT_CONNECTED;
		}
	}
	return CONNECTED;
}

void formHorizontalSequence(char board[BOARD_HEIGHT][BOARD_WIDTH], char sign, Cell startingCellInLine) {
	for (int i = 0; i < 4; i++) {
		sequenceOnCheck[i] = board[startingCellInLine.y][startingCellInLine.x + i];

		winningSequence[i].x = startingCellInLine.x + i;
		winningSequence[i].y = startingCellInLine.y;
	}
}

void formVerticalSequence(char board[BOARD_HEIGHT][BOARD_WIDTH], char sign, Cell startingCell) {
	for (int i = 0; i < 4; i++) {
		sequenceOnCheck[i] = board[startingCell.y + i][startingCell.x];

		winningSequence[i].x = startingCell.x;
		winningSequence[i].y = startingCell.y + i;
	}
}
void formDiagonalTangentNegativeSequence(char board[BOARD_HEIGHT][BOARD_WIDTH], char sign, Cell startingCell) {
	for (int i = 0; i < 4; i++) {
		sequenceOnCheck[i] = board[startingCell.y + i][startingCell.x + i];

		winningSequence[i].x = startingCell.x + i;
		winningSequence[i].y = startingCell.y + i;
	}
}
void formDiagonalTangentPositiveSequence(char board[BOARD_HEIGHT][BOARD_WIDTH], char sign, Cell startingCell) {
	for (int i = 0; i < 4; i++) {
		sequenceOnCheck[i] = board[startingCell.y - i][startingCell.x + i];

		winningSequence[i].x = startingCell.x + i;
		winningSequence[i].y = startingCell.y - i;
	}
}


int checkWinCondition(char board[BOARD_HEIGHT][BOARD_WIDTH], int turn) {
	char sign = chooseSignBasedOnTurn(turn);
	Cell startingCell;

	int connectedFourHorizontally = 0;
	int connectedFourVertically = 0;
	int connectedFourDiagonallyTangentNegative = 0;
	int connectedFourDiagonallyTangentPositive = 0;

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		startingCell.y = i;
		for (int j = 0; j < BOARD_WIDTH; j++) {
			startingCell.x = j;
			if (j < 4) {
				formHorizontalSequence(board, sign, startingCell);
				connectedFourHorizontally = checkIfConnectedFour(sign);
				if (connectedFourHorizontally)
					return WIN;
			}

			if (i < 4) {
				formVerticalSequence(board, sign, startingCell);
				connectedFourVertically = checkIfConnectedFour(sign);
				if (connectedFourVertically)
					return WIN;
			}

			if (i < 4 && j < 4) {
				formDiagonalTangentNegativeSequence(board, sign, startingCell);
				connectedFourDiagonallyTangentNegative = checkIfConnectedFour(sign);
				if (connectedFourDiagonallyTangentNegative)
					return WIN;
			}

			if (i >= 3 && j < 4) {
				formDiagonalTangentPositiveSequence(board, sign, startingCell);
				connectedFourDiagonallyTangentPositive = checkIfConnectedFour(sign);
				if (connectedFourDiagonallyTangentPositive)
					return WIN;
			}
		}
	}
	return NO_WIN;
}
int checkDrawCondition(char board[BOARD_HEIGHT][BOARD_WIDTH]) {


	int cellIsEmpty = 0;
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			cellIsEmpty = checkIfCellIsEmpty(board[i][j]);
			if (cellIsEmpty)
				return NO_DRAW;
		}
	}
	return DRAW;
}

int checkGameEndConditions(char board[BOARD_HEIGHT][BOARD_WIDTH], int turn) {
	int win = checkWinCondition(board, turn);
	if (win)
		return WIN * turn;

	int draw = checkDrawCondition(board);
	if (draw)
		return DRAW;

	return CONTINUE;
}

int simulateGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// Game loop

	int turn = 1, playerInput = 0, gameEnd = 0, column = -1;
	int emptyCellLineNumber = -1;
	while (!gameEnd) {

		displayBoard(board, NO_WIN);

		playerInput = getPlayerProperInput();

		if (playerInput == 0) {
			saveGame(turn);
			continue;
		}

		column = playerInput-1;

		emptyCellLineNumber = findFirstEmptyCellFromBottomOfColumn(board, column);
		if (emptyCellLineNumber == ILLEGAL_MOVE_FULL_COLUMN) {
			printf("Illegal move: there is no free space in this column\n");
			continue;
		}

		putDotOnField(board, emptyCellLineNumber, column, turn);

		gameEnd = checkGameEndConditions(board, turn);
		if (gameEnd) {
			return gameEnd;
		}
		// printf("Game has not been finished yet\n");
		turn *= (-1);
	}
	return SUCCESSFUL_EXECUTION;
}


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

void playGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	initializeGame(playersNames, board);

	int gameResult = simulateGame(playersNames, board);
	if (gameResult == WIN) {
		displayBoard(board, WIN);
		printf("The game has ended. Player %s (#1) has won!", playersNames[0]);
	} else if (gameResult == -WIN) {
		displayBoard(board, WIN);
		printf("The game has ended. Player %s (#2) has won!", playersNames[1]);
	} else {
		displayBoard(board, NO_WIN);
		printf("The game has ended. NO WINNERS TODAY!", playersNames[1]);
	}

	saveResult();
}

void loadGame() {
	printf("We loaded a game. Now what?\n");
}

void exitGame() {
	printf("Thank you for playing! See you next time.\n");
}

int main(void) {
	char board[BOARD_HEIGHT][BOARD_WIDTH];
	char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH];

	displayMainMenu();
	int inputOption = getProperInput();

	switch (inputOption) {
		case 1:
			playGame(playersNames, board);
		break;
		case 2:
			loadGame();
		break;
		case 3:
			exitGame();
		exit(SUCCESSFUL_EXECUTION);
	}
	return SUCCESSFUL_EXECUTION;
}