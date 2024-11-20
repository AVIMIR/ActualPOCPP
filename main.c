#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SUCCESSFUL_EXECUTION 0

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6
#define LENGTH_OF_WINNING_SEQUENCE 4

#define MAX_PLAYER_NAME_LENGTH (30 + 1) // "+1" for null terminator
#define PLAYER_COUNT 2

#define WIN 2
#define NO_WIN 0

#define DRAW (-1)
#define NO_DRAW 0
#define CONTINUE 0

#define CONNECTED 1
#define NOT_CONNECTED 0

#define IN_WINNING_SEQUENCE 1
#define NOT_IN_WINNING_SEQUENCE 0

#define ILLEGAL_MOVE_FULL_COLUMN (-1)

#define SIGN_IN_WINNING_SEQUENCE 'Y'


typedef struct{
	char sign;
	int x;
	int y;
} Cell;


Cell makeCell(char sign, int x, int y) {
	Cell cell;
	cell.sign = sign;
	cell.x = x;
	cell.y = y;

	return cell;
}


Cell winningSequence[LENGTH_OF_WINNING_SEQUENCE];
short gameIdIsAcquired = 0;
unsigned long long gameID;
char gameIdString[50] = {0};


int checkIfCellIsInWinningSequence(Cell cell) {
	for (int i = 0; i < 4; i++) {
		if (cell.x == winningSequence[i].x && cell.y == winningSequence[i].y) {
			return IN_WINNING_SEQUENCE;
		}
	}
	return NOT_IN_WINNING_SEQUENCE;
}


void getGameId(void) {
	char pathToPreviousGameId[100] = "..\\storage\\previousGameID.txt";

	FILE *previousGameIdFile = fopen(pathToPreviousGameId, "r");
	fscanf(previousGameIdFile, "%llu", &gameID);
	fclose(previousGameIdFile);

	gameID++;
	gameIdIsAcquired = 1;
}

void updateGameId(void) {
	char pathToPreviousGameId[100] = "..\\storage\\previousGameID.txt";

	FILE *previousGameIdFile = fopen(pathToPreviousGameId, "w");
	fprintf(previousGameIdFile, "%llu", gameID);

	fclose(previousGameIdFile);
}

void convertGameIdNumberToString(void) {
	snprintf(gameIdString, 50, "%llu", gameID);
}


void getAndUpdateGameId(void) {
	getGameId();
	convertGameIdNumberToString();
	updateGameId();
}

char calculateSignToDisplay(Cell cellOnBoard, int win) {
	// This function decides what sign to show: which is on the board ('O' or 'X') or 'Y' which indicates that smb won
	int cellIsInWinningSequence = checkIfCellIsInWinningSequence(cellOnBoard);
	if (win && cellIsInWinningSequence) {
		return SIGN_IN_WINNING_SEQUENCE;
	}

	return cellOnBoard.sign;
}

int displayBoard(char board[BOARD_HEIGHT][BOARD_WIDTH], int displayWin) {
	char signToDisplay;
	char signOnBoard;
	Cell cell;
	printf("-------\n");
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			signOnBoard = board[i][j];
			cell = makeCell(signOnBoard, j, i);
			signToDisplay = calculateSignToDisplay(cell, displayWin);
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
		default:
			return '?';
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
		if (cellIsEmpty) {
			return emptyCellLineNumber;
		}
	}
	return ILLEGAL_MOVE_FULL_COLUMN;
}

void putDotOnField(char board[BOARD_HEIGHT][BOARD_WIDTH], int line, int column, int turn) {
	char sign = chooseSignBasedOnTurn(turn);
	board[line][column] = sign;

}

int countEmptyCells(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	int emptyCells = 0;
	int cellIsEmpty = 0;
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			cellIsEmpty = checkIfCellIsEmpty(board[i][j]);
			emptyCells += cellIsEmpty;
		}
	}

	return emptyCells;
}

void saveGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH], int turn) {
	char pathToGames[100] = "..\\storage\\games\\";

	if (!gameIdIsAcquired) {
		getAndUpdateGameId();
	}
	printf("%d", gameID);

	strcat(pathToGames, gameIdString);
	strcat(pathToGames, ".txt");

	printf("%s\n", pathToGames);

	int emptyCells = countEmptyCells(board);

	FILE *gameFile = fopen(pathToGames, "w");
	fprintf(gameFile, "%llu\n%s\n%s\n%d\n", gameID, playersNames[0], playersNames[1], turn);

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			fprintf(gameFile, "%c", board[i][j]);
		}
		fprintf(gameFile, "\n");
	}


	fclose(gameFile);
}

void getPlayerName(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], int playerNumeral) {
	// Gets name of only one player per call

	printf("Please enter player#%d name (max 30 symbols): \n", playerNumeral);
	scanf("%s", playersNames[playerNumeral]);

}
void getPlayersNames(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH]) {
	// Gets names of many players

	for (int i = 0; i < PLAYER_COUNT; i++) {
		getPlayerName(playersNames, i);
	}

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

void saveResult(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], int gameResult) {
	char pathToResults[100] = "..\\storage\\results";
	strcat(pathToResults, ".txt");

	FILE *resultFile = fopen(pathToResults, "a");
	fprintf(resultFile, "ID: %s, 'X' player name: %s, 'O' player name: %s, result: %d\n", gameIdString, playersNames[0], playersNames[1], gameResult);

	fclose(resultFile);
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

	return SUCCESSFUL_EXECUTION;
}

int checkIfFourCharactersInWinningSequenceAreSame(char sign) {
	// Checks if all the cells in 'winningSequence' array have the same character ('X' or 'Y')

	for (int i = 0; i < 4; i++) {
		if (winningSequence[i].sign != sign) {
			return NOT_CONNECTED;
		}
	}
	return CONNECTED;
}

void formHorizontalSequence(char board[BOARD_HEIGHT][BOARD_WIDTH], Cell startingCellInSequence) {
	// Fills 'winningSequence' array with four horizontally consecutive cells

	int y = startingCellInSequence.y;
	int x = startingCellInSequence.x;

	for (int i = 0; i < 4; i++) {
		winningSequence[i].sign = board[y][x + i];

		winningSequence[i].x = x + i;
		winningSequence[i].y = y;
	}
}

void formVerticalSequence(char board[BOARD_HEIGHT][BOARD_WIDTH], Cell startingCellInSequence) {
	// Fills 'winningSequence' array with four vertically consecutive cells

	int y = startingCellInSequence.y;
	int x = startingCellInSequence.x;

	for (int i = 0; i < 4; i++) {
		winningSequence[i].sign = board[y + i][x];

		winningSequence[i].x = x;
		winningSequence[i].y = y + i;
	}
}
void formDiagonalTangentNegativeSequence(char board[BOARD_HEIGHT][BOARD_WIDTH], Cell startingCellInSequence) {
	// Fills 'winningSequence' array with four diagonally (whose slope is negative) consecutive cells

	int y = startingCellInSequence.y;
	int x = startingCellInSequence.x;

	for (int i = 0; i < 4; i++) {
		winningSequence[i].sign = board[y + i][x + i];

		winningSequence[i].x = x + i;
		winningSequence[i].y = y + i;
	}
}
void formDiagonalTangentPositiveSequence(char board[BOARD_HEIGHT][BOARD_WIDTH], Cell startingCellInSequence) {
	// Fills 'winningSequence' array with four diagonally (whose slope is positive) consecutive cells

	int y = startingCellInSequence.y;
	int x = startingCellInSequence.x;

	for (int i = 0; i < 4; i++) {
		winningSequence[i].sign = board[y - i][x + i];

		winningSequence[i].x = x + i;
		winningSequence[i].y = y - i;
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
			startingCell.sign = board[i][j];
			if (j < 4) {
				formHorizontalSequence(board, startingCell);
				connectedFourHorizontally = checkIfFourCharactersInWinningSequenceAreSame(sign);
				if (connectedFourHorizontally) {
					return WIN;
				}
			}

			if (i < 3) {
				formVerticalSequence(board, startingCell);
				connectedFourVertically = checkIfFourCharactersInWinningSequenceAreSame(sign);
				if (connectedFourVertically) {
					return WIN;
				}
			}

			if (i < 3 && j < 4) {
				formDiagonalTangentNegativeSequence(board, startingCell);
				connectedFourDiagonallyTangentNegative = checkIfFourCharactersInWinningSequenceAreSame(sign);
				if (connectedFourDiagonallyTangentNegative) {
					return WIN;
				}
			}

			if (i >= 3 && j < 4) {
				formDiagonalTangentPositiveSequence(board, startingCell);
				connectedFourDiagonallyTangentPositive = checkIfFourCharactersInWinningSequenceAreSame(sign);
				if (connectedFourDiagonallyTangentPositive) {
					return WIN;
				}
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
			if (cellIsEmpty) {
				return NO_DRAW;
			}
		}
	}

	return DRAW;
}

int checkGameEndConditions(char board[BOARD_HEIGHT][BOARD_WIDTH], int turn) {
	int win = checkWinCondition(board, turn);
	if (win) {
		return WIN * turn;
	}

	int draw = checkDrawCondition(board);
	if (draw) {
		return DRAW;
	}

	return CONTINUE;
}

int simulateGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// Game main loop

	int turn = 1, playerInput = 0, gameEnd = 0, column = -1;
	int emptyCellLineNumber = -1;
	while (!gameEnd) {

		displayBoard(board, NO_WIN);

		playerInput = getPlayerProperInput();

		if (playerInput == 0) {
			saveGame(playersNames, board, turn);
			continue;
		}

		column = playerInput-1;
		emptyCellLineNumber = findFirstEmptyCellFromBottomOfColumn(board, column);
		if (emptyCellLineNumber == ILLEGAL_MOVE_FULL_COLUMN) {
			printf("Illegal move: there is no free space in this column");
			continue;
		}

		putDotOnField(board, emptyCellLineNumber, column, turn);

		gameEnd = checkGameEndConditions(board, turn);
		if (gameEnd) {
			return gameEnd;
		}

		turn *= (-1);
	}
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

void displayBoardBaseOnGameResult(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH], int gameResult) {
	switch (gameResult) {
		case WIN:
			displayBoard(board, WIN);
			printf("The game has ended. Player %s (#1) has won!\n", playersNames[0]);
			break;
		case (-WIN):
			displayBoard(board, WIN);
			printf("The game has ended. Player %s (#2) has won!\n", playersNames[1]);
			break;
		case (DRAW):
			displayBoard(board, NO_WIN);
			printf("The game has ended. NO WINNERS TODAY!\n");
			break;
	}
}



void playGame(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH], char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	initializeGame(playersNames, board);

	int gameResult = simulateGame(playersNames, board);
	displayBoardBaseOnGameResult(playersNames, board, gameResult);

	if (!gameIdIsAcquired) {
		getAndUpdateGameId();
	}
	saveResult(playersNames, gameResult);
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
	char placeHolder = 'a';

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
	}
	scanf("%c", &placeHolder);
	return SUCCESSFUL_EXECUTION;
}