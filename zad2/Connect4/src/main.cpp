#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include "Renderer.h"
#include "Utils.h"
#include "Board.h"
#include "PerfectMove.h"
#include "Controller.h"

void draw(Board& board);

void clearConsole()
{
#ifdef _WIN32
	//system("cls");
#else
	//system("clear");
#endif
}

int main()
{
	Board board(YELLOW);
	
	auto controller = std::make_shared<Controller>(board);
	controller->players[RED] = std::make_unique<AIPlayer>();
	controller->players[YELLOW] = std::make_unique<HumanPlayer>();

	while (1)
	{
		board.clear(RED);
		//parse args
		clearConsole();
		std::cout << "Welcome to Connect4.\n"
			"Configure gameplay, possible commands:\n\n"
			"-[color]:(h|ai-[depth]) -- configure players color can by 'y' or 'r'\n\n"
			"-s:[color] -- select first player\n\n"
			"-p -- play\n\n"
			"-q -- exit\n\n"
			"> ";
		while (1)
		{
			std::string arg;
			std::cin >> arg;
			if (arg.size() >= 2 && arg[0] == '-')
			{
				if (arg[1] == 'r' || arg[1] == 'y')
				{
					if (arg[3] == 'h')
					{
						controller->players[(arg[1] == 'r' ? RED : YELLOW)] = std::make_unique<HumanPlayer>();
						std::cout << "Player set to Human\n> ";
					}
					if (arg[3] == 'a' && arg[4] == 'i')
					{
						int depth = atoi(&arg[6]);
						controller->players[(arg[1] == 'r' ? RED : YELLOW)] = std::make_unique<AIPlayer>(depth);
						std::cout << "Player set to AI with depth " << depth << "\n> ";
					}
				}
				if (arg[1] == 's')
				{
					board.current_color = (arg[3] == 'r' ? RED : YELLOW);
					std::cout << "First player set\n> ";
				}
				if (arg[1] == 'p')
					break;
				if (arg[1] == 'q')
					return 0;
			}
		}
		{
			std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(board, 480, 480);
			renderer->setController(controller);
			renderer->run();
		}
	}


	//while (1)
	//{
	//	draw(board);
	//	if (player == 0)
	//	{
	//		gotoxy(0, 23);
	//		std::cout << "Gdzie zrzucic(0-6)? ";
	//		int pos;
	//		std::cin >> pos;
	//		if (pos == 9)
	//			board.save("save.txt");
	//		if (pos < 0 || pos > 6)
	//		{
	//			std::cout << "nie!!";
	//			continue;
	//		}
	//		if (!board.move(pos, idx)) continue;
	//	}
	//	else
	//	{
	//		PerfectMove perfect_move(board, h);
	//		board.move(perfect_move.findPerfectMove(board.current_color, 7), idx);
	//	}
	//	if (board.win != NONE)
	//	{
	//		draw(board);
	//		gotoxy(0, 24);
	//		std::cout << "Player: " << (player == YELLOW ? 'x' : 'o') << " win\n";
	//		return 0;
	//	}
	//	player = !player;
	//}
	return 0;
}


void draw(Board& board)
{
	system("cls");

	for (int i = 0; i < 6; i++)
	{
		gotoxy(0, 10 + (6 - i));
		std::cout << "|";
		for (int j = 0; j < 7; j++)
			std::cout << (board.board[j][i] == NONE ? ' ' : (board.board[j][i] == YELLOW ? 'x' : 'o')) << "|";
	}
	gotoxy(0, 17);
	std::cout << "---------------";
	gotoxy(0, 18);
	std::cout << " 0 1 2 3 4 5 6";
}