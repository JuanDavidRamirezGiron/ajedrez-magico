#include "FENFileReader.h"

void FENFileReader::prepareBoard()
{

	ifstream file(this->filename);

	if (file.is_open()) 
	{
		string line;
		string player;
		int i = 0;
		while (getline(file, line)) 
		{
			
			string plays = retrievePlays(line);

			if (i % 2 != 0) {
				player = "negras";
			}
			else {
				player = "blancas";
			}
			
			
			vector<string> rawBoard = generateRawBoard(plays);
			//printRawBoard(rawBoard);
			vector<vector<char>> transformedBoard = transformRawBoard(rawBoard);
			cout << "Jugada num: " << i+1 << "\n" << "Juega " << player << endl;
			printTransformedBoard(transformedBoard);
			i++;

		}
	}

	else {
		cout << "No se ha encontrado el archivo FEN" << endl;
	}

}


string FENFileReader::retrievePlays(string line)
{

	string delimiter = " ";
	string plays = line.substr(0, line.find(delimiter));

	//cout << plays << endl;

	return plays;
}

vector<string> FENFileReader::generateRawBoard(string plays)
{
	vector<string> rawBoard(8);

	int i = 7;

	for (char carac : plays)
	{

		if (carac != '/') {
			rawBoard[i].push_back(carac);
		}
		else {
			i--;

		}

	}

	return rawBoard;
}

vector<vector<char>> FENFileReader::transformRawBoard(vector<string> rawBoard)
{
	vector<vector<char>> transformedBoard(8);

	for (int i = rawBoard.size() - 1; i >= 0; i--)
	{

		int col = 0;
		for (char carac : rawBoard[i])
		{

			if (isdigit(carac)) {
				//cout << "Es un digito" << endl;
				int emptyCells = carac - '0';
				do {
					transformedBoard[i].push_back('1');
					emptyCells--;
				} while (emptyCells > 0);

				col += emptyCells;
			}
			else {
				transformedBoard[i].push_back(carac);
				//cout << "NO lo es un digito" << endl;
				col++;
			}

		}


	}


	return transformedBoard;
}



void FENFileReader::printRawBoard(vector<string> rawBoard)
{
	int i = 7;

	do {

		cout << rawBoard[i] << endl;
		i--;

	} while (i >= 0);
}

void FENFileReader::printTransformedBoard(vector<vector<char>> transformedBoard)
{


	int i = 7;

	cout << "    A " << "B " << "C " << "D " << "E " << "F " << "G " << "H " << endl;

	do {

		cout << i + 1 << " [ ";
		for (size_t j = 0; j < 8; j++)
		{
			cout << transformedBoard[i][j] << " ";
		}

		cout << "]" << endl;

		i--;

	} while (i >= 0);


}
