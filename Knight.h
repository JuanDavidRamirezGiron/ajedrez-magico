<<<<<<< HEAD
#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"
#include<iostream>
#include<string>
#include<pair>
using namespace std;

class Knight : public Piece {
public:

	Knight(string name, string color) :Piece(name, color, this.nMoviments)

private:
	int nMoviments = 1;

=======
#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"
#include<iostream>
#include<string>
#include<pair>
using namespace std;

class Knight : public Piece {
public:

	Knight(string name, string color) :Piece(name, color, this.nMoviments)

private:
	int nMoviments = 1;

>>>>>>> origin/dev-G
};