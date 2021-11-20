#include "PGNFileReader.h"

map<int, pair<string, string>> PGNFileReader::gatherInformation()
{
	//string information;

	map<int, pair<string, string>> dict;
	
	vector<string> origin;
	vector<string> destiny;


	ifstream file(this->filename);
	if (file.is_open()) {
		string line;
		int i = 0;
		while (getline(file, line)) {
			
			if (line[0] == '[') {
	
				//cout << u8"La línea leída es: " << line << endl;
				
				//Quita la primera palabra del string
				line = line.substr(line.find_first_of(" \t"));

				getGeneralinformation(i, line);

				
			}
			else if (line[0] == '\n') {
				continue;
			}
			else{

				if (line[0] == '1' and line[1] == '.') {
					cout << "Empiezan las jugadas" << endl;
				}

				getPlays(origin, destiny, line);

			}
			
			i++;
		}

		if (validatePlays(origin, destiny)) {
			cout << "Jugadas validadas" << endl;
			dict = createDictionary(origin, destiny);
			printDictionary(dict);
		}

		file.close();
	} 
	
	return dict;
}

string PGNFileReader::removeBrackets(string line)
{

	if (line.length() > 2) {
		line = line.substr(1, line.size() - 2);
	}

	//cout << u8"La línea modificada es: " << line << endl;

	return line;
}

string PGNFileReader::reorderPlayerNames(string name)
{

	vector<string> positions;
	
	string delimiter = " ";
	size_t pos = 0;
	string token;
	while ((pos = name.find(delimiter)) != string::npos) {
		token = name.substr(0, pos);
		positions.push_back(token);
		name.erase(0, pos + delimiter.length());
	}

	positions[0].pop_back();

	return name + " " + positions[0];
}

void PGNFileReader::getGeneralinformation(int i, string line)
{

	switch (i) {

	case 0:
		line = this->removeBrackets(line);
		cout << u8"Evento: " << line << endl;
		break;
	case 1:
		line = this->removeBrackets(line);
		cout << u8"Lugar: " << line << endl;
		break;
	case 2:
		line = this->removeBrackets(line);
		cout << u8"Fecha: " << line << endl;
		break;
	case 4:
		line = this->removeBrackets(line);
		line = reorderPlayerNames(line);
		cout << u8"Blancas: " << line << endl;
		break;
	case 5:
		line = this->removeBrackets(line);
		line = reorderPlayerNames(line);
		cout << u8"Negras: " << line << endl;
		break;
	}

}

void PGNFileReader::getPlays(vector<string> &origin, vector<string> &destiny, string line)
{

	string delimiter = " ";
	size_t pos = 0;
	string token;
	int j = 0;


	while ((pos = line.find(delimiter)) != string::npos) {

		token = line.substr(0, pos);
		//cout << token << endl;
		string test = token.erase(0, token.find(".") + 1);
		//cout << "TETSING: " << test << endl;
		if (j % 2 == 0 && token != "") {
			origin.push_back(token);
		}
		else if (token != "") {
			destiny.push_back(token);
		}
		line.erase(0, pos + delimiter.length());
		j++;
	}

	if (line != "") {
		destiny.push_back(line);
	}

}

bool PGNFileReader::validatePlays(vector<string> &origin, vector<string> &destiny)
{
	
	if (destiny.size() > origin.size()) {

		destiny.pop_back();

	}
	else if (!destiny.empty() && destiny.back().find('-')) {

		cout << "Acaba con el resultado" << endl;
		destiny.back() = 'X';

	}

	return origin.size() == destiny.size();
}

template<typename Map>
void PGNFileReader::printDictionary(Map& m)
{

	for (map<int, pair<string, string> >::const_iterator it = m.begin(); it != m.end(); ++it)
		cout << it->first << ": " << "<" << it->second.first << ", " << it->second.second << ">" << endl;

}

map<int, pair<string, string>> PGNFileReader::createDictionary(vector<string> &origin, vector<string> &destiny)
{
	map<int, pair<string, string>> dict;

	for (int i = 0; i < origin.size(); i++)
		dict[i] = { origin[i], destiny[i] };

	return dict;
}
