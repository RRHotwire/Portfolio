#include "stdafx.h"

#include <cassert>
#include <iostream>
#include <list>
#include <map>

using namespace std;

// one million
const int MILLION = 1000 * 1000;

// maximum size of internal data structures
const int MAX_ACCOUNTS = 40 * MILLION;
const int MAX_UNPAID = 1 * MILLION;
const int MAX_VIOLATIONS = 2 * MILLION;

// Toll Roads policies
const int ACCOUNT_MAINTENANCE_FEE_CENTS = 200; // $2 in units of cents
const int VIOLATION_PENALTY_CENTS = 5750; // $57.50 in units of cents
const int GRACE_PERIOD_DAYS = 5;

// Days of the week.
enum DayOfWeek {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

// Represents a point in time, including:
// - Unix time (whole seconds since midnight 1/1/1970)
// - day of the week
// - hour (using military time, so this must be 1 through 23)
// - minute (must be 0 through 59)
class Timestamp {
public:
    Timestamp(int unix_time, DayOfWeek day_of_week, int hour, int minute) {
        assert(unix_time >= 0);
        assert((hour >= 0) && (hour <= 23));
        assert((minute >= 0) && (minute <= 59));
        _unix_time = unix_time;
        _day_of_week = day_of_week;
        _hour = hour;
        _minute = minute;
    }

    // Default constructor, leaving all fields uninitialized.
    Timestamp() {

    // Accessors.
    int unix_time() { return _unix_time; }
    DayOfWeek day_of_week() { return _day_of_week; }
    int hour() { return _hour; }
    int minute() { return _minute; }

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
};

