#include <stdio.h>  
#include <string>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <map>
#include "tictactoesolver.h"


#define TRUE   1
#define FALSE  0
#define PORT 8888

using namespace std;

struct Game {
    int player_x = -1;
    int player_o = -1;
    char board[3][3] = { 
        { '_', '_', '_' }, 
        { '_', '_', '_' }, 
        { '_', '_', '_' } 
    };
    int status = 0;
    char turn = '-';
    char winner = '-';
};

map<string, Game> GamesMap;
int actualGameId = 0;

string generateGameId() {
    string id = "000";
    int newGameId = actualGameId + 1;
    string newGameIdStr = to_string(newGameId);
    int newGameIdLen = newGameIdStr.size();
    if (newGameIdLen == 1) {
        id[2] = newGameIdStr[0];
    } else if (newGameIdLen == 2) {
        id[2] = newGameIdStr[0];
        id[1] = newGameIdStr[1];
    } else if (newGameIdLen == 3) {
        id[2] = newGameIdStr[0];
        id[1] = newGameIdStr[1];
        id[0] = newGameIdStr[2];
    }
    actualGameId = newGameId;
    return id;
}

string boardToString(char board[3][3]) {
    printf("boardToString init");
    string newBoard = "";
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            newBoard += board[i][j];
        }
    }
    printf("boardToString %s", newBoard.c_str());
    return newBoard;
}

int charToInt(char x) {
    return (int)x - 48;
}

int main(int argc , char *argv[]) {
    int opt = TRUE;   
    int master_socket, addrlen, new_socket, client_socket[30], max_clients = 30, activity, i, valread , sd, max_sd;
    struct sockaddr_in address;
    char buffer[1025];
    fd_set readfds;
    char *message = "Conectado al servidor exitosamente \r\n";   
    for (i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }

    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {   
        printf("socket failed");   
        exit(EXIT_FAILURE);   
    }   
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {
        printf("setsockopt");
        exit(EXIT_FAILURE);   
    }
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( PORT );

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {   
        printf("bind failed");   
        exit(EXIT_FAILURE);   
    }

    printf("Listener on port %d \n", PORT);   
    if (listen(master_socket, 4) < 0) {   
        printf("listen");   
        exit(EXIT_FAILURE);   
    }
    addrlen = sizeof(address);

    printf("Waiting for connections ...");   
    while(TRUE) {   
        FD_ZERO(&readfds);   
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   

        for (i=0; i<max_clients; i++) {   
            sd = client_socket[i];   
            if(sd > 0) {
                FD_SET( sd , &readfds);
            }
            if(sd > max_sd) {
                max_sd = sd;
            }
        }
        // waits for activity on socket
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno!=EINTR)) {   
            printf("select error");   
        }
        if (FD_ISSET(master_socket, &readfds)) {   
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) <0) {
                printf("accept");
                exit(EXIT_FAILURE);
            }
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs (address.sin_port));
            for (i=0; i<max_clients; i++) {
                if(client_socket[i] == 0 ) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];
            if (FD_ISSET( sd , &readfds)) {
                if ((valread = read(sd , buffer, 1025)) == 0) {
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    close(sd);
                    client_socket[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    printf("BUFFER RECEIVED: %s \n", buffer);
                    
                    char action = buffer[0];
                    printf("ACTION: %c \n", action);

                    if (action == 'S') {
                        string gameId = generateGameId();
                        printf("GAME ID GENERATED: %s \n", gameId.c_str());
                        send(sd , gameId.c_str(), gameId.size() , 0);
                        Game newGame;
                        newGame.player_x = sd;
                        newGame.turn = 'x';
                        GamesMap[gameId] = newGame;
                    }

                    if (action == 'J') {
                        string gameId = "";
                        for (int a = 1; a<=3; a++) {
                            gameId += buffer[a];
                        }
                        printf("GAMEID: %s\n", gameId.c_str());

                        if (GamesMap[gameId].player_x) {
                            printf("Session encontrada\n");
                            Game actualGame = GamesMap[gameId];
                            actualGame.player_o = sd;
                            if(actualGame.status != 2) {
                                string board = boardToString(actualGame.board);
                                send(actualGame.player_x , board.c_str(), board.size() , 0);
                                GamesMap[gameId] = actualGame;
                            } else {
                                printf("Partida terminada, ganador: ");
                            }
                        } else {
                            printf("no existe esa session\n");
                        }
                    }

                    if (action == 'M') {
                        string gameId = "";
                        for (int a = 1; a<=3; a++) {
                            gameId += buffer[a];
                        }
                        printf("GAMEID: %s\n", gameId.c_str());
                        if (GamesMap[gameId].player_x) {
                            printf("Session encontrada\n");
                            Game actualGame = GamesMap[gameId];
                            int x = charToInt(buffer[4]);
                            int y = charToInt(buffer[5]);

                            if (actualGame.board[x][y] == '_') {
                                actualGame.board[x][y] = buffer[6];
                                actualGame.turn = buffer[6];
                                string board = boardToString(actualGame.board);
                                int result = evaluate(actualGame.board);
                                bool movesLeft = isMovesLeft(actualGame.board);

                                if (buffer[6] == 'x') {
                                    if (result == 10) {
                                        board += "x";
                                        send(actualGame.player_x , board.c_str(), board.size() , 0);
                                        send(actualGame.player_o , board.c_str(), board.size() , 0);
                                    } else {
                                        if (result == 0 && movesLeft == 0) {
                                            board += "d";
                                            send(actualGame.player_x , board.c_str(), board.size() , 0);
                                            send(actualGame.player_o , board.c_str(), board.size() , 0);
                                        } else {
                                            send(actualGame.player_o , board.c_str(), board.size() , 0);
                                        }
                                    }
                                } else {
                                    if (result == -10) {
                                        board += "o";
                                        send(actualGame.player_x , board.c_str(), board.size() , 0);
                                        send(actualGame.player_o , board.c_str(), board.size() , 0);
                                    } else {
                                        if (result == 0 && movesLeft == 0) {
                                            board += "d";
                                            send(actualGame.player_x , board.c_str(), board.size() , 0);
                                            send(actualGame.player_o , board.c_str(), board.size() , 0);
                                        } else {
                                            send(actualGame.player_x , board.c_str(), board.size() , 0);
                                        }
                                    }
                                }
                            } else {
                                string board = boardToString(actualGame.board);
                                board += "i";
                                if (buffer[6] == 'x') {
                                    send(actualGame.player_x , board.c_str(), board.size() , 0);
                                } else {
                                    send(actualGame.player_o , board.c_str(), board.size() , 0);
                                }
                            }
                            
                            GamesMap[gameId] = actualGame;
                        }
                    }

                    if (action == 'V') {
                        string gameId = generateGameId();
                        printf("GAME ID GENERATED: %s \n", gameId.c_str());
                        send(sd , gameId.c_str(), gameId.size() , 0);
                        Game newGame;
                        newGame.player_x = sd;
                        newGame.turn = 'x';
                        GamesMap[gameId] = newGame;
                        string board = boardToString(newGame.board);
                        send(newGame.player_x , board.c_str(), board.size() , 0);
                    }


                    if (action == 'I') {
                        string gameId = "";
                        for (int a = 1; a<=3; a++) {
                            gameId += buffer[a];
                        }
                        printf("GAMEID: %s\n", gameId.c_str());
                        if (GamesMap[gameId].player_x) {
                            printf("Session encontrada\n");
                            Game actualGame = GamesMap[gameId];
                            int x = charToInt(buffer[4]);
                            int y = charToInt(buffer[5]);

                            if (actualGame.board[x][y] == '_') {
                                actualGame.board[x][y] = buffer[6];
                                actualGame.turn = buffer[6];

                                bool movesLeft = isMovesLeft(actualGame.board);
                                printf("MOVESLEFT: %d \n", movesLeft);

                                if (movesLeft != 0) {
                                    // CALC BEST SHOOT FROM PC:
                                    string pc_shoot = findBestMove(actualGame.board, 'o', 'x');
                                    int a = charToInt(pc_shoot[0]);
                                    int b = charToInt(pc_shoot[1]);
                                    actualGame.board[a][b] = 'o';
                                }
                                
                                string board = boardToString(actualGame.board);
                                int result = evaluate(actualGame.board);
                                if (result == -10) {
                                    board += "x";
                                    send(actualGame.player_x , board.c_str(), board.size() , 0);
                                } else if (result == 10) {
                                    board += "o";
                                    send(actualGame.player_x , board.c_str(), board.size() , 0);
                                } else if (result == 0 && movesLeft == 0) {
                                    board += "d";
                                    send(actualGame.player_x , board.c_str(), board.size() , 0);
                                } else {
                                    send(actualGame.player_x , board.c_str(), board.size() , 0);
                                }
                            } else {
                                string board = boardToString(actualGame.board);
                                board += "i";
                                send(actualGame.player_x , board.c_str(), board.size() , 0);
                            }

                            GamesMap[gameId] = actualGame;
                        }
                    }

                }
            }
        }
    }

    return 0;   
}  