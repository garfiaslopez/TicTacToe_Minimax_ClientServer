# TicTacToe_Minimax_ClientServer

A tic tac toe socket game implementation, with 2 game modes, 1 vs 1 or 1 vs machine.

The IA from the machine is implemented with minimax algorithm.


### Run Server
```
g++ server.cpp tictactoesolver.cpp -o server && ./server
```

### Run Client
```
g++ tictactoe.cpp client.cpp -o tictactoe && ./tictactoe
```
