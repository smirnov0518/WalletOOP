#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Helper.h"
#include <algorithm>
using std::string;		using std::cout;
using std::vector;		using std::fstream;

struct event {
	//HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	string name;
	date eventDate;
	event() = default;
	event(int year, int mon, int day) : eventDate(year,mon,day){}
	short importance = 1;
	bool operator == (const date & dat) { return dat.year == eventDate.year && dat.mon == eventDate.mon && dat.day == eventDate.day; }
	bool operator == (const pair<int,int> & dat) { return dat.first == eventDate.year && dat.second == eventDate.mon; }
	void print()const {
		textPaint(importance);
		cout << (eventDate.hour < 10 ? "0" : "") << eventDate.hour << ":"
			<< (eventDate.min < 10 ? "0" : "") << eventDate.min << "   " << name;
		//WHITE
	}
};
struct position {
	int x;
	int y;
	short value = 0;
	bool operator == (const position & other) {
		return (abs(x - other.x) < 4 && abs(y - other.y) < 2);
	}
};

struct fullDate : public date {

	int weekDay = 0, daysInMon = 30;
	fullDate() = default;
	void controlDates() {
		if (mon == 13) {
			mon = 1;
			year++;
		}
		if (mon == 0) {
			mon = 12;
			year--;
		}
		if (mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12)daysInMon = 31;
		else if ((year % 4 == 0 && !(year % 100 == 0 && year % 400 != 0)) && mon == 2) {
			daysInMon = 29;
		}
		else if (mon == 2)daysInMon = 28;
		else daysInMon = 30;
	}
	void nextMon() {
		++mon;	controlDates();
	}
	void prevMon() {
		--mon;	controlDates();
	}
	void operator = (initializer_list<int>li) {
		min = *li.begin();		hour = *(li.begin()+1);	day = *(li.begin() + 2); 
		mon = *(li.begin() + 3);		year = *(li.begin() + 4);		weekDay = *(li.begin() + 5);
		controlDates();
	}
};

class Organizer
{
private:
	vector<event> events;
	fullDate currentDate;
	void initEvent();
	void exitInitEvent();
public:
	Organizer()  { 
		initEvent();
		SYSTEMTIME stime;
		FILETIME ltime;
		FILETIME ftTimeStamp;
		GetSystemTimeAsFileTime(&ftTimeStamp);
		FileTimeToLocalFileTime(&ftTimeStamp, &ltime);
		FileTimeToSystemTime(&ltime, &stime);
		currentDate = { stime.wMinute,stime.wHour,stime.wDay, stime.wMonth, stime.wYear, stime.wDayOfWeek};
		!stime.wDayOfWeek ? currentDate.weekDay = 7 : currentDate.weekDay = stime.wDayOfWeek;
		currentDate.weekDay = 7 - ((stime.wDay - currentDate.weekDay) % 7);	// here we count at which day of week starts this month (for print)
		currentDate.controlDates();
	}
	~Organizer() { exitInitEvent(); }
	int walkByCallend();
	short addNewEvent(const date & dat);
	void deleteEvent(size_t index);
	void menu();
	int printCallend(position arr[31]);
};

inline void Organizer::initEvent() {
	ifstream in("events.dat", std::ios::binary);
	if (in.fail())throw "can not open file";
	int size=0, stringSize=0;
	in.read(reinterpret_cast<char*>(&size), sizeof(int));
	events.resize(size);
	for (size_t i = 0; i < size; i++)
	{
		in.read(reinterpret_cast<char*>(&stringSize), sizeof(int));
		events[i].name.resize(stringSize);
		in.read(const_cast<char*>(events[i].name.c_str()), stringSize);
		in.read(reinterpret_cast<char*>(&events[i].eventDate), sizeof(date));
		in.read(reinterpret_cast<char*>(&events[i].importance), sizeof(short));
	}
	in.close();
}

inline void Organizer::exitInitEvent()
{
	ofstream out("events.dat", std::ios::binary);
	if (out.fail())throw "can not open file";
	int size = events.size(), stringSize = 0;
	out.write(reinterpret_cast<char*>(&size), sizeof(int));
	for  (size_t i = 0; i < size; i++)
	{
		stringSize = events[i].name.size();
		out.write(reinterpret_cast<char*>(&stringSize), sizeof(int));
		out.write(events[i].name.c_str(), stringSize);
		out.write(reinterpret_cast<char*>(&events[i].eventDate), sizeof(date));
		out.write(reinterpret_cast<char*>(&events[i].importance), sizeof(short));
	}
	out.close();
}
inline int Organizer::printCallend(position arr[31])
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "\t\t    Year  " << currentDate.year << "  Mounth " << currentDate.mon << endl << endl;
	cout << "Mon\tTue\tWed\tThu\tFri\tSut\tSun" << endl << endl;
	auto startIt = find(events.begin(), events.end(), std::pair<int, int>{currentDate.year, currentDate.mon});	// find first element 
	int startInd = startIt - events.begin();
	int weekDay = currentDate.weekDay;
	for (int i = 0; i < weekDay; i++) {  // empty spaces for correct callend 
		cout << " \t";
	}
	for (int i = 1; i <= currentDate.daysInMon; i++) {				  // 1 row is for 1 week, thats why make "endl" after evety sunday
		if (weekDay % 7 == 0) {
			cout << endl << endl;
			weekDay = 0;
		}
		getxy(arr[i - 1].x, arr[i - 1].y);			  // for paintin days in color we need to keep their positions, getxy does it
		if (startInd < events.size() && events[startInd].eventDate == currentDate) {
			if (i >= events[startInd].eventDate.day) {			  // if first record in this month start (for ex) at the end of month we make condition if day that we're
																			// writing biger or equal than day of first record
				for (int j = startInd; events[j].eventDate.mon == currentDate.mon; j++) {       // if it is, start a loop and chek all events in this month after "start"
					if (events[j].eventDate.day == i) {						    // when we found a day that is equal with day that we get to write start another loop
						for (int k = j; events[k].eventDate.day == i; k++) {      // and this loop looks a most important event in this day and put it into days array
							if (events[k].importance > arr[i - 1].value)arr[i - 1].value = events[k].importance;
						}
						break;
					}
				}
			}
		}
		textPaint(arr[i - 1].value);
		cout << i << "\t";
		WHITE
			weekDay++;
	}
	cout << "\n\n";			return weekDay;
}
inline int Organizer::walkByCallend()
{
	position arr[31];
	RECT rect;
	HWND hwnd = GetConsoleWindow();
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	int weekDay = printCallend(arr);
	int cho = 0;

	int pos = 17;				// this variable serves to keep which day was picked by mouse last
	gotopaint(arr[pos].x, arr[pos].y, pos + 1, 1);  // start position when we open calend or come to other month

	bool prev = 0, next = 0, addnew = 0, del = 0;		// bools for deny blinkin
	gotopaint(5, 19, "PREVIOUS", 0);			// buttons for navigate by monthes
	gotopaint(29, 19, "NEXT", 0);

	for (;;) { // event loop
		POINT P;
		GetCursorPos(&P);						// discover position of mouse cursor relatively screen
		GetWindowRect(hwnd, &rect);				// this funtion put the information about console window in descriptor
		P.x = (P.x - rect.left) / 8.19;			// here we take mouse position relatively window and change its coordinates into text coordinates
		P.y = (((P.y - rect.top) - 40) / 13.5);

		// this loop is for walking at callend by mouse and painting days
		// when some day was picked two variables go by days array at the same time and search which day we have picked
		

		if (prev) {  // repainting buttons in common color when we used to pick them
			if ((abs(P.y - 19) > 0) || P.x < 5 || P.x > 14) {
				gotoxy(5, 19);     cout << "PREVIOUS";
				prev = 0;
			}
		}
		if (next) {
			if ((abs(P.y - 19) > 0) || P.x < 28 || P.x > 33) {
				gotoxy(29, 19);    cout << "NEXT";
				next = 0;
			}
		}

		if (abs(P.y - 19) <= 1) {
			if (P.x >= 5 && P.x <= 14) {
				if (prev)goto base;
				if (!next && !prev && !addnew) { gotoxy(arr[pos].x, arr[pos].y);   cout << pos + 1; }
				gotopaint(5, 19, "PREVIOUS", 1);
				prev = 1;
			}
			if (P.x >= 28 && P.x <= 33) {
				if (next)goto base;
				if (!next && !prev && !addnew) { gotoxy(arr[pos].x, arr[pos].y);  cout << pos + 1; }
				gotopaint(29, 19, "NEXT", 1);
				next = 1;
			}

		}
	base:


		if (GetKeyState(VK_LBUTTON) & 0x8000) { // click left mouse button

			Sleep(100);
			if (!prev && !next) {

				gotopaint(60, 3, "ADD NEW", 0);
				gotopaint(72, 3, "DEL", 0);
				int output = 5;		date thisDay(currentDate.year, currentDate.mon, pos + 1);
				auto eventInThisDay = find(events.begin(), events.end(), thisDay);
				if (eventInThisDay != events.end()) {	// if this day contains some events
					while (eventInThisDay->eventDate == thisDay) {
						gotoxy(58, output);
						eventInThisDay->print();
						eventInThisDay++;		output++;
					}
				}
				


				for (;;) {
					if (_kbhit()) {
						char ex = _getch();
						if (ex == 27) {
							for (int k = 0; k < events.size() + 4; k++) { gotoxy(58, k + 3); cout << "\t\t\t\t"; }
							break;
							ex = 0;
						}
					}

					GetCursorPos(&P);
					GetWindowRect(hwnd, &rect);
					P.x = (P.x - rect.left) / 8.19;
					P.y = (((P.y - rect.top) - 40) / 13.5);
					if (P.y == 3) {
						if (P.x >= 58 && P.x <= 68) {
							if (addnew)goto out;	// avoid blinkin
							gotopaint(60, 3, "ADD NEW", 1);
							addnew = 1;
						}
						else if (P.x >= 71 && P.x <= 75) {
							if (del)goto out;
							gotopaint(72, 3, "DEL", 1);
							del = 1;
						}

					}
					if (addnew && (P.x < 58 || P.x >68 || P.y != 3)) { gotopaint(60, 3, "ADD NEW", 0);   addnew = 0; }
					if (del && (P.x < 71 || P.x >75 || P.y != 3)) { gotopaint(72, 3, "DEL", 0);   del = 0; }
				out:
					if (GetKeyState(VK_LBUTTON) & 0x8000) {
						if (addnew) {
							arr[pos].value = addNewEvent(date{ currentDate.year,currentDate.mon,currentDate.day });
							addnew = 0;
							textPaint(arr[pos].value);
							gotoxy(arr[pos].x, arr[pos].y);

							cout << pos + 1;
							WHITE
						}
						else if (del) {
							int result = walkByevents(/*size_event*/0 + 4, 5, 56); ///////////////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!
							if (result >= 0) deleteEvent(0);// (events, &size_event, size_event - 1 - (result - 5));        ///////////////////// HERE TOO
						}
						for (int i = 0; i < 7; i++) { gotoxy(48, i + 22); cout << "\t\t\t\t\t\t\t"; } // clear
						gotoxy(59, 3);
						cout << "\t\t\t";
						for (int k = 0; k < /*size_event*/0 + 4; k++) { gotoxy(55, k + 3); cout << "\t\t\t\t"; }  //clear
						break;
					}

				}
			}
			if(prev || next)currentDate.weekDay = weekDay;	// we change curent.weekDay in case if change month, otherwise callend remains the same
			if (prev)return 0;
			if (next)return 1;

		}

		if (P.x % 8 < 4 && P.y % 2 == 0 && P.x > 0 && P.y > 0) {    // condition for checkin if mouse is within some day in callend
			auto res = find(std::begin(arr), std::end(arr), position{ P.x,P.y });
			if (res - std::begin(arr) == pos || res == std::end(arr))continue;	// now pos keeps previous picked day
			gotopaint(arr[pos].x, arr[pos].y, pos + 1, 0);
			pos = res - std::begin(arr);			// update pos
			gotoxy(arr[pos].x, arr[pos].y);
			cout << pos + 1;///////////////////////////////////////////////////////////////////////////////////
			gotopaint(res->x, res->y, pos + 1, 1);
		}

		char exit = 0;
		if (_kbhit()) {
			exit = _getch();
		}
		if (exit == 27)return 27;
	}
}

inline short Organizer::addNewEvent(const date & dat)
{
	return 0;
}

inline void Organizer::deleteEvent(size_t index)
{
}

inline void Organizer::menu()
{

	//add_event_to_array(events, &size_event, create_event("сюди передати структуру з теперішньою датою")); //визов функції добавленя
	//delete_event(events, &size_event, index - 1);//це видалення (index - 1, бо масив від 0 йде)

	for (int i = 0; i < 30; i++) cout << "\t\t\t\t\t\t\t\t\t\t\t\t\n";  // clear

	int returned = 0;

	currentDate.controlDates();

	for (;;) {
		system("cls");
		gotoxy(65, 1);
		cout << "(ESC) Exit\n";
		returned = walkByCallend();

		switch (returned) {
		case 1: {
			currentDate.nextMon();
			currentDate.controlDates();
			break;
		}
		case 0: {
			currentDate.weekDay = 7 - ((currentDate.daysInMon - currentDate.weekDay) % 7);
			currentDate.prevMon();
			currentDate.controlDates();
			currentDate.weekDay = 7 - ((currentDate.daysInMon - currentDate.weekDay) % 7);
			break;
		}
		case 27: {
			system("cls");
			return;
		}
		}
	}
}


