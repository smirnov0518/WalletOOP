#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Wallet.h"
#include "Helper.h"
using std::cout;	using std::cin;
using std::string;	using std::fstream;
using std::vector;


class CurencyStock {
private:
	void initCurencies();
	void exitInitCurencies();
public:
	CurencyStock() { initCurencies(); }
	~CurencyStock() { exitInitCurencies(); }
	struct Curency {
		string name = "";
		double course = 0;
		void print()const{ cout  << "\t" << name << " (" << course << ")\n"; }
	};
	vector <Curency> courses;
	int mainCurencyIndex = 0;
	const Curency & mainCurse()const { return courses[mainCurencyIndex]; }
	void addNew();
	void deleteCur(int ind);
	void manager();
	operator double() { return courses[mainCurencyIndex].course; }
	friend class Transaction;
};

inline void CurencyStock::initCurencies()
{
	 courses.push_back({ "DOL",1 });	return;
	ifstream in("curencies.dat", std::ios::binary);
	if (in.fail())throw "can not open file";
	int size = 0, stringSize=0;
	in.read(reinterpret_cast<char*>(&size), sizeof(int));
	courses.resize(size);
	in.read(reinterpret_cast<char*>(&mainCurencyIndex), sizeof(int));
	for (size_t i = 0; i < size; i++)
	{
		in.read(reinterpret_cast<char*>(&stringSize), sizeof(int));
		courses[i].name.resize(stringSize);
		in.read(const_cast<char*>(courses[i].name.c_str()), stringSize);
		in.read(reinterpret_cast<char*>(&courses[i].course), sizeof(double));
	}
	in.close();
}

inline void CurencyStock::exitInitCurencies()
{
	ofstream out("curencies.dat", std::ios::binary);
	if (out.fail())throw "can not open file";
	int size = courses.size(),stringSize=0;
	out.write(reinterpret_cast<char*>(&size), sizeof(int));
	out.write(reinterpret_cast<char*>(&mainCurencyIndex), sizeof(int));
	for (size_t i = 0; i < size; i++)
	{
		stringSize = courses[i].name.size();
		out.write(reinterpret_cast<char*>(&stringSize), sizeof(int));
		out.write(courses[i].name.c_str(), stringSize);
		out.write(reinterpret_cast<char*>(&courses[i].course), sizeof(double));
	}
	out.close();
}

inline void CurencyStock::addNew()
{
	Curency temp;
	system("cls");
	do {
		cout << "Enter name curency: ";
		cin >> temp.name;
		cout << "Write a course: 1$ =         " << temp.name << endl;
		gotoxy(21, 1);
		cin >> temp.course;
		if (!cin){	repairStream();		continue;	}
	} while (0);
	courses.push_back(temp);	mainCurencyIndex = courses.size() - 1;
}

inline void CurencyStock::deleteCur(int ind)
{
	if (ind < 0 || ind >= courses.size())	return;
	if (mainCurencyIndex == ind)mainCurencyIndex = 0;		// protect from out of range error
	if (courses.size() > 1) courses.erase(courses.begin() + ind); // we need 1 curency at least, and here we make condition
}

inline void CurencyStock::manager() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	int y = 0, i = 0;
	char choise = 0;

	gotoxy(0, y);
	do {

		gotoxy(0, 0);
		for (i = 0; i <courses.size(); i++) {
			if (i == y)SetConsoleTextAttribute(handle, 18); // paint curency if it is on currently possision
			courses[i].print();
			SetConsoleTextAttribute(handle, 15);   // return to common color
		}
		if (y >= i)SetConsoleTextAttribute(handle, 18); // if cursor is lower than count of all currencies- paint a line in blue
		cout << " Add new curency\n";
		SetConsoleTextAttribute(handle, 15); // return to a common console color

		choise = _getch();  // movin by screen
		switch (choise) {

		case 72: {
			if (y > 0)y--;  // prevent a mivin cursor lower than zero or higher than currecyCount+1 
			gotoxy(0, y);
			break;
		}
		case 80: {
			if (y < i + 1)y++;
			gotoxy(0, y);
			break;
		}
		case 13: {
			if (y <= i - 1) {
				bool reverse = 1;  // we have only two strings -delete/choise, thats why we chanching bool and paint one of strings
				system("cls");
				do {
					gotoxy(0, 0);
					if (reverse)SetConsoleTextAttribute(handle, 18);
					cout << "Choise\n";
					SetConsoleTextAttribute(handle, 15);
					if (!reverse)SetConsoleTextAttribute(handle, 18);
					cout << "Delete currency\n";
					SetConsoleTextAttribute(handle, 15);
					choise = _getch();
					if (choise == 72 || choise == 80)reverse = !reverse;  // if up button or down buttun pressed  we change a bool
				} while (choise != 13);
				choise = 0;
				if (reverse)mainCurencyIndex = y;  //if pressed "choise", mainCurrency takes a pressed currently index of courses
				else deleteCur(y);
			}
			else	addNew();
		}
		}

	} while (choise != 27 && choise != 13); // moving by loop till (ENTER) or (ESC)
	system("cls");
}