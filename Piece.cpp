#include "Piece.h"

Piece::Piece(string name, string color, int nMoviments)
{
	this->name = name;
	this->color = color;
	this->alive = true;
	this->moviments = new pair<int, int>[nMoviments]

}
