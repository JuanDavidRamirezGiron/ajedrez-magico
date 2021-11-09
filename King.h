#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"
#include<iostream>
#include<string>
#include<pair>
using namespace std;

class King : public Piece {
public:

	King(string name, string color) :Piece(name, color, this.nMoviments)

private:
	int nMoviments = 1;

};