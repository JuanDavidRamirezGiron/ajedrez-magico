#pragma once
#include"Square.h"
#include"Game.h"
#include"Player.h"


class Chessboard {
public:

	Chessboard(int rows, int columns);

private:
	int nRows;
	int nColumns;
	Square [nRows][nColumns] chessboard;

};