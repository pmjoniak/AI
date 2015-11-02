#pragma once
#include "Board.h"
#include "PerfectMove.h"
#include "Heuristic.h"

enum class PlayerType : int
{
	AI,
	Human,
};

class Player
{
public:
	PlayerType type;

	virtual int move(Board& board) { return -1; }
};


class HumanPlayer : public Player
{
public:
	HumanPlayer()
	{
		type = PlayerType::Human;
	}
};

class AIPlayer : public Player
{
private:
	int depth = 5;
	Heuristic* h;

public:
	AIPlayer(Heuristic* h,  int depth = 5)
	{
		this->depth = depth;
		this->h = h;
		type = PlayerType::AI;
	}

	int move(Board& board)
	{
		PerfectMove perfect_move(board, std::bind(&Heuristic::h, h, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return perfect_move.findPerfectMove(board.current_color, depth);
	}
};