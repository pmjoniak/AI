#pragma once
#include <functional>

class Board;

class PerfectMove
{
private:
	Board& board;
	int start_color; 
	std::function<float(Board&, int, int)> h;

	struct res
	{
		float result = -1;
		int move = -1;
	};

public:
	PerfectMove(Board& board, std::function<float(Board&, int, int)> h);

	int findPerfectMove(int color, int depth);

	float recursiveMinMax(bool maximize, int depth, float alpha, float beta);
};