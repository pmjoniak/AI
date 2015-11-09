#include "HeuristicAdv.h"
#include "Board.h"
#include <list>
#include <algorithm>

HeuristicAdv::HeuristicAdv()
{
	params = std::vector<float>(6);
}

float HeuristicAdv::strength1D(Board& b, int x, int y, int color, int dx, int dy, int type)
{
	if (used[x][y][type]) return 0;
	std::list<bool> data;
	data.push_back(true);
	for (int i = 1; i < 7; i++)
	{
		int xx = x + dx*i;
		int yy = y + dy*i;
		if (xx >= 0 && yy >= 0 && xx < 7 && yy < 6)
		{
			if (b.board[xx][yy] == color)
			{
				data.push_back(true);
				used[xx][yy][type] = true;
			}
			else if (b.board[xx][yy] == NONE)
				data.push_back(false);
			else
				break;
		}
		else
			break;
	}
	for (int i = 1; i < 7; i++)
	{
		int xx = x - dx*i;
		int yy = y - dy*i;
		if (xx >= 0 && yy >= 0 && xx < 7 && yy < 6)
		{
			if (b.board[xx][yy] == color)
			{
				data.push_front(true);
				used[xx][yy][type] = true;
			}
			else if (b.board[xx][yy] == NONE)
				data.push_front(false);
			else
				break;
		}
		else
			break;
	}

	if (data.size() < 4) return 0;
	std::vector<bool> vec = { data.begin(), data.end() };
	int minimum = 4;
	for (int i = 0; i <= data.size() - 4; i++)
	{
		int sum = 0;
		for (int j = 0; j < 4; j++)
		{
			if (vec[i + j] == false)
				sum++;
		}
		minimum = std::min(minimum, sum);
	}

	return params[(4 - minimum)];
}

float HeuristicAdv::strength(Board& b, int x, int y, int color)
{
	float sum = 0;
	sum += strength1D(b, x, y, color, 1, 0, 0);
	sum += strength1D(b, x, y, color, 0, 1, 1);
	sum += strength1D(b, x, y, color, 1, 1, 2);
	sum += strength1D(b, x, y, color, 1, -1, 3);
	return sum;
}

float HeuristicAdv::h(Board& board, int color, int depth)
{
	if (board.win == color)
		return (float)(WIN + depth);
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