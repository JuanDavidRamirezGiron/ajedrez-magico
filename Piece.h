#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"
#include<iostream>
#include<string>
#include<pair>
using namespace std;

class Piece {
public:

	Piece(string name, string color, int nMoviments);
	virtual ~Piece() = 0;

private:
	string name;
	string color;
	int nMoviments;
	bool alive;
	pair<int, int> moviments[nMoviments];
};