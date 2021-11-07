#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"
#include<iostream>
#include<string>
#include<pair>
using namespace std;

class Pawn : public Piece {
public:

	Pawn(string name, string color):Piece(name, color, this.nMoviments)

private:
	int nMoviments = 2;

};