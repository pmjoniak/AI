#pragma once
#include <vector>
#include <string>

#define NONE 2
#define RED 0
#define YELLOW 1

class Board
{
public:
	std::vector<std::vector<int>> board;
	int win = NONE;
	int current_color = RED;

	Board(int start_color = RED);
	void clear(int start_color = RED);

	bool move(int pos, int& idx);
	void retract(int pos, int idx);

	void checkWin(int x, int y, int color);
	void load(std::string path, int current_color);
	void save(std::string path);

};
