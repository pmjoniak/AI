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

void shool()
{
	srand(time(NULL));
	Board board(YELLOW);
	Heuristic h_best;
	Heuristic h_new;
	h_best.params = { 1.5, 2, 2.5, 3, 3.5, 4 };
	//h_best.params = { 1, 1, 1, 1, 1, 1 };
	int idx;
	while (1)
	{
		h_new.params = h_best.params;
		h_new.params[rand() % 6] *= (1.0f + (float)((rand() % 10000) - 5000) / 25000.0f);
		//h_new.params[r] = h_new.params[r] + ((float)((rand() % 10000) - 5000) / 10000.0f);
		std::map<int, std::unique_ptr<AIPlayer>> players;
		players[RED] = std::make_unique<AIPlayer>(&h_best, 6);
		players[YELLOW] = std::make_unique<AIPlayer>(&h_new, 6);

		int win_best = 0;
		int win_new = 0;
		for (int i = 0; i < 100; i++)
		{
			board.clear(rand() % 2);
			while (board.win == NONE)
			{
				int move = players[board.current_color]->move(board);
				if (move != -1) board.move(move, idx); else break;
			}
			if (board.win == RED) win_best++;
			else if (board.win == YELLOW) win_new++;
			//std::cout << i << ": " << win_best << " : " << win_new << "\n";
		}
		std::cout << win_best << " : " << win_new << "\n";
		if (win_new > win_best)
			h_best.params = h_new.params;
		std::cout << "params: [" << h_best.params[0] << ", " << h_best.params[1] << ", " << h_best.params[2] << ", " << h_best.params[3] << ", " << h_best.params[4] << ", " << h_best.params[5] << "]\n";
	}
}

void test()
{
	Board board(YELLOW);
	Heuristic h_best;
	Heuristic h_new;
	int idx;
	h_best.params = { 1.5, 2, 2.5, 3, 3.5, 4 };
	h_new.params = { 0.0331f, 0.1287f, 0.0316f, 0.338f, 0.4693f, 0.00061f };
	std::map<int, std::unique_ptr<AIPlayer>> players;
	players[RED] = std::make_unique<AIPlayer>(&h_best, 4);
	players[YELLOW] = std::make_unique<AIPlayer>(&h_new, 4);

	int win_best = 0;
	int win_new = 0;
	for (int i = 0; i < 10000; i++)
	{
		board.clear(rand() % 2);
		while (board.win == NONE)
		{
			int move = players[board.current_color]->move(board);
			if (move != -1) board.move(move, idx); else break;
		}
		if (board.win == RED) win_best++;
		else if (board.win == YELLOW) win_new++;
		std::cout << i << ": " << win_best << " : " << win_new << "\n";
	}
	//std::cout << win_best << " : " << win_new << "\n";
}

int main()
{
	//test();
	shool();

	Board board(YELLOW);
	Heuristic h;
	h.params = { 1.5, 2, 2.5, 3, 3.5, 4 };
	Heuristic h2;
	h2.params = { 0.0331f, 0.1287f, 0.0316f, 0.338f, 0.4693f, 0.00061f };

	auto controller = std::make_shared<Controller>(board);
	controller->players[RED] = std::make_unique<AIPlayer>(&h);
	controller->players[YELLOW] = std::make_unique<AIPlayer>(&h2);
	//controller->players[YELLOW] = std::make_unique<HumanPlayer>();

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
						controller->players[(arg[1] == 'r' ? RED : YELLOW)] = std::make_unique<AIPlayer>(&h, depth);
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