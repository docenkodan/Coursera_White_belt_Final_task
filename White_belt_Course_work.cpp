#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <iomanip>
#include <exception>
using namespace std;

class Date {
public:
	Date (const int& new_year, const int& new_month,
			const int& new_day) {
		year = new_year;
		if (new_month >= 1 && new_month <= 12) {
			month = new_month;
		} else {
			throw logic_error("Month value is invalid: " +
					to_string(new_month));
		}
		if (new_day >= 1 && new_day <= 31) {
				day = new_day;
		} else {
			throw logic_error("Day value is invalid: " +
					to_string(new_day));
		}
	}
	int GetYear() const {
		return year;
	}
	int GetMonth() const {
		return month;
	}
	int GetDay() const {
		return day;
	}
private:
	int year;
	int month;
	int day;
};

Date DateFromString (const string& new_date) {
	istringstream input (new_date);
	int year = 0, month = 0, day = 0;
	char c1, c2;
	input >> year >> c1 >> month >> c2 >> day;
	if (input && c1 == '-' && c2 == '-' && input.eof()) {
		return Date(year, month, day);
	}
	throw logic_error("Wrong date format: " + new_date);
}

ostream& operator<< (ostream& stream, const Date& date){
	stream << setfill('0') << setw(4) << date.GetYear() << "-" <<
			setw(2) << date.GetMonth() << "-" <<
			setw(2) << date.GetDay();
	return stream;
}

bool operator<(const Date& lhs, const Date& rhs) {
	if (lhs.GetYear() != rhs.GetYear())
		return lhs.GetYear() < rhs.GetYear();
	if (lhs.GetMonth() != rhs.GetMonth())
		return lhs.GetMonth() < rhs.GetMonth();
	return lhs.GetDay() < rhs.GetDay();
}

class Database {
public:
	Database(){}
	void AddEvent(const Date& date, const string& event) {
		date_to_events[date].insert(event);
	}
	bool DeleteEvent(const Date& date, const string& event){
		if (date_to_events.count(date) > 0) {
			if (date_to_events[date].count(event) > 0) {
				date_to_events[date].erase(event);
				return true;
			}
		}
		return false;
	}
	int DeleteDate(const Date& date) {
		int n = 0 ;
		if (date_to_events.count(date) > 0) {
			n = date_to_events[date].size();
			date_to_events.erase(date);
		}
		return n;
	}
	set<string> Find(const Date& date) const {
		if (date_to_events.count(date) > 0)
			return date_to_events.at(date);
		else return {};
	}
	void Print() const {
		for (const auto& i : date_to_events) {
			for (const auto& event : i.second) {
				cout << i.first << " " << event << endl;
			}
		}
	}
private:
	map<Date, set<string>> date_to_events;
};

int main() {
	Database db;
	string command_line;
	while (getline(cin, command_line)) {
		try {
			stringstream ss (command_line);
			string command;
			ss >> command;
			if (command == "Add" || command == "Del" || command == "Find") {
				string str_date;
				ss >> str_date;
				Date date = DateFromString(str_date);
				if (command == "Add") {
					string event;
					ss >> event;
					if (event != "")
					{
						db.AddEvent(date, event);
					}
				} else if (command == "Del") {
					if (!ss.eof()) {
						string event;
						ss >> event;
						if (db.DeleteEvent(date, event)) {
							cout << "Deleted successfully" << endl;
						} else {
							cout << "Event not found" << endl;
						}
					} else {
						cout << "Deleted " << db.DeleteDate(date) <<
								" events" << endl;
					}
				} else if (command == "Find") {
					for (const auto& event : db.Find(date)) {
						cout << event << endl;
					}
				}
			} else if (command == "Print") {
				db.Print();
			} else if (!command.empty()){
				throw logic_error ("Unknown command: " + command);
			}
		} catch (exception &ex) {
			cout << ex.what() << endl;
		}
	}
	return 0;
}
