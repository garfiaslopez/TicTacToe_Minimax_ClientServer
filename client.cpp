#include <stdio.h> 
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "client.h"

#define PORT 8888
#define MAX_BUFFER 1025

using namespace std;

int sock = 0, valread;
struct sockaddr_in serv_addr;
char buffer[MAX_BUFFER] = {0};

int readFromServer() {
    //valread = read(sock, buffer, MAX_BUFFER);
    valread = recv(sock , buffer , sizeof(buffer) , 0);
    return valread;
}

void getValue(char bufferClient[MAX_BUFFER] ) {
    for (int i=0; i<MAX_BUFFER;i++) {
        bufferClient[i] = buffer[i];
    }
}

void sendToServer(string data) {
    send(sock , data.c_str(), strlen(data.c_str()) , 0 );
}

int initClient(string serverUrl) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, serverUrl.c_str(), &serv_addr.sin_addr)<=0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    return 1;
}
