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

class Category {
	const string filename;
protected:
	Category(const string & filename) : filename(filename) { initCat(); }
	~Category() { exitInitCat(); 	}
	static const int COUNT_CAT = 15; ////////// ????????????
	struct CategorySum {
		double sum = 0;
		string name = "";
		bool operator == (const CategorySum other) { return name == other.name; }
	};
	vector <CategorySum> categories;
	void printDiagram()const;
	const string & addNewCat(double sum);
	void deleteCat(int index);
	const string & choiseCat(double sum);
	void initCat();
	void exitInitCat();
	friend class Wallet;
};


inline void Category::initCat()
{
	ifstream in(filename, std::ios::binary);
	if (in.fail())throw "can not open file";
	int size = 0 , stringSize=0;
	in.read(reinterpret_cast<char*>(&size), sizeof(int));
	categories.resize(size);
	for (size_t i = 0; i < size; i++)
	{
		in.read(reinterpret_cast<char*>(&stringSize), sizeof(int));
		categories[i].name.resize(stringSize);
		in.read(const_cast<char*>(categories[i].name.c_str()), stringSize);
		in.read(reinterpret_cast<char*>(&categories[i].sum), sizeof(double));
	}
	in.close();
}

inline void Category::exitInitCat()
{
	ofstream out(filename, std::ios::binary);
	if (out.fail())throw "can not open file";
	int size = categories.size(), stringSize=0;
	out.write(reinterpret_cast<char*>(&size), sizeof(int));
	for (size_t i = 0; i < size; i++)
	{
		stringSize = categories[i].name.size();
		out.write(reinterpret_cast<char*>(&stringSize), sizeof(int));
		out.write(categories[i].name.c_str(), stringSize);
		out.write(reinterpret_cast<char*>(&categories[i].sum), sizeof(double));
	}
	out.close();
}



inline const string & Category::addNewCat(double sum){
	system("cls");
	string bufer = "new cat";
	if (sum <= 0)return "invalid action";
	cout << "Enter name of new category: ";
	repairStream();
	getline(cin, bufer, '\n');
	auto found = std::find(categories.begin(), categories.end(), CategorySum{ sum,bufer });
	if (found != categories.end()) {
		found->sum += sum; return found->name;
	}
	if (categories.size() < COUNT_CAT)categories.push_back(CategorySum{ sum,bufer });
	return categories.back().name;
}

inline void Category::deleteCat(int index)
{
	if (categories.size() > index && index >=0) {
		if(categories.size()>1)categories.erase(categories.begin() + index);
	}
}

inline const string & Category::choiseCat(double sum)
{
	system("cls");
	if (sum <= 0)return "invalid action";
	int bufer = 0, y = 1, i = 0;
	char choise = 0;
	cout << "  choise the category" << std::endl;
	for (i = 0; i < categories.size(); i++) {   //  show all categories
		cout << "  " << i + 1 << " " << categories[i].name << std::endl;
	}
	cout << "\n  Add new category\n";
	int result = walkByevents(i + 3, 1, 0);
	if (result < i + 2) {
		categories[result - 1].sum += sum;
		return categories[result - 1].name;
	}
	else addNewCat(sum);
}


