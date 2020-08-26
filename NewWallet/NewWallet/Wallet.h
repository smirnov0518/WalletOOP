#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include "CategoryStuck.h"
#include <algorithm>
#include "Helper.h"
#include <fstream>
#include "CurencyStock.h"


using std::string;		using std::vector;
using std::cout;		using std::fstream;
const HDC hdc = GetDC(GetConsoleWindow());
const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);


class Transaction {
public:
	void operator = (const Transaction & other){} ////////////////////// ?????????????? deleted function
	const int dates_color = 112;
	double sum=0;
	string category="";
	string details="";
	bool incomeSpend = 1;
	date actionDate{};
	void print(int & count_lines, date & temp, const CurencyStock::Curency & mainCurency = { "dol" })const;
};


class Wallet
{
private:
	vector<Transaction> actions;
	Category incomeCategories{ "incomeCat.dat" };
	Category spendCategories{"spendCat.dat"};
public:
	CurencyStock curencyManager;
	double incomeSum = 0, spendSum=0;
	void initActions();
	void exitInitActions();
	Wallet();
	~Wallet();
	void addNewAction(bool mode=1);
	void deleting(int index);
	void redact(int);
	void printall()const {
		int countLines = 12;  // const int transaction_main_x = 12 in <Wallet_Part1>
		date temp = { 0,0,0,0,0 };
		for (auto & it : actions) {
			it.print(countLines,temp, curencyManager.mainCurse());	++countLines;
		}
		cout << "\n\n";
		SetConsoleTextAttribute(handle, 15);
	}
	void menu();	// calls other methods
};

