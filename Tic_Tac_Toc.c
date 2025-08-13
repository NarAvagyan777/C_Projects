
#include <stdio.h>
#include <stdbool.h> 

const char xPlayer = 'x';
const char oPlayer = 'o';
const char empty = ' ';

void printBoard(char board[3][3]) {
		printf(" %c | ", board[0][0]); printf("%c | ", board[0][1]); printf("%c\n", board[0][2]);
		printf("---+---+---\n");
		printf(" %c | ", board[1][0]); printf("%c | ", board[1][1]); printf("%c\n", board[1][2]);
		printf("---+---+---\n");
		printf(" %c | ", board[2][0]); printf("%c | ", board[2][1]); printf("%c\n", board[2][2]);
}

bool isLegalMove(int row, int col, char board[3][3]) {
		return row >=0 && row <= 2 && col >= 0 && col <= 2 && board[row][col] == empty;
}

bool didWin(char player, char board[3][3]) {
	
		// check rows

		if (board[0][0] == player && board[0][1] == player && board[0][2] == player) {
				return true;
		}	

		if (board[1][0] == player && board[1][1] == player && board[1][2] == player) {
				return true;
		}	

		if (board[2][0] == player && board[2][1] == player && board[2][2] == player) {
				return true;
		}	
		// check columns

		if (board[0][0] == player && board[1][0] == player && board[2][0] == player) {
				return true;
		}	

		if (board[0][1] == player && board[1][1] == player && board[2][1] == player) {
				return true;
		}

		if (board[0][2] == player && board[1][2] == player && board[2][2] == player) {
				return true;
		}	
		// check diagonal

		if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
				return true;
		}	

		if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
				return true;
		}	
		return false;
}
bool didEnd(char board[3][3]) {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (board[i][j] == empty) {
				return false;
			}
		}
	}
	return true;
}



int main() {

char player = xPlayer;

char board[3][3] = {
	{empty, empty, empty},
	{empty, empty, empty},		
	{empty, empty, empty}
};

int row, col;
	while (true) {
		if (player == xPlayer) {
			printf("X Please enter the row\n");
			scanf("%d", &row);
			printf("X Please enter the column\n");
			scanf("%d", &col);
		}
		if (player == oPlayer) {
			printf("O Please enter the row\n");
			scanf("%d", &row);
			printf("O Please enter the column\n");
			scanf("%d", &col);
		}

		if (isLegalMove(row, col , board) == false) {
			printf("Not a legal move\n");
			continue;
		}

		board[row][col] = player;
		printBoard(board);

		if (didWin(player, board)) {
			if (player == xPlayer) {
				printf("X player WIN!!!");
				break;
			}
			if (player == oPlayer) {
				printf("O player WIN!!!");
				break;
			}
		}
		
		if (didEnd(board)) {
			printf("The game is over with draw, thank you for choosing our game\n");
			break;
		}

		if (player == xPlayer) {
			player = oPlayer;
		} else {
			player = xPlayer;	
		}
	}
}
