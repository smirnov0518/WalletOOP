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



struct date {
	int min=0;
	int hour=0;
	int day=0;
	int mon=0;
	int year=0;
	void operator = (initializer_list<int>li[5]) {
		min = *li->begin();		hour = *li->begin() + 1;	day = *li->begin() + 2;
		mon = *li->begin() + 3;		year = *li->begin() + 4;
	}
	bool operator==(const date& objDate)const
	{
		return (objDate.year == year && objDate.mon == mon && objDate.day == day);
	}
};

class Transaction {
public:
	double sum=0;
	string category="";
	string details="";
	bool incomeSpend = 1;
	date actionDate{};
	 Transaction()=default;
	 Transaction(const Transaction& tran)
	{
	this->sum = tran.sum;
	this->category = tran.category;
	this->details = tran.details;
	this->incomeSpend = tran.incomeSpend;
	this->actionDate = tran.actionDate;
	}
//	void print(int & count_lines, date & temp, const CurencyStock::Curency & mainCurency = { "dol" })const;
	Transaction& operator=(Transaction&& tran)noexcept
	{
		this->sum = tran.sum;
		this->category = tran.category;
		this->details = tran.details;
		this->incomeSpend = tran.incomeSpend;
		this->actionDate = tran.actionDate;
		return *this;
	}
	inline bool operator==(const Transaction& right)
	{
		if (sum == right.sum &&  category == right.category &&  details == right.details &&  incomeSpend == right.incomeSpend &&  actionDate == right.actionDate)
		{
			return true;
		}
		return false;
	}

};


class Wallet
{
private:
	vector<Transaction> actions;
	Category incomeCategories{ "incomeCat.dat" };
	Category spendCategories{"spendCat.dat"};
	void showTranslations(vector<Transaction>& transaction, COORD coord);
	void showTranslation(Transaction& transaction, COORD coord);
	void income_and_spend_menu(bool parameter);
	void action_manager(Transaction& transaction, COORD coord);
	void transactionsByTime();
	void show_transactionsByTime(COORD a, string* str, double* numbers, const int* color);
	void show_diagram(const Category& caterogy);
	void settings();
public:
	CurencyStock curencyManager;
	double incomeSum = 0, spendSum=0;
	void initActions();
	void exitInitActions();
	Wallet();	// can be bug in init with strings
	~Wallet();
	void addNewAction(bool mode=true);
	void deleting(int index);
	void redact(int);
	void menu();	// calls other methods
};

