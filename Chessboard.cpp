<<<<<<< HEAD
#include "Chessboard.h"

Chessboard::Chessboard(int rows, int columns)
{
	this->nRows = rows;
	this->nColumns = columns;
	
	for (int i = 0; i < this.nRows; i++) {
		for (int j = 0; j < this.nRows; j++) {
			this.chessboard[i][j] = new Square();
		}
	}
}
=======
#include "Chessboard.h"

Chessboard::Chessboard(int rows, int columns)
{
	this->nRows = rows;
	this->nColumns = columns;
	
	for (int i = 0; i < this.nRows; i++) {
		for (int j = 0; j < this.nRows; j++) {
			this.chessboard[i][j] = new Square();
		}
	}
}
>>>>>>> origin/dev-G
