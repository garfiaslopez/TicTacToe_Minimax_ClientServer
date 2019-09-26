#include <iostream>
#include <cstdio>
#include <string.h>
#include "tictactoesolver.h"

using namespace std;

char player = 'x', opponent = 'o';

bool isMovesLeft(char board[3][3]) { 
    for (int i = 0; i<3; i++) {
        for (int j = 0; j<3; j++) {
            if (board[i][j]=='_') {
                return true;
            }   
        }
    }
    return false; 
} 

int evaluate(char board[3][3]) {
    // rows
    for (int row = 0; row<3; row++) { 
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == player) {
                return +10;
            } else if (board[row][0] == opponent) {
                return -10; 
            }
        } 
    }
    //columns
    for (int col = 0; col<3; col++) { 
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) { 
            if (board[0][col] == player) {
                return +10;
            } else if (board[0][col] == opponent) {
                return -10;
            }
        } 
    }
    //diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0]==player) {
            return +10;
        } else if (board[0][0] == opponent) {
            return -10;
        }
    } 
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) { 
        if (board[0][2] == player) {
            return +10;
        } else if (board[0][2] == opponent) {
            return -10;
        }
    } 
    return 0; 
} 

int minimax(char board[3][3], int depth, bool isMax) {
    int score = evaluate(board); 
    if (score == 10 || score == -10) {
        return score;
    } else if (isMovesLeft(board) == false) {
        return 0;
    }

    if (isMax) { 
        int best = -1000; 
        for (int i = 0; i<3; i++) { 
            for (int j = 0; j<3; j++) { 
                if (board[i][j]=='_') { 
                    board[i][j] = player;
                    best = max(best, minimax(board, depth+1, !isMax)); 
                    board[i][j] = '_'; 
                } 
            } 
        } 
        return best; 
    } else { 
        int best = 1000; 
        for (int i = 0; i<3; i++) { 
            for (int j = 0; j<3; j++) { 
                if (board[i][j]=='_') { 
                    board[i][j] = opponent;
                    best = min(best, minimax(board, depth+1, !isMax));
                    board[i][j] = '_'; 
                }
            }
        }
        return best; 
    }
}

string findBestMove(char board[3][3], char setPlayer, char setOponent) {

    int bestVal = -1000;
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
    player = setPlayer;
    opponent = setOponent;
  
    for (int i = 0; i<3; i++) { 
        for (int j = 0; j<3; j++) { 
            if (board[i][j]=='_') { 
                board[i][j] = player; 
                int moveVal = minimax(board, 0, false); 
                board[i][j] = '_';
                if (moveVal > bestVal) { 
                    bestMove.row = i; 
                    bestMove.col = j; 
                    bestVal = moveVal; 
                } 
            } 
        } 
    }
    return to_string(bestMove.row) + to_string(bestMove.col); 
} 

void stringToBoard(string strBoard, char board[3][3]) {
    int it = 0;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            board[i][j] = strBoard[it];
            it++;
        }
    }
}


/*
// Driver code 
int main() {
    char board[3][3] = 
    { 
        { 'x', 'o', 'x' }, 
        { '_', 'o', 'x' }, 
        { '_', '_', '_' } 
    };
    int score = evaluate(board);
    if (score == 10) {
        printf("Player won :\n"); 
    } else if (score == -10) {
        printf("Oponent won :\n"); 
    } else {
        Move bestMove = findBestMove(board, 'x', 'o'); 
        printf("The Optimal Move is :\n"); 
        printf("ROW: %d COL: %d\n\n", bestMove.row, bestMove.col ); 
    }
    return 0; 
}

*/