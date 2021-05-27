#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>
using namespace std;
enum class Tictactoe
{
	TTT_Empty = '_',
	TTT_X = 'X',
	TTT_O = '0'
};
enum class Progress
{
	Gameplay,
	WON_Human,
	WON_AI,
	Draw
};
#pragma pack(push, 1)
struct Field
{
	Tictactoe **field = nullptr;
	const size_t size = 3;
	size_t turn = 0;
	Tictactoe human = Tictactoe::TTT_Empty;
	Tictactoe ai = Tictactoe::TTT_Empty;
	Progress progress = Progress::Gameplay;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct Coordinates
{
	size_t y;
	size_t x;
};
#pragma pack(pop)
void ClearScr()
{
	system("cls");
}
int32_t getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}
void InitField(Field& f)
{
	f.field = new Tictactoe *[f.size];
	for (size_t y = 0; y < f.size; y++)
	{
		f.field[y] = new Tictactoe[f.size];
	}
	for (size_t y = 0; y < f.size; y++)
	{
		for (size_t x = 0; x < f.size; x++)
		{
			f.field[y][x] = Tictactoe::TTT_Empty;
		}
	}
	if (getRandomNum(0, 1000) > 500)
	{
		f.human = Tictactoe::TTT_X;
		f.ai = Tictactoe::TTT_O;
		f.turn = 0;
	}
	else
	{
		f.human = Tictactoe::TTT_O;
		f.ai = Tictactoe::TTT_X;
		f.turn = 1;
	}
}
void DeinitField(Field& f)
{
	for (size_t y = 0; y < f.size; y++)
	{
		delete[] f.field[y];
	}
	delete[] f.field;
}
void DrawField(const Field& f)
{
	cout << endl << "      ";
	for (size_t x = 0; x < f.size; x++)
	{
		cout << x + 1 << "     ";
	}
	cout << endl;
	for (size_t y = 0; y < f.size; y++)
	{
		cout << " " << y + 1 << " |  ";
		for (size_t x = 0; x < f.size; x++)
		{
			cout << (char)f.field[y][x] << "  |  ";
		}
		cout << endl;
	}
	cout << endl << " Человек: " << (char)f.human << endl << " Компьютер: " << (char)f.ai << endl;
}
Progress checkWin(const Field& f)
{
	for (size_t y = 0; y < f.size; y++)
	{
		if (f.field[y][0] == f.field[y][1] && f.field[y][0] == f.field[y][2])
		{
			if (f.field[y][0] == f.human)
			{
				return Progress::WON_Human;
			}
			else if (f.field[y][0] == f.ai)
			{
				return Progress::WON_AI;
			}
		}
	}
	for (size_t x = 0; x < f.size; x++)
	{
		if (f.field[0][x] == f.field[1][x] && f.field[0][x] == f.field[2][x])
		{
			if (f.field[0][x] == f.human)
			{
				return Progress::WON_Human;
			}
			else if (f.field[0][x] == f.ai)
			{
				return Progress::WON_AI;
			}
		}
	}
	if (f.field[0][0] == f.field[1][1] && f.field[0][0] == f.field[2][2])
	{
		if (f.field[1][1] == f.human)
		{
			return Progress::WON_Human;
		}
		else if (f.field[1][1] == f.ai)
		{
			return Progress::WON_AI;
		}
	}
	if (f.field[2][0] == f.field[1][1] && f.field[2][0] == f.field[0][2])
	{
		if (f.field[1][1] == f.human)
		{
			return Progress::WON_Human;
		}
		else if (f.field[1][1] == f.ai)
		{
			return Progress::WON_AI;
		}
	}
	bool draw = true;
	for (size_t y = 0; y < f.size; y++)
	{
		for (size_t x = 0; x < f.size; x++)
		{
			if (f.field[y][x] == Tictactoe::TTT_Empty)
			{
				draw = false;
				break;
			}
		}
		if (!draw)
		{
			break;
		}
	}
	if (draw)
	{
		return Progress::Draw;
	}
	return Progress::Gameplay;
}
Coordinates GetHumanCoord(const Field& f)
{
	Coordinates c{ 0 };
	cout << endl;
	do
	{
		cout << " Введите координаты по X (1..3): ";
		cin >> c.x;
		cout << " Введите координаты по Y (1..3): ";
		cin >> c.y;
	} while (c.x == 0 || c.y == 0 || c.x > 3 || c.y > 3 || f.field[c.y - 1][c.x - 1] != Tictactoe::TTT_Empty);
	c.y--;
	c.x--;
	return c;
}
Coordinates GetAICoord(const Field& f)
{
	Coordinates c{ 0 };
	if (f.field[1][1] == Tictactoe::TTT_Empty)
	{
		return { 1,1 };
	}
	Coordinates buf[4];
	size_t num = 0;
	if (f.field[0][0] == Tictactoe::TTT_Empty)
	{
		buf[num] = { 0,0 };
		num++;
	}
	if (f.field[2][2] == Tictactoe::TTT_Empty)
	{
		buf[num] = { 2,2 };
		num++;
	}
	if (f.field[0][2] == Tictactoe::TTT_Empty)
	{
		buf[num] = { 0,2 };
		num++;
	}
	if (f.field[2][0] == Tictactoe::TTT_Empty)
	{
		buf[num] = { 2,0 };
		num++;
	}
	if (num != 0)
	{
		const size_t index = getRandomNum(0, 10000) % num;
		return buf[index];
	}
	num = 0;
	if (f.field[0][1] == Tictactoe::TTT_Empty)
	{
		buf[num] = { 0,1 };
		num++;
	}
	if (f.field[2][1] == Tictactoe::TTT_Empty)
	{
		buf[num] = { 2,1 };
		num++;
	}
	if (f.field[1][0] == Tictactoe::TTT_Empty)
	{
		buf[num] = { 1,0 };
		num++;
	}
	if (f.field[1][2] == Tictactoe::TTT_Empty)
	{
		buf[num] = { 1,2 };
		num++;
	}
	if (num != 0)
	{
		const size_t index = getRandomNum(0, 10000) % num;
		return buf[index];
	}
	for (size_t y = 0; y < f.size; y++)
	{
		for (size_t x = 0; x < f.size; x++)
		{
			if (f.field[y][x] == Tictactoe::TTT_Empty)
			{
				f.field[y][x] = f.ai;
				if (checkWin(f) == Progress::WON_AI)
				{
					f.field[y][x] = Tictactoe::TTT_Empty;
					return { y,x };
				}
				f.field[y][x] = Tictactoe::TTT_Empty;
			}
		}
	}
	for (size_t y = 0; y < f.size; y++)
	{
		for (size_t x = 0; x < f.size; x++)
		{
			if (f.field[y][x] == Tictactoe::TTT_Empty)
			{
				f.field[y][x] = f.human;
				if (checkWin(f) == Progress::WON_Human)
				{
					f.field[y][x] = Tictactoe::TTT_Empty;
					return { y,x };
				}
				f.field[y][x] = Tictactoe::TTT_Empty;
			}
		}
	}
	return c;
}
void Celebration(Field& f)
{
	cout << endl;
	if (f.progress == Progress::WON_Human)
	{
		cout << " Человек победил!" << endl;
	}
	else if (f.progress == Progress::WON_AI)
	{
		cout << " Компьютер победил!" << endl;
	}
	else if (f.progress == Progress::Draw)
	{
		cout << " Ничья" << endl;
	}
}
int main()
{
	setlocale(LC_ALL, "Russian");
	Field f;
	InitField(f);
	ClearScr();
	DrawField(f);
	do
	{
		if (f.turn % 2 == 0)
		{
			Coordinates c = GetHumanCoord(f);
			f.field[c.y][c.x] = f.human;
		}
		else
		{
			Coordinates c = GetAICoord(f);
			f.field[c.y][c.x] = f.ai;
		}
		f.turn++;
		ClearScr();
		DrawField(f);
		f.progress = checkWin(f);
	} while (f.progress == Progress::Gameplay);
	Celebration(f);
	DeinitField(f);
	return 0;
}