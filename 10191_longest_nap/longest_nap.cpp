#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <algorithm>
using namespace std;

string int_to_str(int x) {
    string sx;
    stringstream ss;

    ss << x;
    ss >> sx;

    return sx;
}

int str_to_int (string sx) {
    int x;
    stringstream ss;

    ss << sx;
    ss >> x;

    return x;
}

class Time {
public:
    int hour, minute;
    int operator-(const Time &t2) {
        int delta_hour = this->hour - t2.hour;
        int delta_min = this->minute - t2.minute;

        return 60*delta_hour + delta_min;
    }

    Time () {}
    Time (string s) {
        hour = str_to_int(s.substr(0,2));
        minute = str_to_int(s.substr(3,2));
    }

    string get_string() {
        if (minute < 10) {
            return int_to_str(hour) + ":0" + int_to_str(minute);
        }
        else {
            return int_to_str(hour) + ":" + int_to_str(minute);
        }
    }
};

void magia_aux(vector<string> &appointments, int s, vector<Time> &app_start, vector<Time> &app_end) {
    app_start.clear();
    app_start.resize(s+2);
    app_end.clear();
    app_end.resize(s+2);

    for (int i = 0; i < s; i++) {
        app_start[i+1] = Time(appointments[i].substr(0,5));
        app_end[i+1] = Time(appointments[i].substr(6,5));
    }

    app_end[0] = Time("10:00");
    app_start[s+1] = Time("18:00");
}

bool comp_time(Time t1, Time t2) {
    if (t1.hour != t2.hour) return t1.hour < t2.hour;
    return t1.minute < t2.minute;
}

bool comp_pair(pair<Time,Time> p1, pair<Time,Time> p2) {
    return comp_time(p1.first, p2.first);
}

void magia(vector<string> &appointments, int s, Time &nap_start, Time &nap_end) {
    int nap_time;
    int max_nap_time = 0;
    vector<Time> app_start, app_end;
    vector<pair<Time,Time> > app_aux(s+2);

    magia_aux(appointments, s, app_start, app_end);

    for (int i = 0; i < s+2; i++) {
        app_aux[i].first = app_start[i];
        app_aux[i].second = app_end[i];
    }

    sort(app_aux.begin(), app_aux.end(), comp_pair);

    for (int i = 0; i < s+2; i++) {
        app_start[i] = app_aux[i].first;
        app_end[i] = app_aux[i].second;
    }

    for (int i = 1; i < s+2; i++) {
        nap_time = app_start[i] - app_end[i-1];
        if (max_nap_time < nap_time) {
            max_nap_time = nap_time;
            nap_end = app_start[i];
            nap_start = app_end[i-1];
        }
    }
}

string format(int minutes) {
    if (minutes < 60) {
        return int_to_str(minutes) + " minutes";
    }
    else {
        int hour = minutes / 60;
        minutes = minutes % 60;

        return int_to_str(hour) + " hours and " + int_to_str(minutes) + " minutes";
    }
}

int main () {
    int s, day;
    int nap_minutes;
    string answer;
    Time nap_start, nap_end;
    vector<string> appointments;

    day = 1;
    while (cin >> s) {
        cin.ignore();

        appointments.clear();
        appointments.resize(s);

        for (int i = 0; i < s; i++) {
            getline(cin, appointments[i]);
        }

        magia(appointments, s, nap_start, nap_end);
        nap_minutes = nap_end - nap_start;
        answer = format(nap_minutes);

        cout << "Day #" << day << ": the longest nap starts at " << nap_start.get_string() << " and will last for " << answer << "." << endl;
        day++;
    }

    return 0;
}

