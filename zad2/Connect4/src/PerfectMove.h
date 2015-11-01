#pragma once
#include <functional>

class Board;

class PerfectMove
{
private:
	Board& board;
	int start_color; 
	std::function<float(Board&, int)> h;

	struct res
	{
		int result = -1;
		int move = -1;
	};

public:
	PerfectMove(Board& board, std::function<float(Board&, int)> h);

	int findPerfectMove(int color, int depth);

	int recursiveMinMax(bool maximize, int depth);
};