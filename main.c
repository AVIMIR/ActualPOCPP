#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SEPARATOR_LINE "-------------------------------------------------------------\n"

#define BUFFER_SIZE 256
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
char gameIdString[BUFFER_SIZE] = {0};
char board[BOARD_HEIGHT][BOARD_WIDTH];
char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH];

int checkIfCellIsInWinningSequence(const Cell cell) {
	for (int i = 0; i < 4; i++) {
		if (cell.x == winningSequence[i].x && cell.y == winningSequence[i].y) {
			return IN_WINNING_SEQUENCE;
		}
	}
	return NOT_IN_WINNING_SEQUENCE;
}

void getGameId(void) {
	char pathToPreviousGameId[BUFFER_SIZE] = "..\\storage\\previousGameID.txt";

	FILE *previousGameIdFile = fopen(pathToPreviousGameId, "r");
	fscanf(previousGameIdFile, "%llu", &gameID);
	fclose(previousGameIdFile);

	gameID++;
	gameIdIsAcquired = 1;
}

void updateGameId(void) {
	char pathToPreviousGameId[BUFFER_SIZE] = "..\\storage\\previousGameID.txt";

	FILE *previousGameIdFile = fopen(pathToPreviousGameId, "w");
	fprintf(previousGameIdFile, "%llu", gameID);

	fclose(previousGameIdFile);
}

void convertGameIdNumberToString(void) {
	snprintf(gameIdString, BUFFER_SIZE, "%llu", gameID);
}

void getAndUpdateGameId(void) {
	getGameId();
	convertGameIdNumberToString();
	updateGameId();
}

char chooseSignToDisplay(const Cell cellOnBoard, int win) {
	// This function decides what sign to show: which is on the board ('O' or 'X') or 'Y' which indicates that smb won

	int cellIsInWinningSequence = checkIfCellIsInWinningSequence(cellOnBoard);
	if (win && cellIsInWinningSequence) {
		return SIGN_IN_WINNING_SEQUENCE;
	}

	return cellOnBoard.sign;
}

int displayBoard(const char anyBoard[BOARD_HEIGHT][BOARD_WIDTH], int displayWin, int separate) {
	if (separate) {
		printf(SEPARATOR_LINE);
	}

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			char signOnBoard = anyBoard[i][j];
			Cell cell = makeCell(signOnBoard, j, i);
			char signToDisplay = chooseSignToDisplay(cell, displayWin);
			printf("%c", signToDisplay);
		}
		printf("\n");
	}
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

int checkIfCellIsEmpty(const char cell) {
	return !(cell == 'X' || cell == 'O');
}

int findFirstEmptyCellFromBottomOfColumn(int column) {
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

void putDotOnField(int line, int column, int turn) {
	char sign = chooseSignBasedOnTurn(turn);
	board[line][column] = sign;

}

int countEmptyCells() {
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

void saveGame(int turn) {
	// Saves players' names, a current turn and current board to a <gameID>.txt file
	// and
	// Appends gameID to gamesIDs.txt file

	char pathToGames[BUFFER_SIZE] = "..\\storage\\games\\";

	if (!gameIdIsAcquired) {
		getAndUpdateGameId();
	}

	strcat(pathToGames, gameIdString);
	strcat(pathToGames, ".txt");

	printf("Game has been successfully saved in %s\n", pathToGames);

	int emptyCells = countEmptyCells(board);

	FILE *gameFile = fopen(pathToGames, "w");
	fprintf(gameFile, "%llu\n%s\n%s\n%d\n%d\n", gameID, playersNames[0], playersNames[1], emptyCells, turn);

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			fprintf(gameFile, "%c", board[i][j]);
		}
		fprintf(gameFile, "\n");
	}
	fclose(gameFile);

	char pathToGamesIds[BUFFER_SIZE] = "..\\storage\\gamesIDs.txt";
	FILE *gamesIdsFile = fopen(pathToGamesIds, "a");

	fprintf(gamesIdsFile, "%llu\n", gameID);
	fclose(gamesIdsFile);
}

void getPlayerName(int orderNumber) {
	// Gets name of only one player per call

	printf("Please enter player#%d name (max 30 symbols): \n", orderNumber);
	scanf("%s", playersNames[orderNumber]);

}
void getPlayersNames() {
	// Gets names of many players

	for (int i = 0; i < PLAYER_COUNT; i++) {
		getPlayerName(i);
	}

}

void copyBoard(char destination[BOARD_HEIGHT][BOARD_WIDTH], const char source[BOARD_HEIGHT][BOARD_WIDTH]) {
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			destination[i][j] = source[i][j];
		}
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

int displayPlayersNames() {
	printf(SEPARATOR_LINE);
	for (int i = 0; i < PLAYER_COUNT; i++) {
		printf("Player#%d: %s\n", i + 1, playersNames[i]);
	}

	return SUCCESSFUL_EXECUTION;
}

void saveResult(int gameResult) {
	char pathToResults[100] = "..\\storage\\results";
	strcat(pathToResults, ".txt");

	FILE *resultFile = fopen(pathToResults, "a");
	fprintf(resultFile, "ID: %s, X-player name: %s, O-player name: %s, result: %d\n", gameIdString, playersNames[0], playersNames[1], gameResult);

	fclose(resultFile);
}
int getInputInBoundaries(const char *textToPrint, int leftBoundary, int rightBoundary) {
	int input = -1;

	do {
		printf("%s", textToPrint);
		scanf("%d", &input);
	} while (input < leftBoundary || input > rightBoundary);

	return input;
}


int initializeGame(int GameIsLoaded, int loadedTurn) {
	// Prepares the game to play
	gameIdIsAcquired = 0;
	if (GameIsLoaded) {
		return loadedTurn;
	}
	getPlayersNames(playersNames);
	initializeBoard(board);

	return 1;
}

int checkIfFourCharactersInWinningSequenceAreSame(const char sign) {
	// Checks if all the cells in 'winningSequence' array have the same character ('X' or 'Y')

	for (int i = 0; i < 4; i++) {
		if (winningSequence[i].sign != sign) {
			return NOT_CONNECTED;
		}
	}
	return CONNECTED;
}

void formHorizontalSequence(const Cell startingCellInSequence) {
	// Fills 'winningSequence' array with four horizontally consecutive cells

	int y = startingCellInSequence.y;
	int x = startingCellInSequence.x;

	for (int i = 0; i < 4; i++) {
		winningSequence[i].sign = board[y][x + i];

		winningSequence[i].x = x + i;
		winningSequence[i].y = y;
	}
}

void formVerticalSequence(const Cell startingCellInSequence) {
	// Fills 'winningSequence' array with four vertically consecutive cells

	int y = startingCellInSequence.y;
	int x = startingCellInSequence.x;

	for (int i = 0; i < 4; i++) {
		winningSequence[i].sign = board[y + i][x];

		winningSequence[i].x = x;
		winningSequence[i].y = y + i;
	}
}
void formDiagonalTangentNegativeSequence(const Cell startingCellInSequence) {
	// Fills 'winningSequence' array with four diagonally (whose slope is negative) consecutive cells

	int y = startingCellInSequence.y;
	int x = startingCellInSequence.x;

	for (int i = 0; i < 4; i++) {
		winningSequence[i].sign = board[y + i][x + i];

		winningSequence[i].x = x + i;
		winningSequence[i].y = y + i;
	}
}
void formDiagonalTangentPositiveSequence(const Cell startingCellInSequence) {
	// Fills 'winningSequence' array with four diagonally (whose slope is positive) consecutive cells

	int y = startingCellInSequence.y;
	int x = startingCellInSequence.x;

	for (int i = 0; i < 4; i++) {
		winningSequence[i].sign = board[y - i][x + i];

		winningSequence[i].x = x + i;
		winningSequence[i].y = y - i;
	}
}


int checkWinCondition(int turn) {
	char sign = chooseSignBasedOnTurn(turn);


	int connectedFourHorizontally = 0;
	int connectedFourVertically = 0;
	int connectedFourDiagonallyTangentNegative = 0;
	int connectedFourDiagonallyTangentPositive = 0;

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			const Cell startingCell = makeCell(board[i][j], j, i);
			if (j < 4) {
				formHorizontalSequence(startingCell);
				connectedFourHorizontally = checkIfFourCharactersInWinningSequenceAreSame(sign);
				if (connectedFourHorizontally) {
					return WIN;
				}
			}

			if (i < 3) {
				formVerticalSequence(startingCell);
				connectedFourVertically = checkIfFourCharactersInWinningSequenceAreSame(sign);
				if (connectedFourVertically) {
					return WIN;
				}
			}

			if (i < 3 && j < 4) {
				formDiagonalTangentNegativeSequence(startingCell);
				connectedFourDiagonallyTangentNegative = checkIfFourCharactersInWinningSequenceAreSame(sign);
				if (connectedFourDiagonallyTangentNegative) {
					return WIN;
				}
			}

			if (i >= 3 && j < 4) {
				formDiagonalTangentPositiveSequence(startingCell);
				connectedFourDiagonallyTangentPositive = checkIfFourCharactersInWinningSequenceAreSame(sign);
				if (connectedFourDiagonallyTangentPositive) {
					return WIN;
				}
			}
		}
	}
	return NO_WIN;
}
int checkDrawCondition() {
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

int checkGameEndConditions(int turn) {
	int win = checkWinCondition(turn);
	if (win) {
		return WIN * turn;
	}

	int draw = checkDrawCondition(board);
	if (draw) {
		return DRAW;
	}

	return CONTINUE;
}

int simulateGame(int loadedTurn) {
	// Game main loop
	int turn = loadedTurn;
	int playerInput = 0, gameEnd = 0;
	int emptyCellLineNumber = -1;
	while (!gameEnd) {

		displayBoard(board, NO_WIN, 1);

		playerInput = getInputInBoundaries("Enter the order number of a column or '0' to save the game:", 0, 7);

		if (playerInput == 0) {
			saveGame(turn);
			continue;
		}

		int column = playerInput-1;
		emptyCellLineNumber = findFirstEmptyCellFromBottomOfColumn(column);
		if (emptyCellLineNumber == ILLEGAL_MOVE_FULL_COLUMN) {
			printf("Illegal move: there is no free space in this column");
			continue;
		}

		putDotOnField(emptyCellLineNumber, column, turn);

		gameEnd = checkGameEndConditions(turn);
		if (gameEnd) {
			return gameEnd;
		}

		turn *= (-1);
	}
}


void displayMainMenu() {
	printf(SEPARATOR_LINE);
	printf("Welcome to the Connect four!\n\n");
	printf("What would you like to do:\n");
	printf("1) Play a new game\n");
	printf("2) Load an already saved game\n");
	printf("3) Exit the game\n");
}

void displayBoardBaseOnGameResult(int gameResult) {
	switch (gameResult) {
		case WIN:
			displayBoard(board, WIN, 1);
			printf("The game has ended. X-Player %s has won!\n", playersNames[0]);
			break;
		case (-WIN):
			displayBoard(board, WIN, 1);
			printf("The game has ended. O-Player %s has won!\n", playersNames[1]);
			break;
		case (DRAW):
			displayBoard(board, NO_WIN, 1);
			printf("The game has ended. NO WINNERS TODAY!\n");
			break;
	}
}

int playGameOption(int GameIsLoaded, int loadedTurn) {
	int turn = initializeGame(GameIsLoaded, loadedTurn);

	int gameResult = simulateGame(turn);
	displayBoardBaseOnGameResult(gameResult);

	if (!gameIdIsAcquired) {
		getAndUpdateGameId();
	}

	saveResult(gameResult);

	printf("What would you like to do?\n");
	printf("1) Play a new game\n");
	printf("2) Return to main menu\n");
	int option = getInputInBoundaries("", 1, 2);
	option = option*(-1) + 2;
	return option;
}


void displayLoadGameMenu() {
	printf(SEPARATOR_LINE);
	printf("1) List all saved games\n");
	printf("2) List all saved games for a particular player\n");
	printf("3) Show the board of one of the saved games\n");
	printf("4) Load a game\n");
	printf("5) Return to main menu\n");
}

void getBoardFromFile(FILE *savedGameFile, char boardPlaceHolder[BOARD_HEIGHT][BOARD_WIDTH]) {
	int y = 0, x = 0;
	char ch = fgetc(savedGameFile); // get first whitespace symbol
	while ((ch = fgetc(savedGameFile)) != EOF && y < BOARD_HEIGHT) {
		if (ch == '\0') {
			continue;
		}

		if (ch == '\n') {
			y++;
			x = 0;
			continue;
		}

		boardPlaceHolder[y][x++] = ch;
	}
}

void readLineOfSavedGameFile(FILE *savedGameFile, char *placeHolder) {
	char dummy[BUFFER_SIZE];

	if (placeHolder != NULL) {
		fscanf(savedGameFile, "%s", placeHolder);
	} else {
		fscanf(savedGameFile, "%s", dummy);
	}
}

void getGameInfo(const char *gameIdToDisplay, char *xPlayerName, char *oPlayerName, char *numberOfEmptyCells, char *turn, char boardPlaceHolder[BOARD_HEIGHT][BOARD_WIDTH]) {
	char pathToSavedGameFile[BUFFER_SIZE] = "..\\storage\\games\\";

	strcat(pathToSavedGameFile, gameIdToDisplay);
	strcat(pathToSavedGameFile, ".txt");

	FILE *savedGameFile = fopen(pathToSavedGameFile, "r");

	readLineOfSavedGameFile(savedGameFile, NULL); // read game id from a file (not useful in program execution)
	readLineOfSavedGameFile(savedGameFile, xPlayerName);
	readLineOfSavedGameFile(savedGameFile, oPlayerName);
	readLineOfSavedGameFile(savedGameFile, numberOfEmptyCells);
	readLineOfSavedGameFile(savedGameFile, turn);
	if (boardPlaceHolder != NULL) {
		getBoardFromFile(savedGameFile, boardPlaceHolder);
	}


	fclose(savedGameFile);
}

void listAllSavedGames() {
	printf(SEPARATOR_LINE);

	char pathToGamesIdsFile[BUFFER_SIZE] = "..\\storage\\gamesIDs.txt";

	FILE *gamesIdsFile = fopen(pathToGamesIdsFile, "r");

	char gameIdToDisplay[BUFFER_SIZE];
	while (fscanf(gamesIdsFile, "%s", &gameIdToDisplay) > 0) {
		char numberOfEmptyCells[BUFFER_SIZE], oPlayerName[BUFFER_SIZE], xPlayerName[BUFFER_SIZE];

		getGameInfo(gameIdToDisplay, xPlayerName, oPlayerName, numberOfEmptyCells, NULL, NULL);
		printf("%s, %s, %s, %s\n", gameIdToDisplay, xPlayerName, oPlayerName, numberOfEmptyCells);
	}

	fclose(gamesIdsFile);
}

int checkIfPlayerNameIsInGameInfo(const char *playerName, const char *xPlayerName, const char *oPlayerName) {
	int itIsOPlayer = !strcmp(playerName, oPlayerName);
	int itIsXPlayer = !strcmp(playerName, xPlayerName);

	return itIsOPlayer || itIsXPlayer;
}

void listAllSavedGamesForPlayer() {
	printf(SEPARATOR_LINE);

	char pathToGamesIdsFile[BUFFER_SIZE] = "..\\storage\\gamesIDs.txt";

	FILE *gamesIdsFile = fopen(pathToGamesIdsFile, "r");

	char playerName[BUFFER_SIZE];
	scanf("%s", playerName);

	char gameIdToCheck[BUFFER_SIZE];
	while (fscanf(gamesIdsFile, "%s", &gameIdToCheck) > 0) {
		char numberOfEmptyCells[BUFFER_SIZE], oPlayerName[BUFFER_SIZE], xPlayerName[BUFFER_SIZE];

		getGameInfo(gameIdToCheck, xPlayerName, oPlayerName, numberOfEmptyCells, NULL, NULL);

		int playerNameIsInGameInfo = checkIfPlayerNameIsInGameInfo(playerName, xPlayerName, oPlayerName);
		if (playerNameIsInGameInfo) {
			printf("%s, %s, %s, %s\n", gameIdToCheck, xPlayerName, oPlayerName, numberOfEmptyCells);
		}
	}

	fclose(gamesIdsFile);

}
void showTheBoardOfOneSavedGame() {
	printf(SEPARATOR_LINE);
	char gameIdToDisplayBoard[BUFFER_SIZE];
	scanf("%s", gameIdToDisplayBoard);

	char oPlayerName[BUFFER_SIZE];
	char xPlayerName[BUFFER_SIZE];
	char boardPlaceHolder[BOARD_HEIGHT][BOARD_WIDTH];

	getGameInfo(gameIdToDisplayBoard, xPlayerName, oPlayerName, NULL, NULL, boardPlaceHolder);

	printf("%s %s\n", xPlayerName, oPlayerName);
	displayBoard(boardPlaceHolder, NO_WIN, 0);

}
int loadGameById() {
	printf(SEPARATOR_LINE);
	char gameIdToLoad[BUFFER_SIZE];
	scanf("%s", gameIdToLoad);

	char turn[BUFFER_SIZE];
	getGameInfo(gameIdToLoad, playersNames[0], playersNames[1], NULL, turn, board);

	const int turnNumber = atoi(turn);
	return turnNumber;
}

int loadGameOption() {
	while (1) {
		displayLoadGameMenu();
		int inputOption = getInputInBoundaries("", 0, 7);

		switch (inputOption) {
			case 1:
				listAllSavedGames();
			break;
			case 2:
				listAllSavedGamesForPlayer();
			break;
			case 3:
				showTheBoardOfOneSavedGame();
			break;
			case 4:
				return loadGameById();
			default:
				return SUCCESSFUL_EXECUTION;
		}
	}
}

int exitGameOption() {
	printf("Thank you for playing! See you next time.\n");
	return 1;
}

int main(void) {
	char placeHolder = 'a';
	int inputOption;
	int playAgain = 0;

	int isExit = 0;
	while (!isExit) {
		if (!playAgain) {
			displayMainMenu();
			inputOption = getInputInBoundaries("", 1, 3);
		} else {
			inputOption = playAgain;
		}

		switch (inputOption) {
			case 1:
				playAgain = playGameOption(0, 1);
				break;
			case 2: {
				int loadedTurn = loadGameOption();
				if (loadedTurn) {
					playAgain = playGameOption(1, loadedTurn);
				}
				break;
			}
				break;
			case 3:
				isExit = exitGameOption();
		}
	}
	scanf("%c", &placeHolder);
	return SUCCESSFUL_EXECUTION;
}