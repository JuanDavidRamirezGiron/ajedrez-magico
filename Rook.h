#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"
#include<iostream>
#include<string>
#include<pair>
using namespace std;

class Rook : public Piece {
public:

	Rook(string name, string color) :Piece(name, color, this.nMoviments)

private:
	int nMoviments = 2;

};