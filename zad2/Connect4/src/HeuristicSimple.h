#pragma once
#include <vector>
#include "Heuristic.h"

class HeuristicSimple : public Heuristic
{
public:
	HeuristicSimple();
	float h(Board& board, int color, int depth);
	std::vector<float> params;

private:
	bool used[7][6][4];
	
private:
	float strength(Board& b, int x, int y, int color);
	float strength1D(Board& b, int x, int y, int color, int dx, int dy, int type);
};