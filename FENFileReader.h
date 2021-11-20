#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>

using namespace std;

class FENFileReader {

	string filename;

private:
	string retrievePlays(string line);
	vector<string> generateRawBoard(string plays);
	vector<vector<char>> transformRawBoard(vector<string>);

public:

	FENFileReader(string filename) {
		this->filename = filename;
	}

	vector<vector<vector<char>>> prepareBoard();
	void printRawBoard(vector<string> rawBoard);
	void printTransformedBoard(vector<vector<char>> transformedBoard);
	void printAllBoardStatus(vector<vector<vector<char>>> allBoardStatus);

};