#include "HeuristicWin.h"
#include "Board.h"

float HeuristicWin::h(Board& board, int color, int depth)
{
	if (board.win == color)
		return (float)(WIN + depth);
	if (board.win == !color)
		return -(float)(WIN + depth);

	return 0;
}