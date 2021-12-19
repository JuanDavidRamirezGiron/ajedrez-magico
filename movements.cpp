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
	vector<vector<char>> matrizFinal = allBoardStatus[position+1];
	vector<glm::vec3> puntosControl;
	float primerpieceX = 0;
	float primerpieceZ = 0;
	float segundopieceX = 0;
	float segundopieceZ = 0;
	float copiaprimerpieceX = 0;
	float copiaprimerpieceZ = 0;
	int i = 0;
	int j = 0;

	/*while ((iguals || iguals1) && i < matrizJugada.size()) {
		j = 0;
		while ((iguals1 || iguals) && j < matrizJugada[i].size())
		{
			if (matrizJugada[i][j] != matrizFinal[i][j])
			{
				if (iguals) {
					iguals = false;
					segundopieceX = originX + (i + 1) * cellSize;
					segundopieceZ = originZ + (j + 1) * cellSize;
				}
				else {
					iguals1 = false;
					primerpieceX = originX + (i + 1) * cellSize;
					primerpieceZ = originZ + (j + 1) * cellSize;
				}
			}

			j++;

		}
		i++;

	}
	
	*/


	for (int i = 0; i < matrizJugada.size(); i++) {
		for (int j = 0; j < matrizJugada[i].size(); j++) {
			if (matrizJugada[i][j] != matrizFinal[i][j])
			{
				if (iguals) {
					iguals = false;
					primerpieceX = i;
					primerpieceZ = j;
					i = 0;
				}
				else if(primerpieceX != i || primerpieceZ != j){
					segundopieceX = i;
					segundopieceZ = j;
				}
			}
		}
	}

	if (matrizFinal[primerpieceX][primerpieceZ] == '1')
	{
		primerpieceX = originX + (primerpieceX + 1) * cellSize;
		primerpieceZ = originZ + (primerpieceZ + 1) * cellSize;
		segundopieceX = originX + (segundopieceX + 1)*cellSize;
		segundopieceZ = originZ + (segundopieceZ + 1)*cellSize;
	}
	else if(matrizFinal[primerpieceX][primerpieceZ] != '1')
	{
		copiaprimerpieceX = primerpieceX;
		copiaprimerpieceZ = primerpieceZ;
		primerpieceX = originX + (segundopieceX + 1)*cellSize;
		primerpieceZ = originZ + (segundopieceZ + 1)*cellSize;
		segundopieceX = originX + (copiaprimerpieceX + 1)*cellSize;
		segundopieceZ = originZ + (copiaprimerpieceZ + 1)*cellSize;
	}

	float distanceX = segundopieceX - primerpieceX;
	float distanceZ = segundopieceZ;
	float distanceZ1 = distanceZ;
	float distanceZ2 = distanceZ;
	if (segundopieceZ != primerpieceZ)
	{
		float distanceZ = segundopieceZ - primerpieceZ;
		float distanceZ1 = distanceZ * 1.0f / 3;
		float distanceZ2 = distanceZ * 2.0f / 3;
	}
	puntosControl.push_back({ primerpieceX, 0, primerpieceZ });
	puntosControl.push_back({ primerpieceX + (distanceX * 1.0f / 3), 3, distanceZ1 });
	puntosControl.push_back({ primerpieceX + (distanceX * 2.0f / 3), 3, distanceZ2 });
	puntosControl.push_back({ segundopieceX, 0, segundopieceZ });


	

	
	
	/*if (position % 2 != 0)
	{
		float distanceX = segundopieceX - primerpieceX;
		float distanceZ = segundopieceZ;
		float distanceZ1 = distanceZ;
		float distanceZ2 = distanceZ;
		if (segundopieceZ != primerpieceZ)
		{
			float distanceZ = segundopieceZ - primerpieceZ;
			float distanceZ1 = distanceZ * 1.0f / 3;
			float distanceZ2 = distanceZ * 2.0f / 3;
		}
		puntosControl.push_back({ primerpieceX, 0, primerpieceZ });
		puntosControl.push_back({ primerpieceX + (distanceX * 1.0f / 3), 3, distanceZ1 });
		puntosControl.push_back({ primerpieceX + (distanceX * 2.0f / 3), 3, distanceZ2 });
		puntosControl.push_back({ segundopieceX, 0, segundopieceZ });
	}
	else {
		float distanceX = primerpieceX - segundopieceX;
		float distanceZ = segundopieceZ;
		float distanceZ1 = distanceZ;
		float distanceZ2 = distanceZ;
		if (segundopieceZ != primerpieceZ)
		{
			float distanceZ = segundopieceZ - primerpieceZ;
			float distanceZ1 = distanceZ * 1.0f / 3;
			float distanceZ2 = distanceZ * 2.0f / 3;
		}
		puntosControl.push_back({ segundopieceX, 0, segundopieceZ }  );
		puntosControl.push_back({ segundopieceX + (distanceX * 1.0f / 3), 3, distanceZ1});
		puntosControl.push_back({ segundopieceX + (distanceX * 2.0f / 3), 3, distanceZ2});
		puntosControl.push_back({ primerpieceX, 0, primerpieceZ });
	}*/

	return puntosControl;
}