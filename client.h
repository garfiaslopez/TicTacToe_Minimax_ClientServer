#include <string.h> 
#define MAX_BUFFER 1025

using namespace std;

int initClient(string serverUrl);
int readFromServer();
void getValue(char bufferClient[MAX_BUFFER]);
void sendToServer(string data);
