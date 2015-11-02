#include "PerfectMove.h"
#include "Board.h"
#include <vector>
#include <algorithm>
#include <iostream>

PerfectMove::PerfectMove(Board& board, std::function<float(Board&, int, int)> h)
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
			r.result = recursiveMinMax(false, depth - 1, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
			r.move = i;
			results.push_back(r);
			board.retract(i, idx);
		}
	}
	std::sort(results.begin(), results.end(), [](const res& r1, const res& r2){
		return r1.result > r2.result;
	});
	if (results.empty()) return -1;
	float best = results[0].result;
	unsigned int count = 0;
	for (count = 1; count < results.size(); count++)
		if (results[count].result < best)
			break;
			
	return results[rand() % count].move;
	//return results[0].move;
}

float PerfectMove::recursiveMinMax(bool maximize, int depth, float alpha, float beta)
{
	if (depth <= 0 || board.win != NONE)
		return h(board, start_color, depth);

	int idx;
	bool any = false;
	float best = (maximize ? -1 : 1) * std::numeric_limits<float>::max();
	for (int i = 0; i < 7; i++)
	{
		if (board.move(i, idx))
		{
			any = true;
			float val = recursiveMinMax(!maximize, depth - 1, alpha, beta);
			if (maximize)
			{
				best = std::max(best, val);
				alpha = std::max(alpha, best);
			}
			else
			{
				best = std::min(best, val);
				beta = std::min(beta, best);
			}
			board.retract(i, idx);
			if (beta <= alpha)
				break;
		}
	}
	if (!any)
		return h(board, start_color, depth);
	return best;
}