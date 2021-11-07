#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"
#include<iostream>
#include<string>
using namespace std;

class Player {
public:

	Player(string color);

private:
	string color;
	Peça aPeces[16];

};