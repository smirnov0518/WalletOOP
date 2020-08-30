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
#define GREEN SetConsoleTextAttribute(handle, 10)
#define ORANGE SetConsoleTextAttribute(handle, 14)
#define MOVE_CONSOLE_POINTER SetConsoleCursorPosition(handle,{0,0});

struct date {
	int min = 0;
	int hour = 0;
	int day = 0;
	int mon = 0;
	int year = 0;
	date() = default;
	date(initializer_list<int>li){ *this=li;}
	date(int year, int mon, int day) {
		this->year = year;		this->mon = mon;	this->day = day;
	}
	void operator = (initializer_list<int>li) {
		min = *(li.begin());		hour = *(li.begin() + 1);	day = *(li.begin() + 2);
		mon = *(li.begin() + 3);		year = *(li.begin() + 4);
	}
	bool operator==(const date& objDate)const
	{
		return (objDate.year == year && objDate.mon == mon && objDate.day == day);
	}
};
inline void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
inline void getxy(int & x, int & y)
{

	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
	x = csbiInfo.dwCursorPosition.X;
	y = csbiInfo.dwCursorPosition.Y;
}

inline void textPaint(short importance) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (importance == 1) GREEN;
	else if (importance == 2) ORANGE;
	else if (importance == 3) RED
}
template <typename mytype>
void gotopaint(int a, int b, mytype name, bool paint) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(a, b);
	if (paint)SetConsoleTextAttribute(handle, 64);
	cout << name;
	SetConsoleTextAttribute(handle, 15);
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