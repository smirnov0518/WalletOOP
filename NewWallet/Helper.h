#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>
using namespace std;

#define RED SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
#define WHITE SetConsoleTextAttribute(handle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
#define WHITE_BLACK SetConsoleTextAttribute(handle,112)
#define BLUE SetConsoleTextAttribute(handle, 3)
#define GREN SetConsoleTextAttribute(handle, 10)
#define ORANGE SetConsoleTextAttribute(handle, 14)

#define MOVE_CONSOLE_POINTER SetConsoleCursorPosition(handle,{0,0});
inline void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

inline int walkByevents(int maximum, int min, int x) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	RED
		char move = 0, icon = 16;
	int coord = min;
	gotoxy(x, coord);
	cout << icon;
	do {
		move = _getch();
		switch (move) {

		case 72: {
			if (coord - 1 >= min) {
				gotoxy(x, coord);
				cout << ' ';
				coord--;
				gotoxy(x, coord);
				cout << icon;
			}
			break;
		}
		case 80: {
			if (coord + 1 <= maximum) {
				gotoxy(x, coord);
				cout << ' ';
				coord++;
				gotoxy(x, coord);
				cout << icon;
			}
			break;
		}
		}

	} while (move != 13); 
	WHITE
		return coord;
}

inline void repairStream() {
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
}

inline string double_to_string(double sum)
{
	string res;
	res += std::to_string(int(sum * 100) / 100);
	res += '.';
	res += std::to_string(abs(int((sum - (int(sum))) * 100)));
	return res;
}