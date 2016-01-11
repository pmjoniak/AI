#include "Board.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <iomanip>

double gamma = 1;
double e = 0.4;

void showU(std::ostream& out, std::vector<std::vector<double>>& U, Board& board)
{
	for (int y = 0; y < board.m; y++)
	{
		for (int x = 0; x < board.n; x++)
		{
			out << std::fixed << std::setprecision(4) << std::setw(6) << U[x][board.m - y - 1] << (x != board.n - 1 ? ", " : "");
		}
		out << "\n";
	}
	out << "\n";
	out << "\n";
}

char a2arrow(int a)
{
	if (a == -1) return '-';
	if (a == 0) return '^';
	if (a == 1) return '>';
	if (a == 2) return 'v';
	if (a == 3) return '<';
}

void showPi(std::ostream& out, std::vector<std::vector<int>>& pi, Board& board)
{
	for (int y = 0; y < board.m; y++)
	{
		for (int x = 0; x < board.n; x++)
		{
			out << a2arrow(pi[x][board.m - y - 1]) << " ";
		}
		out << "\n";
	}
	out << "\n";
	out << "\n";
}

std::vector<std::vector<std::vector<double>>> U_h;

std::vector<std::vector<double>> iteracjaWartosci(Board& board)
{
	U_h.clear();
	auto U = std::vector<std::vector<double>>(board.n, std::vector<double>(board.m, 0));
	for (int y = 0; y < board.m; y++)
		for (int x = 0; x < board.n; x++)
			if (board.T(x, y))
				U[x][y] = board.R(x, y);

	for (int i = 0; i < 5000; i++)
	{
		U_h.push_back(U);
		auto U_old = U;
		double max_e = -std::numeric_limits<double>::max();
		for (int y = 0; y < board.m; y++)
		{
			for (int x = 0; x < board.n; x++)
			{
				if (board.F(x, y) || board.T(x, y)) continue;
				double maximum = -std::numeric_limits<double>::max();
				for (int a = 0; a < 4; a++)
				{
					double sum = 0;
					auto ss = board.getNewS(x, y, a);
					for (auto s : ss)
						sum += s.p * U_old[s.x][s.y];
					maximum = std::max(maximum, sum);
				}

				U[x][y] = board.R(x, y) + gamma*maximum;
				max_e = std::max(max_e, abs(U[x][y] - U_old[x][y]));
			}
		}
		//showU(U, board);
		if (max_e < 0.00001)
		{
			std::cout << "iterations: " << i << "\n";
			return U;
		}
	}

	return U;
}

std::vector<std::vector<int>> polityka(std::vector<std::vector<double>>& U, Board& board)
{
	auto pi = std::vector<std::vector<int>>(board.n, std::vector<int>(board.m, 0));
	for (int y = 0; y < board.m; y++)
	{
		for (int x = 0; x < board.n; x++)
		{
			if (board.F(x, y) || board.T(x,y)) {pi[x][y] = -1; continue;}
			int best_a = -1;
			double maximum = -std::numeric_limits<double>::max();

			for (int a = 0; a < 4; a++)
			{
				auto ss = board.getNewS(x, y, a);
				double sum = 0;
				for (auto s : ss)
					sum += U[s.x][s.y] * s.p;
				if (sum > maximum)
				{
					maximum = sum;
					best_a = a;
				}
			}
			pi[x][y] = best_a;
		}
	}

	return pi;
}

void saveUh(Board& board, std::string path)
{
	std::ofstream out(path);

	out << "nr ";
	for (int y = 0; y < board.m; y++)
		for (int x = 0; x < board.n; x++)
			out << "(" << x+1 << "," << y+1 << ") ";
	out << "\n";
	for (int i = 0; i < U_h.size(); i++)
	{
		auto& u = U_h[i];
		out << i << " ";
		for (int y = 0; y < board.m; y++)
		{
			for (int x = 0; x < board.n; x++)
			{
				out << u[x][y] << " ";
			}
		}
		out << "\n";
	}

	out.close();
}


void Q_learning(Board& board, std::string path_result)
{
	
	board.reset();
	auto Q = std::vector<std::vector<std::vector<double>>>(board.n, std::vector<std::vector<double>>(board.m, std::vector<double>(4, 0.0)));
	auto N = std::vector<std::vector<std::vector<int>>>(board.n, std::vector<std::vector<int>>(board.m, std::vector<int>(4, 0)));
	auto U = std::vector<std::vector<double>>(board.n, std::vector<double>(board.m, 0));

	auto getBestA = [&Q](int x, int y)
	{
		int best_a = -1;
		double maximum = -std::numeric_limits<double>::max();
		for (int a = 0; a < 4; a++)
			if (maximum < Q[x][y][a])
			{
			maximum = Q[x][y][a];
			best_a = a;
			}
		return best_a;
	};

	auto getA = [&](){
		double p = (double)(rand() % 10000) / 10000.0;
		if (p < 1 - e)
			return getBestA(board.cx, board.cy);
		else
			return rand() % 4;
	};

	bool start = true;
	int l_a=-1;
	int l_x = board.start_x;
	int l_y = board.start_y;
	double l_r = -1;
	int resets = 0;
	for (int i = 0; i < 100000000; i++)
	{
		//if (board.T(board.cx, board.cy))
		//{
		//	for (int a = 0; a < 4; a++)
		//		Q[board.cx][board.cy][a] = board.R(board.cx, board.cy);
		//}
		if (!start)
		{
			N[l_x][l_y][l_a]++;
			//double alpha = 1.0 / std::pow(N[l_x][l_y][l_a],0.6);
			double alpha = 1.0 / N[l_x][l_y][l_a];
			double max_a = -std::numeric_limits<double>::max();
			for (int a = 0; a < 4; a++)
				max_a = std::max(max_a, Q[board.cx][board.cy][a]);
			double dq = alpha * (board.R(board.cx, board.cy) + gamma * max_a - Q[l_x][l_y][l_a]);
			Q[l_x][l_y][l_a] += dq;
			//if (std::abs(dq) > 0.0001)
			//	std::cout << dq << "\n";
		}
		if (board.T(board.cx, board.cy))
		{
			board.reset();
			resets++;
			start = true;
			//std::cout << "restets: " << resets << "\n";
			//for (int y = 0; y < board.m; y++)
			//{
			//	for (int x = 0; x < board.n; x++)
			//	{
			//		std::cout << std::setprecision(3) << std::setw(6) << Q[x][board.m - y - 1][getBestA(x, board.m - y - 1)] << ", ";
			//	}
			//	std::cout << "\n";
			//}
			//std::cout << "\n";
			//std::cout << "\n";
			if (resets % 1000 == 0)
			{
				for (int y = 0; y < board.m; y++)
					for (int x = 0; x < board.n; x++)
						U[x][y] = Q[x][y][getBestA(x, y)];
				U_h.push_back(U);
			}
			continue;
		}
		l_x = board.cx;
		l_y = board.cy;
		l_a = getA();
		l_r = board.R(board.cx, board.cy);
		board.move(l_a);
		//if (board.T(board.cx, board.cy))
		//{
		//	for (int a = 0; a < 4; a++)
		//		Q[board.cx][board.cy][a] = board.R(board.cx, board.cy);
		//}
		start = false;
		if (i % 10000 == 0) std::cout << i << "\n";
	}
	std::ofstream out(path_result);
	out << "Przebiegow: " << resets << "\n";
	out << "Uzytecznosci:\n";
	for (int y = 0; y < board.m; y++)
	{
		for (int x = 0; x < board.n; x++)
		{
			out << std::fixed << std::setprecision(2) << std::setw(6) << Q[x][board.m - y - 1][getBestA(x, board.m - y - 1)] << ", ";
		}
		out << "\n";
	}
	out << "\n";
	out << "\n";

	out << "Polityka:\n";
	for (int y = 0; y < board.m; y++)
	{
		for (int x = 0; x < board.n; x++)
		{
			if (board.T(x, board.m - y - 1) || board.F(x, board.m - y - 1))
				out << "- ";
			else
				out << a2arrow(getBestA(x, board.m - y - 1)) << " ";
		}
		out << "\n";
	}

	out.close();
}

std::string dir = "";
std::string board_path = "board.txt";
std::string result_path = "result.txt";
int iteracje = true;
int q = false;

void loadConfig()
{
	std::ifstream in(dir+"config.txt");
	std::string param;
	while (!in.eof())
	{
		in >> param;
		if (param == "board_file")
		{
			in >> board_path;
		}
		else if (param == "gamma")
		{
			in >> gamma;
		}
		else if (param == "e")
		{
			in >> e;
		}
		else if (param == "iteracja_wartosci")
		{
			in >> iteracje;
		}
		else if (param == "q_learning")
		{
			in >> q;
		}
	}
}


int main(int argc, char** argv)
{
	if (argc >= 2)
		dir = std::string(argv[1]);
	loadConfig();

	Board board;
	board.load(dir + board_path);

	srand(time(NULL));

	if (iteracje)
	{
		auto U = iteracjaWartosci(board);
		auto pi = polityka(U, board);

		std::ofstream out(dir+result_path);
		out << "Iteracji: " << U_h.size() << "\n\n";
		out << "Uzytecznosci:\n";
		showU(out, U, board);
		out << "Polityka:\n";
		showPi(out, pi, board);
		out.close();
		
		saveUh(board, dir + "data.dat");
	}

	if (q)
	{
		Q_learning(board, dir + result_path);
		saveUh(board, dir + "data.dat");
	}
}