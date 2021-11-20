<<<<<<< HEAD
#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"
#include<iostream>
#include<string>
#include<pair>
using namespace std;

class Queen : public Piece {
public:

	Queen(string name, string color) :Piece(name, color, this.nMoviments)

private:
	int nMoviments = 4;

=======
#pragma once
#include"Piece.h"
#include"Player.h"
#include"Chessboard.h"
#include<iostream>
#include<string>
#include<pair>
using namespace std;

class Queen : public Piece {
public:

	Queen(string name, string color) :Piece(name, color, this.nMoviments)

private:
	int nMoviments = 4;

>>>>>>> origin/dev-G
};