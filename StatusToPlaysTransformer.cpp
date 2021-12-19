#include "StatusToPlaysTransformer.h"


vector<vector<vector<int>>> StatusToPlaysTransformer::transformStatusToPlays(vector<vector<vector<char>>> allBoardStatus)
{

	int count = 0;
	
	vector<vector<int>> tempPlayPieces;
	vector<int> tempPiece;
	this->allPlays = {};


	for (vector<vector<char>> boards : allBoardStatus)
	{

		tempPlayPieces.clear();
		tempPiece.clear();

		for (size_t row = 0; row < boards.size(); row++)
		{

			for (size_t col = 0; col < boards[row].size(); col++)
			{
				char currentPiece = boards[row][col];

				if (!isdigit(currentPiece)) {
					//cout << "Count:" << count << endl;
					//cout << "Pieza es: " << boards[row][col] << endl;
					//count++;
					
					pieceType type;
					pieceColor color;

					if (isupper(currentPiece)) {
						
						color = pieceColor::white;

						switch (currentPiece) {

							case 'R':

								type = pieceType::tower;
								break;

							case 'N':
								
								type = pieceType::knight;
								break;

							case 'B':

								type = pieceType::bishop;
								break;

							case 'Q':
								
								type = pieceType::queen;
								break;

							case 'K':
								
								type = pieceType::king;
								break;

							case 'P':
								
								type = pieceType::pawn;
								break;
							}

					}
					else {

						color = pieceColor::black;

						switch (currentPiece) {
							case 'r':

								type = pieceType::tower;
								break;

							case 'n':

								type = pieceType::knight;
								break;

							case 'b':

								type = pieceType::bishop;
								break;

							case 'q':

								type = pieceType::queen;
								break;

							case 'k':

								type = pieceType::king;
								break;

							case 'p':

								type = pieceType::pawn;
								break;

						}

					}

					tempPiece = createPiece(type, color, row, col);
					tempPlayPieces.push_back(tempPiece);

				}

			}

		}

		this->allPlays.push_back(tempPlayPieces);


	}

	return this->allPlays;
}

vector<int> StatusToPlaysTransformer::createPiece(int pieceType, int pieceColor, int row, int col)
{
	return vector<int>{pieceType, pieceColor, row+1, col+1};
}
