#include "PerfectMove.h"
#include "Board.h"
#include <vector>
#include <algorithm>
#include <iostream>

PerfectMove::PerfectMove(Board& board, std::function<float(Board&, int)> h)
	: board(board), h(h)
{

}

int PerfectMove::findPerfectMove(int color, int depth)
{
	start_color = color;
	std::vector<res> results;
	int idx;
	for (int i = 0; i < 7; i++)
	{
		if (board.move(i, idx))
		{
			res r;
			r.result = recursiveMinMax(false, depth - 1);
			r.move = i;
			results.push_back(r);
			board.retract(i, idx);
		}
	}
	std::sort(results.begin(), results.end(), [](const res& r1, const res& r2){
		return r1.result > r2.result;
	});
	float best = results[0].result;
	int count = 0;
	for (count = 1; count <= results.size(); count++)
		if (results[count].result < best)
			break;
			
	return results[rand() % count].move;
}

int PerfectMove::recursiveMinMax(bool maximize, int depth)
{
	if (depth <= 0 || board.win != NONE)
		return h(board, start_color);

	int idx;
	bool any = false;
	float best = (maximize ? -1 : 1) * std::numeric_limits<float>::max();
	for (int i = 0; i < 7; i++)
	{
		if (board.move(i, idx))
		{
			any = true;
			float val = recursiveMinMax(!maximize, depth - 1);
			if (maximize)
				best = std::max(best, val);
			else
				best = std::min(best, val);

			board.retract(i, idx);
		}
	}
	if (!any)
		return h(board, start_color);
	return best;
}