#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define SEPARATOR_LINE "-------------------------------------------------------------\n"

#define BUFFER_SIZE 256

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6
#define LENGTH_OF_WINNING_SEQUENCE 4

#define MAX_PLAYER_NAME_LENGTH (30 + 1) // "+1" for null terminator
#define PLAYER_COUNT 2

#define SIGN_IN_WINNING_SEQUENCE 'Y'

// All files expected to be in "storage" directory
#define STORAGE_PATH "..\\storage\\"
#define SAVED_GAMES_DIR_NAME "games\\"
#define GAMES_IDS_FILE_NAME "gamesIDs"
#define RESULTS_FILE_NAME "results"
#define PREVIOUS_GAME_ID_FILE_NAME "previousGameID"
#define STORAGE_FILES_EXTENSION ".txt"

typedef enum {
	ILLEGAL_MOVE_FULL_COLUMN = -10,
	INVALID_GAME_ID,
} ErrorCodes;

typedef enum {
	GAME_IS_NOT_LOADED,
	GAME_IS_LOADED,
} GameLoaded;

typedef enum {
	DRAW = -1,
	CONTINUE,
	WIN_X_PLAYER,
	WIN_O_PLAYER,
} GameResult;

typedef enum {
	FALSE,
	TRUE,
} Bool;

typedef enum {
	LIST_ALL_SAVED_GAMES = 1,
	LIST_ALL_SAVED_GAMES_PLAYER,
	SHOW_THE_BOARD_OF_SAVED_GAME,
	LOAD_SAVED_GAME_BY_ID,
	RETURN_TO_MAIN_MENU,
} LoadGameOption;

typedef enum {
	PLAY_NEW_GAME = 1,
	LOAD_SAVED_GAME,
	EXIT_GAME,
} MainMenuOption;

typedef enum {
	O_PLAYER = -1,
	DEFAULT,
	X_PLAYER,
} Turn;

typedef struct{
	char sign;
	int x, y;
} Cell;


void findPathToNeededFile(char pathHolder[BUFFER_SIZE], const char *fileName, const Bool GameSaveFile) {
	/*
	 * This function saves a path to a desired file in 'pathHolder' variable.
	 * It is needed for concatenation of strings constants of different paths (which cannot easily be done in C), so that
	 * the end user could configure file structure with ease.
	 *
	 * It is expected that 'pathHolder' is empty.
	 */

	strcat(pathHolder, STORAGE_PATH);
	if (GameSaveFile) {
		strcat(pathHolder, SAVED_GAMES_DIR_NAME);
	}
	strcat(pathHolder, fileName);
	strcat(pathHolder, STORAGE_FILES_EXTENSION);
}

Cell makeCell(const char board[BOARD_HEIGHT][BOARD_WIDTH], const int x, const int y) {
	const Cell cell = {
		.sign = board[y][x],
		.x = x,
		.y = y
	};
	return cell;
}

Cell winningSeq[LENGTH_OF_WINNING_SEQUENCE];
Bool gameIdIsAcquired = FALSE;
unsigned long long gameID;
char gameIdString[BUFFER_SIZE] = {0};
char mainBoard[BOARD_HEIGHT][BOARD_WIDTH];
char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH];

Bool checkCellIsInWinningSeq(const Cell cell) {
	for (int i = 0; i < 4; i++) {
		if (cell.x == winningSeq[i].x && cell.y == winningSeq[i].y) {
			return TRUE;
		}
	}
	return FALSE;
}

void getGameId(void) {
	char pathToPreviousGameId[BUFFER_SIZE] = {0};
	findPathToNeededFile(pathToPreviousGameId, PREVIOUS_GAME_ID_FILE_NAME, FALSE);

	FILE *previousGameIdFile = fopen(pathToPreviousGameId, "r");
	fscanf(previousGameIdFile, "%llu", &gameID);
	fclose(previousGameIdFile);

	gameID++;
	gameIdIsAcquired = 1;
}

void updateGameId(void) {
	char pathToPreviousGameId[BUFFER_SIZE] = {0};
	findPathToNeededFile(pathToPreviousGameId, PREVIOUS_GAME_ID_FILE_NAME, FALSE);

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

char chooseSignToDisplay(const Cell cellOnBoard, const GameResult gameResult) {
	// This function decides what sign to show: which is on the board ('.', 'O', 'X') or 'Y' which indicates that smb
	// won

	const Bool smbWon = (gameResult == WIN_X_PLAYER || gameResult == WIN_O_PLAYER);
	const Bool cellIsInWinningSeq = checkCellIsInWinningSeq(cellOnBoard);

	if (smbWon && cellIsInWinningSeq) {
		return SIGN_IN_WINNING_SEQUENCE;
	}

	return cellOnBoard.sign;
}

void displayBoard(const char board[BOARD_HEIGHT][BOARD_WIDTH], const GameResult gameResult, const Bool separate,
	FILE *outputFile) {
	/*
	 * Writes a board in a chosen output file (stdout included).
	 */

	assert(outputFile != NULL);

	if (separate) {
		printf(SEPARATOR_LINE);
	}

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			const Cell cell = makeCell(board, j, i);
			const char signToDisplay = chooseSignToDisplay(cell, gameResult);
			fprintf(outputFile, "%c", signToDisplay);

		}
		fprintf(outputFile, "\n");
	}
	if (separate) {
		fprintf(outputFile, "\n");
	}
}

char chooseSignBasedOnTurn(const Turn turn) {
	switch (turn) {
		case X_PLAYER:
			return 'X';
		case O_PLAYER:
			return 'O';
		case DEFAULT:
		default:
			return '?';
	}
}

int checkCellIsEmpty(const char cell) {
	return (cell == '.');
}

int findFirstEmptyCellDownBelow(const int column) {
	/* This function returns number of the first line from the bottom with empty space in a column
	 * If there is no such line, it returns -1
	 */

	int cellIsEmpty = 0;;
	for (int emptyCellLineNumber = BOARD_HEIGHT-1; emptyCellLineNumber >= 0 ; emptyCellLineNumber--) {

		cellIsEmpty = checkCellIsEmpty(mainBoard[emptyCellLineNumber][column]);
		if (cellIsEmpty) {
			return emptyCellLineNumber;
		}
	}
	return ILLEGAL_MOVE_FULL_COLUMN;
}

void putDotOnField(char board[BOARD_HEIGHT][BOARD_WIDTH], const int line, const int column, const int turn) {
	const char sign = chooseSignBasedOnTurn(turn);
	board[line][column] = sign;
}

int countEmptyCellsOnBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	int emptyCells = 0;
	int cellIsEmpty = 0;
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			cellIsEmpty = checkCellIsEmpty(board[i][j]);
			emptyCells += cellIsEmpty;
		}
	}

	return emptyCells;
}

void saveGame(const int turn) {
	// Saves players' names, a current turn and current board to a <gameID>.txt file
	// and
	// Appends gameID to gamesIDs.txt file

	char pathToGameSaveFile[BUFFER_SIZE] = {0};

	if (!gameIdIsAcquired) {
		getAndUpdateGameId();
	}

	findPathToNeededFile(pathToGameSaveFile, gameIdString, TRUE);

	const int emptyCells = countEmptyCellsOnBoard(mainBoard);

	FILE *gameSaveFile = fopen(pathToGameSaveFile, "w");
	fprintf(gameSaveFile, "%llu\n%s\n%s\n%d\n%d\n", gameID, playersNames[0], playersNames[1], emptyCells, turn);
	displayBoard(mainBoard, CONTINUE, FALSE, gameSaveFile);
	fclose(gameSaveFile);

	char pathToGamesIdsFile[BUFFER_SIZE] = {0};
	findPathToNeededFile(pathToGamesIdsFile, GAMES_IDS_FILE_NAME, FALSE);

	FILE *gamesIdsFile = fopen(pathToGamesIdsFile, "a");
	fprintf(gamesIdsFile, "%llu\n", gameID);
	fclose(gamesIdsFile);

	printf("Game has been successfully saved under ID: %s\n", gameIdString);
}

void getPlayerName(const int orderNumber) {
	// Gets name of only one player per call

	printf("Please enter player#%d name (max 30 symbols):\n>>", orderNumber);
	scanf("%s", playersNames[orderNumber]);
}
void getPlayersNames(char playersNames[PLAYER_COUNT][MAX_PLAYER_NAME_LENGTH]) {
	// Gets names of many players

	for (int i = 0; i < PLAYER_COUNT; i++) {
		getPlayerName(i);
	}
}

void initializeBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]) {
	// Fills the board with periods to avoid unsupported behavior

	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			board[i][j] = '.';
		}
	}
}

void getStringResult(char *placeHolder, const GameResult gameResult) {
	switch (gameResult) {
		case WIN_X_PLAYER:
			strcat(placeHolder, "X has won!");
			break;
		case WIN_O_PLAYER:
			strcat(placeHolder, "O has won!");
			break;
		case DRAW:
			strcat(placeHolder, "Draw!");
			break;
		default:
			strcat(placeHolder, "Unknown!");
	}
}

void saveResult(const GameResult gameResult) {
	char pathToResultsFile[BUFFER_SIZE] = {0};
	findPathToNeededFile(pathToResultsFile, RESULTS_FILE_NAME, FALSE);

	char stringResult[BUFFER_SIZE] = {0};
	getStringResult(stringResult, gameResult);

	FILE *resultFile = fopen(pathToResultsFile, "a");
	fprintf(resultFile,"ID: %s, X-player name: <%s>, O-player name: <%s>, result: %s\n",gameIdString,
		playersNames[0], playersNames[1], stringResult);

	fclose(resultFile);
}
int getInputInBoundaries(const char *textToPrint, const int leftBoundary, const int rightBoundary) {
	int input = -1;

	do {
		printf("%s", textToPrint);
		scanf("%d", &input);
	} while (input < leftBoundary || input > rightBoundary);

	return input;
}


Turn initializeGame(const int GameIsLoaded, const Turn loadedTurn) {
	// Prepares the game to play: gets names of the players and initializes the main board.
	// Returns current player to place a dot (turn).

	gameIdIsAcquired = 0;
	if (GameIsLoaded) {
		return loadedTurn;
	}

	getPlayersNames(playersNames);
	initializeBoard(mainBoard);

	return X_PLAYER;
}

Bool checkEnoughSpaceHrzntalSeq(const Cell startingCellInSeq) {
	return (startingCellInSeq.x < 4);
}
Bool checkEnoughSpaceAboveVrtcalSeq(const Cell startingCellInSeq) {
	return (startingCellInSeq.y >= 3);
}
Bool checkEnoughSpaceBelowVrtcalSeq(const Cell startingCellInSeq) {
	return (startingCellInSeq.y < 4);
}
Bool checkEnoughSpaceTangentNegSeq(const Cell startingCellInSeq) {
	const int EnoughSpaceBelow = checkEnoughSpaceBelowVrtcalSeq(startingCellInSeq);
	const int EnoughSpace = checkEnoughSpaceHrzntalSeq(startingCellInSeq);

	return (EnoughSpaceBelow && EnoughSpace);
}
Bool checkEnoughSpaceTangentPosSeq(const Cell startingCellInSeq) {
	const int EnoughSpaceAbove = checkEnoughSpaceAboveVrtcalSeq(startingCellInSeq);
	const int EnoughSpace = checkEnoughSpaceHrzntalSeq(startingCellInSeq);

	return (EnoughSpaceAbove && EnoughSpace);
}

Cell newFormHrzntalSeqEnd(const Cell startingCellInSeq) {
	const int x = startingCellInSeq.x;
	const int y = startingCellInSeq.y;

	return makeCell(mainBoard, x + 3, y);
}
Cell newFormVrtcalSeqEnd(const Cell startingCellInSeq) {
	const int x = startingCellInSeq.x;
	const int y = startingCellInSeq.y;

	return makeCell(mainBoard, x, y + 3);
}
Cell newFormTangentNegSeqEnd(const Cell startingCellInSeq) {
	const int x = startingCellInSeq.x;
	const int y = startingCellInSeq.y;

	return makeCell(mainBoard, x + 3, y + 3);
}
Cell newFormTangentPosSeqEnd(const Cell startingCellInSeq) {
	const int x = startingCellInSeq.x;
	const int y = startingCellInSeq.y;

	return makeCell(mainBoard, x + 3, y - 3);
}

Bool checkCellsInSeqAreSame(const Cell sequence[LENGTH_OF_WINNING_SEQUENCE], const char playerSign) {
	for (int i = 0; i < LENGTH_OF_WINNING_SEQUENCE; i++) {
		if(sequence[i].sign != playerSign) {
			return FALSE;
		}
	}
	return TRUE;
}

void formSeq(Cell sequencePlaceHolder[LENGTH_OF_WINNING_SEQUENCE]) {
	/*
	 * Forms an array of Cells by finding difference between coordinates of the beginning and of the end of the sequence
	 * points by calculating "stepX" and "stepY", and then adding them to the start depending on the distance between
	 * two points
	 */

	int xStart = sequencePlaceHolder[0].x;
	int yStart = sequencePlaceHolder[0].y;

	int xEnd = sequencePlaceHolder[LENGTH_OF_WINNING_SEQUENCE - 1].x;
	int yEnd = sequencePlaceHolder[LENGTH_OF_WINNING_SEQUENCE - 1].y;

	// sign is needed to ensure that boundaries in a range are also included
	int signX = (xEnd - xStart > 0) ? 1 : -1;
	int signY = (yEnd - yStart > 0) ? 1 : -1;

	int stepX = (xEnd - xStart + 1 * signX)/LENGTH_OF_WINNING_SEQUENCE;
	int stepY = (yEnd - yStart + 1 * signY)/LENGTH_OF_WINNING_SEQUENCE;

	for (int i = 1; i <= LENGTH_OF_WINNING_SEQUENCE - 2; i++) {
		sequencePlaceHolder[i] = makeCell(mainBoard, xStart + i*stepX, yStart + i*stepY);
	}
}

void copySeq(Cell destination[LENGTH_OF_WINNING_SEQUENCE], const Cell source[LENGTH_OF_WINNING_SEQUENCE]) {
	for (int i = 0; i < LENGTH_OF_WINNING_SEQUENCE; i++) {
		destination[i] = source[i];
	}
}

Bool newCheckSeqIsWinning(Bool (*checkEnoughSpace)(const Cell), const Cell startingCellInSeq,
	Cell (*newFormSeqEnd)(const Cell), const char playerSign) {
	/*
	 * This function encapsulates the whole process of deciding whether a provided sequence is winning or not.
	 * Checks if the sequence is within the boundaries to avoid "Segmentation Fault".
	 * Forms a sequence consisting of four cells.
	 * Then checks if it contains the same characters, which would mean that they are connected, therefore the
	 * game has ended in someone's win.
	 * If they are the same, 'winningSeq' array is filled with the contents of 'sequence'.
	 * Finally, a boolean value is returned indicating status of the sequence.
	 */

	const int enoughSpace = (*checkEnoughSpace)(startingCellInSeq);
	if (!enoughSpace) {
		return FALSE;
	}

	const Cell endingCellInSeq = newFormSeqEnd(startingCellInSeq);
	Cell sequence[LENGTH_OF_WINNING_SEQUENCE] = {
		startingCellInSeq,
		{0},
		{0},
		endingCellInSeq,
	};
	formSeq(sequence);

	const Bool cellsInSeqSame = checkCellsInSeqAreSame(sequence, playerSign);
	if (cellsInSeqSame) {
		copySeq(winningSeq, sequence);
	}

	return cellsInSeqSame;
}

Bool checkWinCondition(const Turn turn) {
	const char playerSign = chooseSignBasedOnTurn(turn);
	Bool win = FALSE;
	int x = 0, y = 0;

	while (y < BOARD_HEIGHT && !win) {
		const Cell startingCellInSeq = makeCell(mainBoard, x++, y);

		const int connectedFourHrzntally = newCheckSeqIsWinning(&checkEnoughSpaceHrzntalSeq, startingCellInSeq,
			&newFormHrzntalSeqEnd, playerSign);
		win = (win || connectedFourHrzntally);

		const int connectedFourVrtcally = newCheckSeqIsWinning(&checkEnoughSpaceBelowVrtcalSeq, startingCellInSeq,
			&newFormVrtcalSeqEnd, playerSign);
		win = (win || connectedFourVrtcally);

		const int connectedFourTangentNegative = newCheckSeqIsWinning(&checkEnoughSpaceTangentNegSeq, startingCellInSeq,
			&newFormTangentNegSeqEnd, playerSign);
		win = (win || connectedFourTangentNegative);

		const int connectedFourTangentPositive = newCheckSeqIsWinning(&checkEnoughSpaceTangentPosSeq, startingCellInSeq,
			&newFormTangentPosSeqEnd, playerSign);
		win = (win || connectedFourTangentPositive);
		if (x >= BOARD_WIDTH) {
			x = 0;
			y++;
		}
	}
	return win;
}

Bool checkDrawCondition(void) {
	int cellIsEmpty = 0;
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			cellIsEmpty = checkCellIsEmpty(mainBoard[i][j]);
			if (cellIsEmpty) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

GameResult checkGameEndConditions(const int turn) {
	Bool win = checkWinCondition(turn);
	if (win && turn == X_PLAYER) {
		return WIN_X_PLAYER;
	} else if (win && turn == O_PLAYER) {
		return WIN_O_PLAYER;
	}

	int draw = checkDrawCondition();
	if (draw) {
		return DRAW;
	}

	return CONTINUE;
}

GameResult simulateGame(const Turn loadedTurn) {
	/*
	 * Game main loop
	 *
	 * Displays the main board, asks a player for a valid input, updates board accordingly, checks end conditions.
	 * Return the game result.
	 */

	Turn currentTurn = loadedTurn;
	int playerInput = 0;
	GameResult gameEnd = CONTINUE;

	int counter = 0;
	while (!gameEnd && counter++ < 42) {

		displayBoard(mainBoard, CONTINUE, TRUE, stdout);
		printf("Turn: %c\n", chooseSignBasedOnTurn(currentTurn));
		playerInput = getInputInBoundaries("Enter the order number of a column (1-7) or '0' to save the game:\n>>",
			0,7);

		if (playerInput == 0) {
			saveGame(currentTurn);
			continue;
		}

		const int column = playerInput-1;
		const int emptyCellLineNumber = findFirstEmptyCellDownBelow(column);
		if (emptyCellLineNumber == ILLEGAL_MOVE_FULL_COLUMN) {
			printf("Illegal move: there is no free space in this column");
			continue;
		}

		putDotOnField(mainBoard, emptyCellLineNumber, column, currentTurn);

		gameEnd = checkGameEndConditions(currentTurn);
		if (gameEnd) {
			return gameEnd;
		}

		currentTurn *= (-1);
	}

	return DRAW;
}


void displayMainMenu(void) {
	printf(SEPARATOR_LINE);
	printf("Welcome to the Connect four!\n\n");
	printf("What would you like to do:\n");
	printf("1) Play a new game\n");
	printf("2) Load an already saved game\n");
	printf("3) Exit the game\n");
}

void displayBoardBasedOnGameResult(const GameResult gameResult) {
	switch (gameResult) {
		case WIN_X_PLAYER:
			displayBoard(mainBoard, WIN_X_PLAYER, TRUE, stdout);
			printf("The game has ended. X-Player %s has won!\n", playersNames[0]);
			break;
		case (WIN_O_PLAYER):
			displayBoard(mainBoard, WIN_O_PLAYER, TRUE, stdout);
			printf("The game has ended. O-Player %s has won!\n", playersNames[1]);
			break;
		case (DRAW):
			displayBoard(mainBoard, DRAW, TRUE, stdout);
			printf("The game has ended. NO WINNERS TODAY!\n");
			break;
		default:
			break;
	}
}

Bool playGameOption(const Bool GameIsLoaded, const int loadedTurn) {
	/*
	 * Whenever a game takes place, this function is invoked.
	 * This function encapsulates Initialization of a game, "simulation" of a game (the gameplay process), saving the
	 * result and returning a boolean value indicating desire of a user to play again.
	 *
	 * That being said, this function is responsible for all the processes related to a game.
	 */

	const Turn calculatedTurn = initializeGame(GameIsLoaded, loadedTurn);

	const GameResult gameResult = simulateGame(calculatedTurn);
	displayBoardBasedOnGameResult(gameResult);

	if (!gameIdIsAcquired) {
		getAndUpdateGameId();
	}

	saveResult(gameResult);

	printf("Would you like to play again?\n");
	printf("1) Play a new game\n");
	printf("2) Return to main menu\n");

	const int option = getInputInBoundaries("Enter a valid option (1-2):\n>>", 1, 2);
	return (option == 1);
}


void displayLoadGameMenu(void) {
	printf(SEPARATOR_LINE);
	printf("1) List all saved games\n");
	printf("2) List all saved games for a particular player\n");
	printf("3) Show the board of one of the saved games\n");
	printf("4) Load a game by ID\n");
	printf("5) Return to main menu\n");
}

void getBoardFromFile(FILE *savedGameFile, char boardPlaceHolder[BOARD_HEIGHT][BOARD_WIDTH]) {
	int y = 0, x = 0;
	char ch = fgetc(savedGameFile); // get first whitespace symbol
	while ((ch = fgetc(savedGameFile)) != EOF && y < BOARD_HEIGHT) {
		if (ch == '\0') {
			continue;
		} else if (ch == '\n') {
			y++;
			x = 0;
			continue;
		}

		boardPlaceHolder[y][x++] = ch;
	}
}

void readLineOfSavedGameFile(FILE *savedGameFile, char *placeHolder) {
	/*
	 * This function is needed to read information or only move the cursor (if the information in the line is "unnecessary"
	 * for that specific 'getGameInfo' call).
	 */

	if (placeHolder) {
		fscanf(savedGameFile, "%s", placeHolder);
		return;
	}
	char dummy[BUFFER_SIZE] = {0};
	fscanf(savedGameFile, "%s", dummy);
}

void getGameInfo(const char *gameIdToDisplay, char *xPlayerName, char *oPlayerName, char *numberOfEmptyCells, char *turn,
	char boardPlaceHolder[BOARD_HEIGHT][BOARD_WIDTH]) {
	/*
	 * Provided with a valid ID,f etches information about the game.
	 */


	char pathToSavedGameFile[BUFFER_SIZE] = {0};
	findPathToNeededFile(pathToSavedGameFile, gameIdToDisplay, TRUE);

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

int checkPlayerNameInGameInfo(const char *playerName, const char *xPlayerName, const char *oPlayerName) {
	const int itIsOPlayer = !strcmp(playerName, oPlayerName);
	const int itIsXPlayer = !strcmp(playerName, xPlayerName);

	return itIsOPlayer || itIsXPlayer;
}

void listAllSavedGames(Bool forPlayer) {
	/*
	 * Lists all saved games.
	 * If "forPlayer" is "TRUE", asks for the name of the player and shows games only for this specific player.
	 */

	printf(SEPARATOR_LINE);
	char pathToGamesIdsFile[BUFFER_SIZE] = {0};
	findPathToNeededFile(pathToGamesIdsFile, GAMES_IDS_FILE_NAME, FALSE);

	FILE *gamesIdsFile = fopen(pathToGamesIdsFile, "r");

	char playerName[BUFFER_SIZE] = "";
	if (forPlayer) {
		printf("Enter the name of the player:\n>>");
		scanf("%s", playerName);
	}

	char gameIdToDisplay[BUFFER_SIZE];
	while (fscanf(gamesIdsFile, "%s", &gameIdToDisplay) > 0) {
		char numberOfEmptyCells[BUFFER_SIZE], oPlayerName[BUFFER_SIZE], xPlayerName[BUFFER_SIZE];
		getGameInfo(gameIdToDisplay, xPlayerName, oPlayerName, numberOfEmptyCells, NULL, NULL);

		if (!forPlayer) {
			printf("%s, %s, %s, %s\n", gameIdToDisplay, xPlayerName, oPlayerName, numberOfEmptyCells);
			continue;
		}

		int playerNameIsInGameInfo = checkPlayerNameInGameInfo(playerName, xPlayerName, oPlayerName);
		if (playerNameIsInGameInfo) {
			printf("%s, %s, %s, %s\n", gameIdToDisplay, xPlayerName, oPlayerName, numberOfEmptyCells);
		}
	}
	fclose(gamesIdsFile);
}

Bool checkGameIdExists(const char *enteredGameId) {
	char pathToGamesIdsFile[BUFFER_SIZE] = {0};
	findPathToNeededFile(pathToGamesIdsFile, GAMES_IDS_FILE_NAME, FALSE);

	FILE *gamesIdsFile = fopen(pathToGamesIdsFile, "r");

	char gameIdToCheck[BUFFER_SIZE];
	while (fscanf(gamesIdsFile, "%s", gameIdToCheck) > 0) {
		if (!strcmp(enteredGameId, gameIdToCheck)) {
			fclose(gamesIdsFile);
			return TRUE;
		}
	}

	fclose(gamesIdsFile);
	return FALSE;
}


void showTheBoardOfOneSavedGame(void) {

	/*
	 * Shows the contents of a game: players' names and the board.
	 * If there is no game with such ID, returns error code "INVALID_GAME_ID" (-9).
	 */

	printf(SEPARATOR_LINE);
	char gameIdToDisplayBoard[BUFFER_SIZE];

	printf("Enter the ID of the game:\n>>");
	scanf("%s", gameIdToDisplayBoard);

	char oPlayerName[BUFFER_SIZE];
	char xPlayerName[BUFFER_SIZE];
	char boardPlaceHolder[BOARD_HEIGHT][BOARD_WIDTH];

	const Bool gameExists = checkGameIdExists(gameIdToDisplayBoard);
	if (!gameExists) {
		printf("Game with ID: %s doesn't exist. Please, enter a valid ID.\n", gameIdToDisplayBoard);
		return;
	}

	getGameInfo(gameIdToDisplayBoard, xPlayerName, oPlayerName, NULL, NULL, boardPlaceHolder);

	printf("%s %s\n", xPlayerName, oPlayerName);
	displayBoard(boardPlaceHolder, CONTINUE, FALSE, stdout);

}


int loadGameById(void) {
	/*
	 * Loads to a game to memory: players' names and the board.
	 * Returns turn of a current player.
	 * If there is no game with such ID, returns error code "INVALID_GAME_ID" (-9).
	 */

	printf(SEPARATOR_LINE);
	char gameIdToLoad[BUFFER_SIZE];

	printf("Enter the ID of the game:\n>>");
	scanf("%s", gameIdToLoad);

	const Bool gameExists = checkGameIdExists(gameIdToLoad);
	if (!gameExists) {
		printf("Game with ID: %s doesn't exist. Please, enter a valid ID.\n", gameIdToLoad);
		return INVALID_GAME_ID;
	}

	char turn[BUFFER_SIZE];
	getGameInfo(gameIdToLoad, playersNames[0], playersNames[1], NULL, turn,
		mainBoard);

	const int turnNumber = atoi(turn);
	return turnNumber;
}

Turn loadGameOption(void) {
	/*
	 * Asks a user to enter an option number.
	 * Returns a current turn in the loaded game.
	 */

	Turn turn = DEFAULT;
	Bool loadOptionChosen = TRUE;
	while (loadOptionChosen) {
		displayLoadGameMenu();
		const LoadGameOption inputOption = getInputInBoundaries("Enter a valid option (1-5):\n>>", 1,
			5);

		switch (inputOption) {
			case LIST_ALL_SAVED_GAMES:
				listAllSavedGames(FALSE);
				break;
			case LIST_ALL_SAVED_GAMES_PLAYER:
				listAllSavedGames(TRUE);
				break;
			case SHOW_THE_BOARD_OF_SAVED_GAME:
				showTheBoardOfOneSavedGame();
				break;
			case LOAD_SAVED_GAME_BY_ID:
				turn = loadGameById();
				if (turn != INVALID_GAME_ID) {
					loadOptionChosen = FALSE;
				}
				break;
			case RETURN_TO_MAIN_MENU:
			default:
				return DEFAULT;
		}
	}

	return turn;
}

int exitGameOption(void) {
	printf("Thank you for playing! See you next time.\n");
	return TRUE;
}

int main(void) {
	char pause = 'a';
	Bool playAgain = FALSE;
	Turn loadedTurn = DEFAULT;
	Bool isExit = FALSE;

	while (!isExit) {
		 // Main menu looP

		// checks if players desire to play again
		if (playAgain) {
			playAgain = playGameOption(GAME_IS_NOT_LOADED, DEFAULT);
			continue;
		}

		// checks if players desire to continue some saved game
		if (loadedTurn != DEFAULT) {
			playAgain = playGameOption(GAME_IS_LOADED, loadedTurn);
			loadedTurn = DEFAULT;
			continue;
		}
		/*
		 * These conditions override usual behaviour of the programme, so that it should not show the main menu and ask
		 * for an input for it. Therefore, handling of the conditions was separated and moved before the main if-else
		 * statement.
		 */

		/*
		 *
		 */
		displayMainMenu();
		const MainMenuOption inputOption = getInputInBoundaries("Enter a valid option (1-3):\n>>", 1, 3);
		if (inputOption == PLAY_NEW_GAME) {
			playAgain = playGameOption(GAME_IS_NOT_LOADED, DEFAULT);
		} else if (inputOption == LOAD_SAVED_GAME) {
			loadedTurn = loadGameOption();
		} else if (inputOption == EXIT_GAME) {
			isExit = exitGameOption();
		}
	}

	// to prevent immediate closing of a terminal window after execution of the programme
	scanf("%c", &pause);
	printf("Press any button to close the window...");
	scanf("%c", &pause);
	return EXIT_SUCCESS;
}