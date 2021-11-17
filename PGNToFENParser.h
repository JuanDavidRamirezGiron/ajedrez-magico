#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>
#include <array>

using namespace std;

class PGNToFENParser {

	vector<vector<char>> initialChessBoard;
	vector<vector<char>> internalChessBoard;
	vector<vector<vector<char>>> allChessBoardStatus;

	
private:

	int getIndex(vector<char> v, char K);


public:

	PGNToFENParser() {

	}

	vector<vector<char>> initializeInternalChessBoard();
	void printBoard(vector<vector<char>> board);
	void printAllStatusBoard();
	void parsePGNToFEN(map<int, pair<string, string>> plays);
	vector<vector<char>> getInitialChessBoard();
	vector<vector<char>> getInternalChessBoard();

};