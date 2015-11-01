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

public:
	AIPlayer(int depth = 5)
	{
		this->depth = depth;
		type = PlayerType::AI;
	}

	int move(Board& board)
	{
		PerfectMove perfect_move(board, h);
		return perfect_move.findPerfectMove(board.current_color, depth);
	}
};