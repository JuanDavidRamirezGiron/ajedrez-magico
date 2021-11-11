#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>

using namespace std;

class FileReader {

private :
	string filename;

	void getGeneralinformation(int i, string line);
	void getPlays(vector<string> &origin, vector<string> &destiny, string line);
	bool validatePlays(vector<string> &origin, vector<string> &destiny);
	map<int, pair<string, string>> createDictionary(vector<string>& origin, vector<string>& destiny);

public: 
	FileReader(string filename) {
		this->filename = filename;
	}

	string gatherInformation();
	string removeBrackets(string line);
	string reorderPlayerNames(string name);
	
	template<typename Map>
	void printDictionary(Map& m);

	

};


