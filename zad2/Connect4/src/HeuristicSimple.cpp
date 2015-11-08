#include "HeuristicSimple.h"
#include "Board.h"

HeuristicSimple::HeuristicSimple()
{
	params = std::vector<float>(6);
}

float HeuristicSimple::strength1D(Board& b, int x, int y, int color, int dx, int dy, int type)
{
	if (used[x][y][type]) return 0;
	int sum = 1;
	int block = 0;
	for (int i = 1; i < 4; i++)
	{
		int xx = x + dx*i;
		int yy = y + dy*i;
		if (xx >= 0 && yy >= 0 && xx < 7 && yy < 6)
		{
			if (b.board[xx][yy] == color)
			{
				sum++;
				used[xx][yy][type] = true;
			}
			else
			{
				if (b.board[xx][yy] == NONE) block++;
				break;
			}
		}
	}
	for (int i = 1; i < 4; i++)
	{
		int xx = x - dx*i;
		int yy = y - dy*i;
		if (xx >= 0 && yy >= 0 && xx < 7 && yy < 6)
		{
			if (b.board[xx][yy] == color)
			{
				sum ++;
				used[xx][yy][type] = true;
			}
			else
			{
				if (b.board[xx][yy] == NONE) block++;
				break;
			}
		}
	}
	used[x][y][type] = true;
	if (block == 0) return 0;
	else return params[(sum-1)*2+block-1];
}

float HeuristicSimple::strength(Board& b, int x, int y, int color)
{
	float sum = 0;
	sum += strength1D(b, x, y, color, 1, 0, 0);
	sum += strength1D(b, x, y, color, 0, 1, 1);
	sum += strength1D(b, x, y, color, 1, 1, 2);
	sum += strength1D(b, x, y, color, 1, -1, 3);
	return sum;
}

float HeuristicSimple::h(Board& board, int color, int depth)
{
	if (board.win == color)
		return (float)(WIN+depth);
	if (board.win == !color)
		return -(float)(WIN + depth);

	for (int x = 0; x < 7; x++)
		for (int y = 0; y < 6; y++)
			for (int b = 0; b < 4; b++)
				used[x][y][b] = false;

	float sum = 0;
	for (int x = 0; x < 7; x++)
	{
		for (int y = 0; y < 6; y++)
		{
			if (board.board[x][y] == color)
				sum += strength(board, x, y, color);
		}
	}

	return sum;
}