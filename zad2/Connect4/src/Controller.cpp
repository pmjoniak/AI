#include "Controller.h"
#include <iostream>

Controller::Controller(Board& board)
	:board(board)
{

}

void Controller::process()
{
	if (board.win != NONE) return;
	int i;
	if (players.at(board.current_color)->type == PlayerType::AI)
	{
		int move = players.at(board.current_color)->move(board);
		if (move != -1)
		{
			board.move(move, i);
			if (board.win != NONE)
				std::cout << "Player: " << (board.win == YELLOW ? "YELLOW" : "RED") << " win!!\n";
		}
		else
		{
			std::cout << "Draw!!\n";
			board.win = RED;
		}

	}
}

void Controller::drop(int idx)
{
	if (board.win != NONE) return;
	int i;
	board.move(idx, i);
	if (board.win != NONE)
		std::cout << "Player: " << (board.win == YELLOW ? "YELLOW" : "RED") << " win!!\n";
}