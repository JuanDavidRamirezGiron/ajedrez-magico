#include "movements.h"


void Movements::RawBoard(vector<char> rawBoard)
{
	int i = 7;

	do {

		cout << rawBoard[i] << " ";
		i--;

	} while (i >= 0);
	cout << endl;
}

void Movements::printBoard(vector<vector<char>> transformedBoard)
{


	int i = 7;

	do {
		for (size_t j = 0; j < 8; j++)
		{
			cout << transformedBoard[i][j];
		}
		i--;

	} while (i >= 0);


}

vector<glm::vec3> Movements::compareBoards(vector<vector<vector<char>>> allBoardStatus, int position)
{
	const int originX = -9;
	const int originZ = 10;
	const int cellSize = 2;
	bool iguals = true;
	bool iguals1 = true;
	vector<vector<char>> matrizJugada = allBoardStatus[position];
	vector<vector<char>> matrizFinal = allBoardStatus[position + 1];
	vector<glm::vec3> puntosControl;
	int posiniI = 0;
	int posiniJ = 0;
	int posFiI = 0;
	int posFiJ = 0;
	int i = 0;
	int j = 0;
	while ((iguals || iguals1) && i < matrizJugada.size()) {
		j = 0;
		while ((iguals1 || iguals) && j < matrizJugada[i].size())
		{
			if (matrizJugada[i][j] != matrizFinal[i][j])
			{
				if (iguals1) {
					iguals1 = false;
					float pieceX = originX + i * cellSize;
					float pieceZ = originZ - j * cellSize;
					puntosControl.push_back({pieceX, 0, pieceZ});
				}
				else{
					iguals = false;
					int pieceX = originX + i * cellSize;
					int pieceZ = originZ - j * cellSize;
					puntosControl.push_back({ pieceX, 0, pieceZ });
				}
			}
			
			j++;
		}
		i++;

	}
	return puntosControl;
}