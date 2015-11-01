#include<vector>
#include<iostream>
#include<algorithm>
#include"Utils.h"
#include "Board.h"
#include "PerfectMove.h"


#define WIN 10000


float h(Board& board, int color)
{
	if (board.win == color)
		return WIN;
	if (board.win == !color)
		return -WIN;
	return 0;
}





void draw(Board& board);


int main()
{
	Board board;
	int idx = 0;
	int player = 0;
	//board.load("save.txt", 0);
	while (1)
	{
		draw(board);
		if (player == 0)
		{
			gotoxy(0, 23);
			std::cout << "Gdzie zrzucic(0-6)? ";
			int pos;
			std::cin >> pos;
			if (pos == 9)
				board.save("save.txt");
			if (pos < 0 || pos > 6)
			{
				std::cout << "nie!!";
				continue;
			}
			if (!board.move(pos, idx)) continue;
		}
		else
		{
			PerfectMove perfect_move(board, h);
			board.move(perfect_move.findPerfectMove(board.current_color, 7), idx);
		}
		if (board.win != NONE)
		{
			draw(board);
			gotoxy(0, 24);
			std::cout << "Player: " << (player == YELLOW ? 'x' : 'o') << " win\n";
			return 0;
		}
		player = !player;
	}
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