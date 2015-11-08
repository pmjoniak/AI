#pragma once
#include "Heuristic.h"

class HeuristicWin : public Heuristic
{
public:
	float h(Board& board, int color, int depth);

};