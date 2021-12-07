#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>
#include <glm/ext/vector_float3.hpp>

using namespace std;

class Movements {


private:


public:
	void printBoard(vector<vector<char>> transformedBoard);
	void RawBoard(vector<char> rawBoard);
	vector<glm::vec3> compareBoards(vector<vector<vector<char>>> allBoardStatus, int position);

};