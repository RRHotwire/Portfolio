#include "stdafx.h"

#include <cassert>
#include <iostream>
#include <list>
#include <map>

using namespace std;

// This should not matter: {
const int MILLION = 1000 * 1000;

class Timestamp {

    // Accessors.
    void printString() { cout << "Does not matter: [12 " << endl;}

    // Comparison operator. This is needed by some of the unit tests in main().
    bool operator==(const Timestamp& right) const {
        return ((_day_of_week == right._day_of_week) &&
            (_unix_time == right._unix_time) &&
            (_hour == right._hour) &&
            (_minute == right._minute));
    }

private:
    DayOfWeek _day_of_week;
    int _unix_time, _hour, _minute;
    string s;
};

