#include "Wallet.h"
SYSTEMTIME stime;
FILETIME ltime;
FILETIME ftTimeStamp;


void Wallet::showTranslations(vector<Transaction>& transaction, COORD coord)
{
	date temp = { 0,0,0,0,0 };
	int count_lines = coord.Y;
	for (auto i = transaction.rbegin(); i != transaction.rend(); ++i)
	{
		if (!(temp == i->actionDate))
		{
			WHITE_BLACK;
			temp = i->actionDate;
			gotoxy(coord.X + 10, count_lines);//позиція дати
			std::cout << i->actionDate.year << '.' << i->actionDate.mon << '.' << i->actionDate.day;
			count_lines++;
		}
		gotoxy(coord.X, count_lines);
		i->incomeSpend ? GREN : RED;
		cout << (i->actionDate.hour < 10 ? "0" : "");
		cout << i->actionDate.hour << ':';
		cout << (i->actionDate.min < 10 ? "0" : "");
		cout << i->actionDate.min << ' ';
		cout << i->category << "\t" << i->sum* curencyManager.mainCurse().course << " " << curencyManager.mainCurse().name;
		count_lines++;
	}
}

void Wallet::showTranslation(Transaction& transaction, COORD coord)
{
	static const int DATE_COLOR = 112;
		WHITE_BLACK;
		gotoxy(coord.X + 10, coord.Y);//позиція дати
		cout << transaction.actionDate.year << '.' << transaction.actionDate.mon << '.' << transaction.actionDate.day;
		transaction.incomeSpend ? GREN : RED;
		gotoxy(coord.X, coord.Y+1);//позиція транзакції
		cout << (transaction.actionDate.hour < 10 ? "0" : "");
		cout << transaction.actionDate.hour << ':';
		cout << (transaction.actionDate.min < 10 ? "0" : "");
		cout << transaction.actionDate.min << ' ';
		cout << transaction.category << "\t" << transaction.sum * curencyManager << " " << '$';
		gotoxy(coord.X, coord.Y + 2);//позиція деталей
		ORANGE;
		cout << "Detail: " << transaction.details;
}

void Wallet::income_and_spend_menu( bool parameter)
{
	vector<Transaction> temp;
SHOW_INCOME_AND_SPEND_MENU: // використовую коли виходжу із іншого меню
	temp.clear();
	int selected_option = -1;
	int counter_x =0;
	for (size_t i = 0; i < actions.size(); i++)
	{
		if (actions[i].incomeSpend== parameter)
		{
			temp.push_back(move(actions[i]));
		}
	}
	BLUE;
	cout << "   Add new transaction";
	showTranslations(temp, { 3,1 });//3 місце для курсора
	show_diagram(incomeCategories);
	RED;//ЗМІНЮЮ КОЛІР ДЛЯ (>) ВКАЗІВНИКА НА ПУНКТ МЕНЮ
	for (;;)
	{
		gotoxy(0, counter_x);
		cout << '>';
		MOVE_CONSOLE_POINTER;
		switch (_getch())
		{
		case 72://up
			if (selected_option > -1)
			{
				gotoxy(0, counter_x);
				cout << ' ';
				if (selected_option == 0)
				{
					counter_x -= 2;
					selected_option--;
					break;
				}
				if (actions[selected_option].actionDate == actions[selected_option - 1].actionDate)
				{
					counter_x--;
				}
				else {
					counter_x -= 2;
				}
					selected_option--;
			}
			break;
		case 80://down
			if (selected_option < int(temp.size()-1))
			{
				gotoxy(0, counter_x);
				cout << ' ';
				if (selected_option==-1)
				{
					counter_x += 2;
					selected_option++;
					break;
				}
				if (actions[selected_option].actionDate == actions[selected_option + 1].actionDate)
				{
					counter_x++;
				}
				else {
					counter_x += 2;
				}
				selected_option++;
			}
			break;
		case 13://(Enter)
		{
			system("cls");
			if (selected_option==-1)
			{
				BLUE;
				addNewAction(parameter);
			}
			else
			{				
				action_manager(temp[selected_option], { 0,0 });
			}
			system("cls");
			goto SHOW_INCOME_AND_SPEND_MENU;
		}
		case 27://ESC
		{
			return;
		}
		}
	}
}

void Wallet::action_manager(Transaction& transaction, COORD coord)
{
	showTranslation(transaction, coord);
	BLUE;
	coord.Y += short(4);//де буде менюшка
	gotoxy(coord.X+3, coord.Y);
	cout << "Redact";
	gotoxy(coord.X+3, coord.Y+1);//де буде менюшка 
	cout << "Delete";
	bool selected_option = false;
	for (;;)
	{
		RED;//ЗМІНЮЮ КОЛІР ДЛЯ (>) ВКАЗІВНИКА НА ПУНКТ МЕНЮ
		gotoxy(coord.X, coord.Y+int(selected_option));
		cout << '>';
		MOVE_CONSOLE_POINTER;
		switch (_getch())
		{
		case 72://up
			if (selected_option ==true)
			{
				gotoxy(coord.X, coord.Y + int(selected_option));
				cout << ' ';
				selected_option=false;
			}
			break;
		case 80://down
			if (selected_option == false)
			{
				gotoxy(coord.X, coord.Y + int(selected_option));
				cout << ' ';
				selected_option=true;
			}
			break;
		case 13://(Enter)
		{
			system("cls");
			BLUE;
				if (selected_option)
				{
					//Delete
					actions.erase(find(actions.begin(), actions.end(), transaction));
				}
				else
				{
					//Redact
				}
			return;
			system("cls");
		}
		case 27://ESC
		{
			system("cls");
			return;
		}
		}
	}

}

void Wallet::transactionsByTime()
{
	Sleep(50);
	SYSTEMTIME stime;
	FILETIME ltime;
	FILETIME ftTimeStamp;
	GetSystemTimeAsFileTime(&ftTimeStamp);
	FileTimeToLocalFileTime(&ftTimeStamp, &ltime);
	FileTimeToSystemTime(&ltime, &stime);
	double income[4] = { 0 };
	double spend[4] = { 0 };
	for (auto i = actions.rbegin(); i != actions.rend(); ++i)
	{
		if (i->actionDate.day == stime.wDay)
		{
			i->incomeSpend ? income[0] += i->sum : spend[0] += i->sum;
		}
		else if (i->actionDate.day >= stime.wDay - (stime.wDayOfWeek == 0 ? 7 : stime.wDayOfWeek))
		{
			i->incomeSpend ? income[1] += i->sum : spend[1] += i->sum;
		}
		else if (i->actionDate.mon == stime.wMonth)
		{
			i->incomeSpend ? income[2] += i->sum : spend[2] += i->sum;
		}
		else if (i->actionDate.year == stime.wYear)
		{
			i->incomeSpend ? income[3] += i->sum : spend[3] += i->sum;
		}
	}
	income[1] += income[0];   income[2] += income[1];   income[3] += income[2];
	spend[1] += spend[0];   spend[2] += spend[1];     spend[3] += spend[2];

	string str[4] = { "today" ,"by year" ,"by week" ," by mounth" };
	static const int color_income[4] = { 6091105, 2944305, 1233175, 958739 };//кольора таблиці доходів
	static const int color_spend[4] = { 9076469, 6180338, 3218670, 2035653 };//кольора таблиці росходів
	show_transactionsByTime({ 250, 90 }, str, income, color_income);
	show_transactionsByTime({ 250, 175 }, str, spend, color_spend);
}

void Wallet::show_transactionsByTime(COORD a, string* str, double* numbers,const int* color)
{
	SetTextColor(hdc, RGB(0, 0, 0));
	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, 0);
	string numbers_str[4];
	for (size_t i = 0; i < 4; i++)
	{
		numbers_str[i] = std::to_string(int(numbers[i]*curencyManager));
	}
	for (int i = 0; i < 4; i++)
	{
		if (i == 2)
		{
			a.Y = a.Y + 40;
			a.X = a.X - 120 * (i - 1);
		}
		else if (i != 0)
		{
			a.X = a.X + 120;
		}
		SelectObject(hdc, CreateSolidBrush(color[i]));
		Rectangle(hdc, a.X, a.Y, a.X + 120, a.Y + 40);
		TextOutA(hdc, a.X + 60, a.Y, str[i].c_str(), str[i].size());
		TextOutA(hdc, a.X + 60, a.Y + 20, numbers_str[i].c_str(), numbers_str[i].size());
		SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 0)));
	}
}

void Wallet::show_diagram(const Category& caterogy)
{
	//	Sleep(50);
	COORD coord_diagram = { 300,70 };
	int size_diagram = 100;
	const int color[15] = { 16776960, 16711680, 16711935, 32768, 128, 65280, 8388608, 32896, 8388736, 255, 8421376, 65535, 6566655, 5963676, 8453888 };
	const COORD COORD_DIAGRAM = { 50,50 };
	const short SIZE_DIAGRAM = 100;
	int RADIUS = 11;//радіус діаграми
	int distance_circle = SIZE_DIAGRAM / 6.5;//distance circle
	double one_degree = 0;
	for (auto it : caterogy.categories)
	{
		if (it.name == "")break;
		one_degree += it.sum;
	}
	Sleep(50);
	one_degree = 360 / one_degree;
	int r = 100 / 2;
	COORD end = { coord_diagram.X + size_diagram,coord_diagram.Y + r };
	COORD center = { coord_diagram.X + r, coord_diagram.Y + r };
	int temp_angle = 0;
	SetTextAlign(hdc, NULL);
	SetBkMode(hdc, NULL);
	SetTextColor(hdc, RGB(255, 255, 255));
	short counter=0;
	for (auto it : caterogy.categories)
	{
		if (it.name == "")break;
		if (it.sum)
		{
			SelectObject(hdc, CreateSolidBrush(color[counter]));
			Ellipse(hdc, coord_diagram.X + size_diagram, coord_diagram.Y + 15 * counter, coord_diagram.X + size_diagram + 10, 10 + coord_diagram.Y + 15 * counter);
			TextOutA(hdc, coord_diagram.X + size_diagram + 10, coord_diagram.Y + 15 * counter, it.name.c_str(), it.name.size());
			temp_angle += (it.sum * one_degree);
			double rad = temp_angle * 3.14 / 180;
			COORD beg = { (r * cos(rad) + center.X),(r * sin(rad) + center.Y) };
			SelectObject(hdc, CreateSolidBrush(color[counter++]));
			Pie(hdc, coord_diagram.X, coord_diagram.Y, coord_diagram.X + size_diagram, coord_diagram.Y + size_diagram, beg.X, beg.Y, end.X, end.Y);
			end = beg;
		}
	}
}

void Wallet::settings()
{
	std::cout << "Settings";
	_getch();
}

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
	// read size from file and resize vector with actions
	// same steps with currency, apart of one thing- we also need to read index of main currency
}

Wallet::~Wallet()
{
	//actions.resize(7);
	exitInitActions();
	//actions.clear();
	// write actions and currencies at file
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
		cout << "Enter sum of transaction: ";  // cheking a wrong value, cuz we gotta devide course on this digit (line 124)
		if (!std::cin)repairStream();
		std::cin >> sum;
	} while (!std::cin);

	temp.sum = sum / curencyManager;
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
	//БАЛАНС
	//ТРАНЗАКЦІЇ
	//СТАТИСТИКА
	//МЕНЮ 
	//ГАРЯЧІ КЛАВІШІ
	static const int QUANTITY_ITEMS_MENU = 3;//КІЛЬКІСТЬ ПУНКТІВ МЕНЮ
	static const string NAMES_MENU_ITEMS[QUANTITY_ITEMS_MENU] = { "Income","Spend","Settings" };// ІМЕНА ПУНКТІВ МЕНЮ
	const COORD COORD_MENU_ITEMS = { 40,0 };
	int selected_option = 0;
SHOW_MAIN_MENU: // використовую коли виходжу із іншого меню
	showTranslations(actions, { 0, 10 });
	transactionsByTime();
	MOVE_CONSOLE_POINTER;
	BLUE;
	for (int i = 0; i < QUANTITY_ITEMS_MENU; i++)
	{
		gotoxy(COORD_MENU_ITEMS.X, COORD_MENU_ITEMS.Y + i);
		cout << NAMES_MENU_ITEMS[i];
	}
	MOVE_CONSOLE_POINTER;
	RED;//ЗМІНЮЮ КОЛІР ДЛЯ (>) ВКАЗІВНИКА НА ПУНКТ МЕНЮ
	for (;;)
	{
		gotoxy( COORD_MENU_ITEMS.X - 1, selected_option );//-1 відстань вказівника від пункту меню
		cout << '>';
		MOVE_CONSOLE_POINTER;
		switch (_getch())
		{
		case 72://up
			if (selected_option > 0)
			{
				gotoxy(COORD_MENU_ITEMS.X - 1, selected_option );
				cout << ' ';
				selected_option--;
			}
			break;
		case 80://down
			if (selected_option < QUANTITY_ITEMS_MENU - 1)
			{
				gotoxy(COORD_MENU_ITEMS.X - 1, selected_option );
				cout << ' ';
				selected_option++;
			}
			break;
		case 13://(Enter)
		{
			system("cls");
			BLUE;
			switch (selected_option)
			{
			case 0:
				income_and_spend_menu(true);//incom
				break;
			case 1:
				income_and_spend_menu(false);//spend
				break;
			case 2:
				settings();
				break;
			}
			system("cls");
			goto SHOW_MAIN_MENU;
		}
		}
	}
}