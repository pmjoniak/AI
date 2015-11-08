#pragma once

class Board;
#define WIN 10000

class Heuristic
{
public:
	virtual float h(Board& board, int color, int depth) = 0;
};