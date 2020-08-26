#include "Wallet.h"
SYSTEMTIME stime;
FILETIME ltime;
FILETIME ftTimeStamp;

void Wallet::initActions()
{
	ifstream in("actions.dat", std::ios::binary);
	if (in.fail())throw "can not open file";
	//in.read(reinterpret_cast<char*>(&incomeSum), sizeof(double));
	//in.read(reinterpret_cast<char*>(&spendSum), sizeof(double));
	int size = 0, stringSize = 0;
	in.read(reinterpret_cast<char*>(&size), sizeof(int));
	actions.resize(size);
	for (size_t i = 0; i < size; i++)
	{
		in.read(reinterpret_cast<char*>(&actions[i].sum), sizeof(double));
		in.read(reinterpret_cast<char*>(&stringSize), sizeof(int));
		actions[i].category.resize(stringSize);
		in.read(const_cast<char*>(actions[i].category.c_str()), stringSize);
		in.read(reinterpret_cast<char*>(&stringSize), sizeof(int));
		actions[i].details.resize(stringSize);
		in.read(const_cast<char*>(actions[i].details.c_str()), stringSize);
		in.read(reinterpret_cast<char*>(&actions[i].incomeSpend), sizeof(bool));
		in.read(reinterpret_cast<char*>(&actions[i].actionDate), sizeof(date));
	}
	in.close();
}

void Wallet::exitInitActions()
{
	ofstream out("actions.dat", std::ios::binary);
	if (out.fail())throw "can not open file";
	//in.write(reinterpret_cast<char*>(&incomeSum), sizeof(double));
	//in.write(reinterpret_cast<char*>(&spendSum), sizeof(double));
	int size = actions.size(),stringSize=0;
	out.write(reinterpret_cast<char*>(&size), sizeof(int));
	for (size_t i = 0; i < size; i++)
	{
		out.write(reinterpret_cast<char*>(&actions[i].sum), sizeof(double));
		stringSize = actions[i].category.size();
		out.write(reinterpret_cast<char*>(&stringSize), sizeof(int));
		out.write(actions[i].category.c_str(),stringSize);

		stringSize = actions[i].details.size();
		out.write(reinterpret_cast<char*>(&stringSize), sizeof(int));
		out.write(actions[i].details.c_str(), stringSize);

		out.write(reinterpret_cast<char*>(&actions[i].incomeSpend), sizeof(bool));
		out.write(reinterpret_cast<char*>(&actions[i].actionDate), sizeof(date));
	}
	out.close();
}

Wallet::Wallet()
{
	initActions();
}

Wallet::~Wallet()
{
	exitInitActions();
	actions.clear();
}

void Wallet::addNewAction(bool mode)
{
	GetSystemTimeAsFileTime(&ftTimeStamp); // including a time classes
	FileTimeToLocalFileTime(&ftTimeStamp, &ltime);
	FileTimeToSystemTime(&ltime, &stime);
	int bufer=0, index = 0;
	double sum = 0;
	int index_selected_category;
	Transaction temp;
	do {
		cout << "Enter sum of transaction: ";  // cheking a wrong value, cuz we gotta devide course on this digit
		if (!std::cin)repairStream();
		std::cin >> sum;
	} while (!std::cin);

	temp.sum = abs(sum / curencyManager);	// curencyManager has overload on casting to double
	temp.category = mode ? incomeCategories.choiseCat(temp.sum) : spendCategories.choiseCat(temp.sum);
	temp.incomeSpend = mode;
	system("cls");
	cout << "Details: ";
	repairStream();
	getline(cin, temp.details);
	temp.actionDate = { stime.wMinute,stime.wHour,stime.wDay,stime.wMonth,stime.wYear };
	actions.push_back(temp);
}

void Wallet::deleting(int index)
{
	if (index < 0 || index >= actions.size())throw "bad index";
	actions.erase(actions.begin() + index);
}

void Wallet::redact(int index)
{
}

void Wallet::menu()
{
}

void Transaction::print(int & count_lines, date & temp, const CurencyStock::Curency & mainCurency ) const
{
	
	if (temp.year != actionDate.year || temp.mon != actionDate.mon || temp.day != actionDate.day)
	{
		SetConsoleTextAttribute(handle, dates_color);
		temp = actionDate;
		gotoxy(10, count_lines++);
		cout << actionDate.year << '.' << actionDate.mon << '.' << actionDate.day;
	}
	gotoxy(0, count_lines);
	SetConsoleTextAttribute(handle, (incomeSpend ? 32 : 64));
	SetConsoleTextAttribute(handle, (incomeSpend ? 10 : 4));
	cout << (actionDate.hour < 10 ? "0" : "");
	cout << actionDate.hour << ':';
	cout << (actionDate.min < 10 ? "0" : "");
	cout << actionDate.min << ' ';
	cout << category << "\t" << double_to_string(sum*mainCurency.course) << "\t" << mainCurency.name;
}
