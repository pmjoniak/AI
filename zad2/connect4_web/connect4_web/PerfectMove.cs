using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace connect4_web
{
    class res
    {
        public float result = -1;
        public int move = -1;
    };


    class PerfectMove
    {
        Board board;
	    int start_color; 
	    
        public PerfectMove(Board board)
        {
            this.board = board;
        }

        public int findPerfectMove(int color, int depth)
        {
	        start_color = color;
	        List<res> results = new List<res>();
	        int idx;
	        for (int i = 0; i < 7; i++)
	        {
		        if (board.move(i, out idx))
		        {
			        res r = new res();
			        r.result = recursiveMinMax(false, depth - 1, -999999, 999999);
			        r.move = i;
			        results.Add(r);
			        board.retract(i, idx);
		        }
	        }
            results.Sort(delegate(res r1, res r2){
                return (r1.result > r2.result ? -1 : 1);
            });
	        float best = results[0].result;
	        int count = 0;
	        for (count = 1; count < results.Count; count++)
		        if (results[count].result < best)
			        break;

            Random random = new Random();
	        return results[random.Next(count)].move;
	        //return results[0].move;
        }

        public float recursiveMinMax(bool maximize, int depth, float alpha, float beta)
        {
	        if (depth <= 0 || board.win != Board.NONE)
		        return h(board, start_color);

	        int idx;
	        bool any = false;
	        float best = (maximize ? -1 : 1) * 999999;
	        for (int i = 0; i < 7; i++)
	        {
		        if (board.move(i, out idx))
		        {
			        any = true;
			        float val = recursiveMinMax(!maximize, depth - 1, alpha, beta);
			        if (maximize)
			        {
				        best = Math.Max(best, val);
				        alpha = Math.Max(alpha, best);
			        }
			        else
			        {
				        best = Math.Min(best, val);
                        beta = Math.Min(beta, best);
			        }
			        board.retract(i, idx);
			        if (beta <= alpha)
				        break;
		        }
	        }
	        if (!any)
		        return h(board, start_color);
	        return best;
        }








        static int WIN = 10000;

        bool[,,] used = new bool [7,6,4];

        float strength1D(Board b, int x, int y, int color, int dx, int dy, int type)
        {
	        if (used[x,y,type]) return 0;
	        float sum = 1;
	        float block = 0;
	        for (int i = 1; i < 4; i++)
	        {
		        int xx = x + dx*i;
		        int yy = y + dy*i;
		        if (xx >= 0 && yy >= 0 && xx < 7 && yy < 6)
		        {
			        if (b.board[xx,yy] == color)
			        {
				        sum += 1;
				        used[xx,yy,type] = true;
			        }
			        else
			        {
				        if (b.board[xx,yy] == Board.NONE) block += 0.5f;
				        break;
			        }
		        }
	        }
	        for (int i = 1; i < 4; i++)
	        {
		        int xx = x - dx*i;
		        int yy = y - dy*i;
		        if (xx >= 0 && yy >= 0 && xx < 7 && yy < 6)
		        {
			        if (b.board[xx,yy] == color)
			        {
				        sum += 1;
				        used[xx,yy,type] = true;
			        }
			        else
			        {
                        if (b.board[xx, yy] == Board.NONE) block += 0.5f;
				        break;
			        }
		        }
	        }
	        used[x,y,type] = true;
	        if (block < 0.001) return 0;
	        else return sum + block;
        }

        float strength(Board b, int x, int y, int color)
        {
	        float sum = 0;
	        sum += strength1D(b, x, y, color, 1, 0, 0);
	        sum += strength1D(b, x, y, color, 0, 1, 1);
	        sum += strength1D(b, x, y, color, 1, 1, 2);
	        sum += strength1D(b, x, y, color, 1, -1, 3);
	        return sum;
        }

        float h(Board board, int color)
        {
	        if (board.win == color)
		        return WIN;
	        if (board.win == 1-color)
		        return -WIN;

	        for (int x = 0; x < 7; x++)
		        for (int y = 0; y < 6; y++)
			        for (int b = 0; b < 4; b++)
				        used[x,y,b] = false;

	        float sum = 0;
	        for (int x = 0; x < 7; x++)
	        {
		        for (int y = 0; y < 6; y++)
		        {
			        if (board.board[x,y] == color)
				        sum += strength(board, x, y, color);
		        }
	        }

	        return sum;
        }

    }
}
