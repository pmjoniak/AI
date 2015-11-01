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
		board.move(players.at(board.current_color)->move(board), i);
		if (board.win != NONE)
			std::cout << "Player: " << (board.win == YELLOW ? "YELLOW" : "RED") << " win!!\n";
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