#pragma once
#include <vector>

#define NONE 2
#define RED 0
#define YELLOW 1

class Board
{
public:
	std::vector<std::vector<int>> board;
	int win;
	int current_color;

	Board();

	bool move(int pos, int& idx);
	void retract(int pos, int idx);

	void checkWin(int x, int y, int color);
	void load(std::string path, int current_color);
	void save(std::string path);

};