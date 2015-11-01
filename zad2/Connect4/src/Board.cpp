#pragma once
#include "Board.h"
#include <fstream>

Board::Board()
{
	board = std::vector<std::vector<int>>(7, std::vector<int>(6, NONE));
	current_color = RED;
}

bool Board::move(int pos, int& idx)
{
	if (board[pos][5] != NONE)
		return false;
	for (idx = 0; idx < 6; idx++)
		if (board[pos][idx] == NONE)
			break;

	board[pos][idx] = current_color;
	checkWin(pos, idx, current_color);
	current_color = !current_color;
	return true;
}

void Board::retract(int pos, int idx)
{
	board[pos][idx] = NONE;
	current_color = !current_color;
}

void Board::checkWin(int x, int y, int color)
{
	win = NONE;

	//poziom
	int sum = 1;
	for (int i = 1; i < 4; i++)
		if (x - i >= 0)
		{
		if (board[x - i][y] == color)
			sum++;
		else
			break;
		}
	for (int i = 1; i < 4; i++)
		if (x + i < 7)
		{
		if (board[x + i][y] == color)
			sum++;
		else
			break;
		}
	if (sum >= 4){ win = color; return; }

	//pion
	sum = 1;
	for (int i = 1; i < 4; i++)
		if (y - i >= 0)
		{
		if (board[x][y - i] == color)
			sum++;
		else
			break;
		}
	if (sum >= 4){ win = color; return; }

	//skos 1
	sum = 1;
	for (int i = 1; i < 4; i++)
		if (x - i >= 0 && y - i >= 0)
		{
		if (board[x - i][y - i] == color)
			sum++;
		else
			break;
		}
	for (int i = 1; i < 4; i++)
		if (x + i < 7 && y + i < 6)
		{
		if (board[x + i][y + i] == color)
			sum++;
		else
			break;
		}
	if (sum >= 4){
		win = color; return;
	}

	//skos 2
	sum = 1;
	for (int i = 1; i < 4; i++)
		if (x - i >= 0 && y + i < 6)
		{
		if (board[x - i][y + i] == color)
			sum++;
		else
			break;
		}
	for (int i = 1; i < 4; i++)
		if (x + i < 7 && y - i >= 0)
		{
		if (board[x + i][y - i] == color)
			sum++;
		else
			break;
		}
	if (sum >= 4){
		win = color; return;
	}
}


void Board::load(std::string path, int current_color)
{
	std::ifstream in(path);
	int a;
	for (int y = 5; y >= 0; y--)
		for (int x = 0; x < 7; x++)
		{
			in >> a;
			board[x][y] = a;
		}
	this->current_color = current_color;
}

void Board::save(std::string path)
{
	std::ofstream out(path);
	for (int y = 5; y >= 0; y--)
	{
		for (int x = 0; x < 7; x++)
		{
			out << board[x][y] << " ";
		}
		out << "\n";
	}
}