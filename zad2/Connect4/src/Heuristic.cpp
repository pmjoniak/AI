#include "Heuristic.h"
#include "Board.h"

#define WIN 10000

bool used[7][6][4];

float strength1D(Board& b, int x, int y, int color, int dx, int dy, bool type)
{
	if (used[x][y][type]) return 0;
	float sum = 1;
	float block = 0;
	for (int i = 1; i < 4; i++)
	{
		int xx = x + dx*i;
		int yy = y + dy*i;
		if (xx >= 0 && yy >= 0 && xx < 7 && yy < 6)
		{
			if (b.board[xx][yy] == color)
			{
				sum += 1;
				used[xx][yy][type] = true;
			}
			else
			{
				if (b.board[xx][yy] == NONE) block += 0.5;
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
				sum += 1;
				used[xx][yy][type] = true;
			}
			else
			{
				if (b.board[xx][yy] == NONE) block += 0.5;
				break;
			}
		}
	}
	used[x][y][type] = true;
	if (block < 0.001) return 0;
	else return sum + block;
}

float strength(Board& b, int x, int y, int color)
{
	float sum = 0;
	sum += strength1D(b, x, y, color, 1, 0, 0);
	sum += strength1D(b, x, y, color, 0, 1, 1);
	sum += strength1D(b, x, y, color, 1, 1, 2);
	sum += strength1D(b, x, y, color, 1, -1, 3);
	return sum;
}

float h(Board& board, int color)
{
	if (board.win == color)
		return WIN;
	if (board.win == !color)
		return -WIN;

	for (int x = 0; x < 7; x++)
		for (int y = 0; y < 6; y++)
			for (int b = 0; b < 4; b++)
				used[x][y][b] = false;

	int sum = 0;
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