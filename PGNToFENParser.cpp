#include "PGNToFENParser.h"
#include <cctype>
#include <iostream>
#include <cstring>

vector<vector<char>> PGNToFENParser::initializeInternalChessBoard()
{
	string fen = "rnbqkbnr/pppppppp/11111111/11111111/11111111/11111111/PPPPPPPP/RNBQKBNR";

	this->initialChessBoard =
	{
		//Blancas en mayúsculas
		//Negras en minúsculas
		//1 = casilla vacía
		{'R','N','B','Q','K','B','N','R'},
		{'P','P','P','P','P','P','P','P'},
		{'1','1','1','1','1','1','1','1'},
		{'1','1','1','1','1','1','1','1'},
		{'1','1','1','1','1','1','1','1'},
		{'1','1','1','1','1','1','1','1'},
		{'p','p','p','p','p','p','p','p'},
		{'r','n','b','q','k','b','n','r'},

	};

	//printBoard(this->initialChessBoard);

	this->internalChessBoard = this->initialChessBoard;

	return this->initialChessBoard;
}

void PGNToFENParser::printBoard(vector<vector<char>> board)
{
	//El tablero se muestra a la inversa
	//Se va desde 7 a 0 ya que empiezan negras
	int i = 7;

	cout << "    A " << "B " << "C " << "D " << "E " << "F " << "G " << "H " << endl;

	do {

		cout << i+1 <<" [ ";
		for (size_t j = 0; j < 8; j++)
		{
			cout << board[i][j] << " ";
		}

		cout << "]" << endl;

		i--;

	} while (i>=0);
}

void PGNToFENParser::printAllStatusBoard()
{

	for (int i = 0; i < this->allChessBoardStatus.size(); i++)
	{
		cout << "----Mostrando el status de las jugadas " << i << "----" << endl;

		printBoard(this->allChessBoardStatus[i]);
	}

}

vector<vector<char>> PGNToFENParser::getInitialChessBoard()
{
	return this->initialChessBoard;
}

vector<vector<char>> PGNToFENParser::getInternalChessBoard()
{
	return this->internalChessBoard;
}

void PGNToFENParser::parsePGNToFEN(map<int, pair<string, string>> plays)
{

	//cout << plays[0].first[0] << endl;

	vector<char> letras{'a','b','c','d','e','f','g','h'};
	int col;

	for (int i = 0; i < plays.size(); i++)
	{
		string whitePlay = plays[i].first;
		string blackPlay = plays[i].second;

		//jugada blancas
		
		switch (whitePlay[0]) {
			case 'R':
				break;
			case 'N':
				break;
			case 'B':
				break;
			case 'Q':
				break;
			case 'K':
				break;
			case 'P':
				break;
			default:

				col = getIndex(letras, tolower(whitePlay[0]));

				//Es una casilla del tablero, por tanto un peón
				if (col != -1) {

					//col = getIndex(letras, tolower(whitePlay[0]));
					//int lastPosition;

					for (int row = 0; row < 8; row++)
					{
						if (this->internalChessBoard[row][col] == 'P') {
							//lastPosition = j;
							this->internalChessBoard[row][col] = '1';
							break;
						}
					}

					int row = whitePlay[1] - '0';
					row--;
					this->internalChessBoard[row][col] = 'P';
					//printBoard(this->internalChessBoard);
					//this->allChessBoardStatus.push_back(this->internalChessBoard);

					//printAllStatusBoard();

				}
				
				else {
					cout << "No es jugada normal" << endl;
				}

				break;
		}

		//Jugadas negras
		switch (blackPlay[0]) {
		case 'R':
			break;
		case 'N':
			break;
		case 'B':
			break;
		case 'Q':
			break;
		case 'K':
			break;
		case 'P':
			break;
		default:

			col = getIndex(letras, tolower(blackPlay[0]));

			//Es una casilla del tablero, por tanto un peón
			if (col != -1) {

				//col = getIndex(letras, tolower(whitePlay[0]));
				//int lastPosition;

				for (int row = this->internalChessBoard.size() - 1; row >= 0; row--) {
					if (this->internalChessBoard[row][col] == 'p') {
						//lastPosition = j;
						this->internalChessBoard[row][col] = '1';
						break;
					}
				}

				int row = blackPlay[1] - '0';
				row--;
				this->internalChessBoard[row][col] = 'p';
				printBoard(this->internalChessBoard);
				//this->allChessBoardStatus.push_back(this->internalChessBoard);

				//printAllStatusBoard();

			}

			else {
				cout << "No es jugada normal" << endl;
				printAllStatusBoard();
			}

			break;
		}

		this->allChessBoardStatus.push_back(this->internalChessBoard);
		printAllStatusBoard();
	}


	printAllStatusBoard();

}



int PGNToFENParser::getIndex(vector<char> v, char K)
{
	//auto it = find(v.begin(), v.end(), K);
	//int index;

	auto it = find(v.begin(), v.end(), K);

	if (it != v.end()) {

		return distance(v.begin(), it);

	}
	else {
		return -1;
	}
	
}