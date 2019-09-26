#include <string.h>

using namespace std;

struct Move { 
    int row, col; 
};

void stringToBoard(string strBoard, char board[3][3]);

int evaluate(char board[3][3]);
string findBestMove(char board[3][3], char setPlayer, char setOponent);
bool isMovesLeft(char board[3][3]);