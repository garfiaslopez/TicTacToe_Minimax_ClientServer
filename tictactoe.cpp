#include <iostream>
#include <cstdio>
#include <string.h> 
#include "client.h"

using namespace std;

int requestGameMode() {
    int response = -1;
    while ((response < 1) || (response > 4)) {
        cout << "Selecciona modo de juego: \n";
        cout << "1.- Crear partida \n";
        cout << "2.- Unirse a partida \n";
        cout << "3.- Jugar contra la maquina \n";
        cout << "4.- Demostración \n";
        cin >> response;
        if (response < 1 || response > 4) {
            cout << "Selección no valida \n";
        }
    }
    return response;
}

char askRestartGame() {
    char response = 'n';
    cout << "Reiniciar ? [y/n] \n";
    cin >> response;
    return response;
}

void printBoard(string board) {
    cout << " --- --- --- " <<endl;
    cout << "| " << (board[0] == '_' ? '1' : board[0]) << " | " << (board[1] == '_' ? '2' : board[1]) << " | " << (board[2] == '_' ? '3' : board[2]) << " |" << endl;
    cout << "| " << (board[3] == '_' ? '4' : board[3]) << " | " << (board[4] == '_' ? '5' : board[4]) << " | " << (board[5] == '_' ? '6' : board[5]) << " |" << endl;
    cout << "| " << (board[6] == '_' ? '7' : board[6]) << " | " << (board[7] == '_' ? '8' : board[7]) << " | " << (board[8] == '_' ? '9' : board[8]) << " |" << endl;
    cout << " --- --- --- " <<endl;
}

string pointToCoordinate (int p) {
    if (p == 1) {
        return "00";
    }
    if (p == 2) {
        return "01";
    }
    if (p == 3) {
        return "02";
    }
    if (p == 4) {
        return "10";
    }
    if (p == 5) {
        return "11";
    }
    if (p == 6) {
        return "12";
    }
    if (p == 7) {
        return "20";
    }
    if (p == 8) {
        return "21";
    }
    if (p == 9) {
        return "22";
    }
}

void createNewGame() {
    string url = "127.0.0.1";
    int initialized = initClient(url);
    string data = "S";

    sendToServer(data);
    printf("Iniciando session...\n");

    int valRead;
    char buffer[MAX_BUFFER] = {0};
    while(valRead = readFromServer() <0) {};
    getValue(buffer);
    printf("GAME ID: %s Compartir con contrincante.\n", buffer);
    printf("Esperando que se una...\n", buffer);
    
    string gameId(buffer);

    bool notFinished = true;
    while(notFinished) {
        while(valRead = readFromServer() < 0) {};
        getValue(buffer);
        if (buffer[9]) {
            if (buffer[9] != 'i') {
                if (buffer[9] == 'x') {
                    cout<<"FELICIDADES, GANASTE!" << endl;
                } else if (buffer[9] == 'o') {
                    cout<<"OH NO :( PERDISTE. " << endl;
                } else if (buffer[9] == 'd') {
                    cout<<"EMPATE :O" << endl;
                }
                notFinished = false;
            } else {
                cout<<"Posición no valida, intenta de nuevo" << endl;
            }
        }
        if (notFinished) {
            int shoot = 0;
            cout << "Donde deseas tirar? [1-9]"<<endl;
            string board(buffer);
            printBoard(board);
            cin >> shoot;
            string coordinate = pointToCoordinate(shoot);
            sendToServer("M" + gameId + coordinate + "x");
        }
        
    }
}

void joinGame() {
    string url = "127.0.0.1";
    int initialized = initClient(url);
    
    cout << "¿Cual es tu GameId?\n";
    string gameId;
    cin>>gameId;
    sendToServer("J" + gameId);
    printf("Esperando jugada de jugador 1...\n");
    
    int valRead;
    char buffer[MAX_BUFFER] = {0};

    bool notFinished = true;
    while(notFinished) {
        while(valRead = readFromServer() <0) {};
        getValue(buffer);
        if (buffer[9]) {
            if (buffer[9] != 'i') {
                if (buffer[9] == 'x') {
                    cout<<"OH NO :( PERDISTE. " << endl;
                } else if (buffer[9] == 'o') {
                    cout<<"FELICIDADES, GANASTE!" << endl;
                } else if (buffer[9] == 'd') {
                    cout<<"EMPATE :O" << endl;
                }
                notFinished = false;
            } else {
                cout<<"Posición no valida, intenta de nuevo" << endl;
            }
        }
        if (notFinished) {
            int shoot = 0;
            cout << "Donde deseas tirar? [1-9]"<<endl;
            string board(buffer);
            printBoard(board);
            cin >> shoot;
            string coordinate = pointToCoordinate(shoot);
            sendToServer("M" + gameId + coordinate + "o");
        }
    }

}

void playVersusMachine() {
string url = "127.0.0.1";
    int initialized = initClient(url);
    string data = "V";

    sendToServer(data);
    printf("Iniciando session...\n");

    int valRead;
    char buffer[MAX_BUFFER] = {0};
    while(valRead = readFromServer() <0) {};
    getValue(buffer);
    printf("GAME ID: %s.\n", buffer);
    
    string gameId(buffer);

    bool notFinished = true;
    while(notFinished) {
        while(valRead = readFromServer() < 0) {};
        getValue(buffer);
        if (buffer[9]) {
            if (buffer[9] != 'i') {
                if (buffer[9] == 'x') {
                    cout<<"FELICIDADES, GANASTE!" << endl;
                } else if (buffer[9] == 'o') {
                    cout<<"OH NO :( PERDISTE. " << endl;
                } else if (buffer[9] == 'd') {
                    cout<<"EMPATE :O" << endl;
                }
                notFinished = false;
            } else {
                cout<<"Posición no valida, intenta de nuevo" << endl;
            }
        }
        if (notFinished) {
            int shoot = 0;
            cout << "Donde deseas tirar? [1-9]"<<endl;
            string board(buffer);
            printBoard(board);
            cin >> shoot;
            string coordinate = pointToCoordinate(shoot);
            sendToServer("I" + gameId + coordinate + "x");
        }
        
    }
}

void demo() {

}

int main () {
    bool continueGame = true;
    while (continueGame) {
        int gameMode = 0; // 0 -> start new game (X-firstShot), 1 -> join game, 2 -> auto
        gameMode = requestGameMode();
        if (gameMode == 1) {
            createNewGame();
        } else if (gameMode == 2) {
            joinGame();
        } else if (gameMode == 3) {
            playVersusMachine();
        } else if (gameMode == 4) {
            demo();
        }
        char restart = askRestartGame();
        if (restart == 'n' || restart == 'N') {
            continueGame = false;
        }
    }
    
}