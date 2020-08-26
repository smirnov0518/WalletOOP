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
	string name;
	date eventDate;
	short importance = 1;
	bool operator == (const date & dat) { return dat.year == eventDate.year && dat.mon == eventDate.mon; }
};
struct posision {
	int x;
	int y;
	short value = 0;
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
		//initEvent();
		SYSTEMTIME stime;
		FILETIME ltime;
		FILETIME ftTimeStamp;
		GetSystemTimeAsFileTime(&ftTimeStamp);
		FileTimeToLocalFileTime(&ftTimeStamp, &ltime);
		FileTimeToSystemTime(&ltime, &stime);
		currentDate = { stime.wMinute,stime.wHour,stime.wDay, stime.wMonth, stime.wYear, stime.wDayOfWeek};
		!stime.wDayOfWeek ? currentDate.weekDay = 7 : currentDate.weekDay = stime.wDayOfWeek;
		currentDate.controlDates();
	}
	~Organizer() { }//exitInitEvent(); }
	int walkByCallend();
	void addNewEvent(const date & dat);
	void deleteEvent();
	void menu();
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

inline int Organizer::walkByCallend()
{
	date toFunction;
	posision arr[31];
	RECT rect;
	HWND hwnd = GetConsoleWindow();
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	cout << "\t\t    Year  " << currentDate.year << "  Mounth " << currentDate.mon << endl << endl;
	cout << "Mon\tTue\tWed\tThu\tFri\tSut\tSun" << endl << endl;
	auto startIt = find(events.begin(), events.end(), currentDate);	// find first element 
	int startInd = startIt-events.begin();
	for (int i = 0; i < currentDate.weekDay; i++) {  // empty spaces for correct callend 
		cout << " \t";
	}
	for (int i = 1; i <= currentDate.daysInMon; i++) {				  // 1 row is for 1 week, thats why make "endl" after evety sunday
		if (currentDate.weekDay % 7 == 0) {
			cout << endl << endl;
			currentDate.weekDay = 0;
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
		currentDate.weekDay++;
	}
	cout << "\n\n"; int cho = 0;
	cin >> cho;
	return cho;
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
			currentDate.weekDay = 7 - ((currentDate.daysInMon - (currentDate.weekDay)) % 7);
			currentDate.prevMon();
			currentDate.controlDates();
			currentDate.weekDay = 7 - ((currentDate.daysInMon - (currentDate.weekDay)) % 7);
			break;
		}
		case 27: {
			system("cls");
			return;
		}
		}
	}
}
