using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace connect4_web
{
    class Board
    {
        public static int NONE = 2;
        public static int RED = 0;
        public static int YELLOW = 1;

        public int[,] board = new int[7,6];
	    public int win = NONE;
	    public int current_color = RED;

        public Board(int start_color)
        {
	        clear(start_color);
        }


        public void clear(int start_color)
        {
            for (int x = 0; x < 7; x++)
                for (int y = 0; y < 6; y++)
                    board[x, y] = NONE;
	        current_color = start_color;
	        win = NONE;
        }

        public bool move(int pos, out int idx)
        {
            if (board[pos, 5] != NONE)
            { idx = -1; return false; }
	        for (idx = 0; idx < 6; idx++)
		        if (board[pos,idx] == NONE)
			        break;

	        board[pos,idx] = current_color;
	        checkWin(pos, idx, current_color);
	        current_color = 1 - current_color;
	        return true;
        }

        public void retract(int pos, int idx)
        {
	        board[pos,idx] = NONE;
	        current_color = 1 - current_color;
	        win = NONE;
        }

        void checkWin(int x, int y, int color)
        {
        	win = NONE;
        
        	//poziom
        	int sum = 1;
        	for (int i = 1; i < 4; i++)
        		if (x - i >= 0)
        		{
        		if (board[x - i,y] == color)
        			sum++;
        		else
        			break;
        		}
        	for (int i = 1; i < 4; i++)
        		if (x + i < 7)
        		{
        		if (board[x + i,y] == color)
        			sum++;
        		else
        			break;
        		}
        	if (sum >= 4){ win = color; return; }
        
        	//pion
        	sum = 1;
        	for (int i = 1; i < 4; i++)
        		if (y - i >= 0)
        		{
        		if (board[x,y - i] == color)
        			sum++;
        		else
        			break;
        		}
        	if (sum >= 4){ win = color; return; }
        
        	//skos 1
        	sum = 1;
        	for (int i = 1; i < 4; i++)
        		if (x - i >= 0 && y - i >= 0)
        		{
        		if (board[x - i,y - i] == color)
        			sum++;
        		else
        			break;
        		}
        	for (int i = 1; i < 4; i++)
        		if (x + i < 7 && y + i < 6)
        		{
        		if (board[x + i,y + i] == color)
        			sum++;
        		else
        			break;
        		}
        	if (sum >= 4){
        		win = color; return;
        	}
        
        	//skos 2
        	sum = 1;
        	for (int i = 1; i < 4; i++)
        		if (x - i >= 0 && y + i < 6)
        		{
        		if (board[x - i,y + i] == color)
        			sum++;
        		else
        			break;
        		}
        	for (int i = 1; i < 4; i++)
        		if (x + i < 7 && y - i >= 0)
        		{
        		if (board[x + i,y - i] == color)
        			sum++;
        		else
        			break;
        		}
        	if (sum >= 4){
        		win = color; return;
        	}
        }

    }
}
