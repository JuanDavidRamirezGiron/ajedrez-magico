#include "FENFileReader.h"

void FENFileReader::prepareBoard()
{

	ifstream file(this->filename);

	if (file.is_open()) 
	{
		string line;
		int i = 0;
		while (getline(file, line)) 
		{
			//cout << line << endl;

			string delimiter = " ";
			string boardStatus = line.substr(0, line.find(delimiter));

			cout << boardStatus << endl;


			delimiter = "/";
			vector<string> rows(8);

			
			int i = 7;

			for (char carac: boardStatus)
			{

				if (carac != '/') {
					rows[i].push_back(carac);
				}
				else {
					i--;

				}

			}

			i = 7;

			do {

				cout << rows[i] << endl;
				i--;

			} while (i >= 0);

			


			

		}
	}

	else {
		cout << "No se ha encontrado el archivo FEN" << endl;
	}

}