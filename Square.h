#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"

class Square {
public:

	Square();

private:
	Piece piece;
	bool occupied;

};