/* Game.h
Ultimate Tic Tac Toe AI project
Andrew Bergman
11/8/23

This file will set up game logic for running a game of ultimate Tic Tac Toe.

*/
#pragma once
#include "State.h"

class controller
{
public:
    move virtual playMove(Ultimate3TState gameState) = 0;
};


class Game
{
private:
    Ultimate3TState gameState_;
    controller* playerX_;
    controller* playerO_;
public:
    Game(/* args */);
    ~Game();
};