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
public:

	FENFileReader(string filename) {
		this->filename = filename;
	}

	void prepareBoard();

};