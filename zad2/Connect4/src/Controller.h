#pragma once
#include "Board.h"
#include <map>
#include "Player.h"
#include <memory>

class Controller
{
public:
	Controller(Board& board);

	void process();
	void drop(int idx);

	std::map<int, std::unique_ptr<Player>> players;

private:
	Board& board;
};