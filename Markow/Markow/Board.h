#pragma once;
#include <vector>
#include <fstream>


struct S
{
	int x;
	int y;
	double p;
};

class Board
{
private:
	std::vector<std::vector<double>> values;
	std::vector<std::vector<bool>> terminated;
	std::vector<std::vector<bool>> forbidden;

public:
	int n, m;
	int start_x, start_y;
	double p1, p2, p3, p4;
	int cx, cy;

public:

	void load(std::string path)
	{

		std::ifstream in(path.c_str(), std::ios::in);
		in >> n >> m;
		terminated = std::vector<std::vector<bool>>(n, std::vector<bool>(m, false));
		forbidden = std::vector<std::vector<bool>>(n, std::vector<bool>(m, false));
		in.ignore(20, '\n');
		int i = 0;
		for (int y = 0; y < m; y++)
		{
			in.ignore(n*3, '\n');
			for (int x = 0; x < n; x++)
			{
				char c;
				in.get();
				c = (char)in.get();
				switch (c)
				{
				case 'S':
					start_x = x;
					start_y = m - y - 1;
					break;
				case 'T':
					terminated[x][m - y - 1] = true;
					i++;
					break;
				case 'F':
					forbidden[x][m - y - 1] = true;
					break;
				case 'B':
					i++;
					break;
				}
			}
			in.ignore(2);
		}
		in.ignore(n * 3, '\n');
		double def_val;
		in >> def_val;
		values = std::vector<std::vector<double>>(n, std::vector<double>(m, def_val));
		for (int a = 0; a < i; a++)
		{
			int x, y;
			double v;
			in >> x >> y >> v;
			values[x][m - y - 1] = v;
		}
		in >> p1 >> p2 >> p3;
		p4 = 1 - (p1 + p2 + p3);
		in.close();
	}

	double R(int x, int y)
	{
		return values[x][y];
	}

	bool T(int x, int y)
	{
		return terminated[x][y];
	}

	bool F(int x, int y)
	{
		return forbidden[x][y];
	}

	std::vector<S> getNewS(int x, int y, int a)
	{
		if (terminated[x][y])
		{
			return{ { x, y, 1 } };
		}
		std::vector<S> res;

		auto add = [&res, x, y, this](int nx, int ny, double p)
		{
			if (nx < 0) nx = 0;
			if (nx >= n) nx = n - 1;
			if (ny < 0) ny = 0;
			if (ny >= m) ny = m - 1;
			if (this->forbidden[nx][ny]) { nx = x; ny = y;}
			res.push_back({ nx, ny, p });
		};

		if (a == 0)
		{
			add(x, y + 1, p1);
			add(x - 1, y, p2);
			add(x + 1, y, p3);
			add(x, y - 1, p4);
		}
		if (a == 1)
		{
			add(x + 1, y, p1);
			add(x - 1, y, p4);
			add(x, y + 1, p2);
			add(x, y - 1, p3);
		}
		if (a == 2)
		{
			add(x + 1, y, p2);
			add(x - 1, y, p3);
			add(x, y + 1, p4);
			add(x, y - 1, p1);
		}
		if (a == 3)
		{
			add(x + 1, y, p4);
			add(x - 1, y, p1);
			add(x, y + 1, p3);
			add(x, y - 1, p2);
		}
		return res;
	}

	void reset()
	{
		cx = start_x;
		cy = start_y;
	}

	void move(int a)
	{
		if (T(cx, cy)) return;
		auto ss = getNewS(cx, cy, a);
		double p = (double)(rand() % 10000) / 10000.0;
		double sum = 0;
		for (auto s : ss)
		{
			sum += s.p;
			if (p < sum)
			{
				cx = s.x;
				cy = s.y;
				return;
			}
		}
		throw "ddsfsdf";
	}
};