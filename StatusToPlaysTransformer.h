#pragma once

#include <vector>
#include <ctype.h>

using namespace std;

class StatusToPlaysTransformer {

	enum pieceType
	{
		pawn = 1,
		bishop = 2,
		tower = 3,
		knight = 4,
		queen = 5,
		king = 6

	};

	enum pieceColor {
		white = true,
		black = false,
	};
	
private:
	
	vector<vector<vector<int>>> allPlays;

	vector<int> createPiece(int pieceType, int pieceColor, int row, int col);
	
public:

	StatusToPlaysTransformer() {}

	vector<vector<vector<int>>> transformStatusToPlays(vector<vector<vector<char>>> allBoardStatus);



};
