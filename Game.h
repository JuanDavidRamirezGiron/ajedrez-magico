#pragma once
#include"Game.h"
#include"Player.h"
#include"Chessboard.h"

class Game {
public:

	Game();

private:

	Tauler tauler;
	char torn;
	Jugador jugadorBlanques;
	Jugador jugadorNegres;

};