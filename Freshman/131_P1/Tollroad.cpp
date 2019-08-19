#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <vector>

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

//Tollroad Array
int FasTrak_Toll[18][18] = { { 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000 }, // Unknown
						  	 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 102, 231, 525, 525, 525, 525, 525, 525, 000 }, // 405 Frwy
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 102, 231, 525, 525, 525, 525, 525, 525, 000 }, // Fairview Dr.
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 102, 231, 525, 525, 525, 525, 525, 525, 000 }, // Bear St
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 102, 231, 525, 525, 525, 525, 525, 525, 000 }, // 55 Frwy
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000 }, // Bristol St N (NB only)
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 102, 231, 525, 525, 525, 525, 525, 525, 000 }, // Jamboree Rd
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 102, 231, 525, 525, 525, 525, 525, 525, 000 }, // MacArthur Blvd
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 102, 231, 525, 525, 525, 525, 525, 525, 000 }, // Bison Ave
							 { 000, 102, 102, 102, 000, 000, 000, 102, 102, 000, 231, 525, 525, 525, 525, 525, 525, 000 }, // Bonita Canyon Dr
							 { 000, 231, 231, 231, 000, 000, 000, 231, 231, 231, 000, 525, 525, 525, 525, 525, 525, 000 }, // Newport Coast Dr
							 { 000, 525, 525, 525, 000, 000, 000, 525, 525, 525, 525, 000, 000, 257, 257, 257, 257, 000 }, // El Toro Rd/Laguna Cyn Rd
							 { 000, 525, 525, 525, 000, 000, 000, 525, 525, 525, 525, 257, 000, 000, 000, 000, 000, 000 }, // Glenwood/Pacific Park Dr
							 { 000, 525, 525, 525, 000, 000, 000, 525, 525, 525, 525, 257, 000, 000, 195, 195, 195, 000 }, // Aliso Creek Rd
							 { 000, 525, 525, 525, 000, 000, 000, 525, 525, 525, 525, 257, 000, 195, 000, 137, 137, 000 }, // La Paz Rd/Moulton Pkwy
							 { 000, 525, 525, 525, 000, 000, 000, 525, 525, 525, 525, 257, 000, 195, 137, 000, 000, 000 }, // Greenfield Dr
							 { 000, 525, 525, 525, 000, 000, 000, 525, 525, 525, 525, 257, 000, 195, 137, 000, 000, 000 }, // 5 Frway (Mission Viejo) (NB Only)
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000 } };// Unknown (Northbound)

int oneTime_Toll[18][18] = { { 000, 000, 000, 000, 000, 000, 000, 000, 000, 202, 331, 748, 748, 748, 748, 748, 748, 748 }, // Unknown
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 202, 331, 748, 748, 748, 748, 748, 748, 748 }, // 405 Frwy
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 202, 331, 748, 748, 748, 748, 748, 748, 748 }, // Fairview Dr.
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 202, 331, 748, 748, 748, 748, 748, 748, 748 }, // Bear St
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 202, 331, 748, 748, 748, 748, 748, 748, 748 }, // 55 Frwy
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000 }, // Bristol St N (NB only)
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 202, 331, 748, 748, 748, 748, 748, 748, 748 }, // Jamboree Rd
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 202, 331, 748, 748, 748, 748, 748, 748, 748 }, // MacArthur Blvd
							 { 000, 000, 000, 000, 000, 000, 000, 000, 000, 202, 331, 748, 748, 748, 748, 748, 748, 748 }, // Bison Ave
							 { 202, 202, 202, 202, 000, 000, 000, 202, 202, 000, 331, 748, 748, 748, 748, 748, 748, 748 }, // Bonita Canyon Dr
							 { 331, 331, 331, 331, 000, 000, 000, 331, 331, 331, 000, 748, 748, 748 ,748, 748, 748, 748 }, // Newport Coast Dr
							 { 748, 748, 748, 748, 000, 000, 000, 748, 748, 748, 748, 000, 000, 357, 357, 357, 357, 357 }, // El Toro Rd/Laguna Cyn Rd
							 { 748, 748, 748, 748, 000, 000, 000, 748, 748, 748, 748, 357, 000, 000, 000, 000, 000, 000 }, // Glenwood/Pacific Park Dr
							 { 748, 748, 748, 748, 000, 000, 000, 748, 748, 748, 748, 357, 000, 000, 295, 295, 295, 295 }, // Aliso Creek Rd
							 { 748, 748, 748, 748, 000, 000, 000, 748, 748, 748, 748, 357, 000, 295, 000, 237, 237, 237 }, // La Paz Rd/Moulton Pkwy
							 { 748, 748, 748, 748, 000, 000, 000, 748, 748, 748, 748, 357, 000, 295, 237, 000, 000, 000 }, // Greenfield Dr
							 { 748, 748, 748, 748, 000, 000, 000, 748, 748, 748, 748, 357, 000, 295, 237, 000, 000, 000 }, // 5 Frway (Mission Viejo) (NB Only)
							 { 748, 748, 748, 748, 000, 000, 000, 748, 748, 748, 748, 357, 000, 295, 237, 000, 000, 000 } };// Unknown (Northbound)

//Southbound Extra Charge
bool SB_ExtraCharge_Toll[18][18] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Unknown
						  		   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, // 405 Frwy
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, // Fairview Dr.
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, // Bear St
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, // 55 Frwy
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Bristol St N (NB only)
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, // Jamboree Rd
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, // MacArthur Blvd
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, // Bison Ave
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, // Bonita Canyon Dr
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, // Newport Coast Dr
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // El Toro Rd/Laguna Cyn Rd
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Glenwood/Pacific Park Dr
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Aliso Creek Rd
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // La Paz Rd/Moulton Pkwy
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Greenfield Dr
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 5 Frway (Mission Viejo) (NB Only)
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };// Unknown (Northbound)

//Northbound Extra Charge
bool NB_ExtraCharge_Toll[18][18] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Unknown
						  		   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 405 Frwy
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Fairview Dr.
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Bear St
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 55 Frwy
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Bristol St N (NB only)
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Jamboree Rd
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // MacArthur Blvd
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Bison Ave
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Bonita Canyon Dr
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // Newport Coast Dr
								   { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, // El Toro Rd/Laguna Cyn Rd
								   { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, // Glenwood/Pacific Park Dr
								   { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, // Aliso Creek Rd
								   { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, // La Paz Rd/Moulton Pkwy
								   { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, // Greenfield Dr
								   { 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, // 5 Frway (Mission Viejo) (NB Only)
								   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };// Unknown (Northbound)

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

// All onramps on the 73. The enum values are assigned explicit integer values
// in case you want to use them as array indices.
enum Onramp {
	ONRAMP_UNKNOWN_SOUTHBOUND = 0,
	ONRAMP_405 = 1,
	ONRAMP_FAIRVIEW = 2,
	ONRAMP_BEAR = 3,
	ONRAMP_55 = 4,
	ONRAMP_BRISTOL = 5,
	ONRAMP_JAMBOREE = 6,
	ONRAMP_MACARTHUR = 7,
	ONRAMP_BISON = 8,
	ONRAMP_BONITA_CANYON = 9,
	ONRAMP_NEWPORT_COAST = 10,
	ONRAMP_EL_TORO = 11,
	ONRAMP_GLENWOOD = 12,
	ONRAMP_ALISO_CREEK = 13,
	ONRAMP_LA_PAZ = 14,
	ONRAMP_GREENFIELD = 15,
	ONRAMP_5 = 16,
	ONRAMP_UNKNOWN_NORTHBOUND = 17,
};

// All offramps on the 73.
enum Offramp {
	OFFRAMP_UNKNOWN_NORTHBOUND = 0,
	OFFRAMP_405 = 1,
	OFFRAMP_BEAR = 2,
	OFFRAMP_55 = 3,
	OFFRAMP_IRVINE = 4,
	OFFRAMP_JAMBOREE = 5,
	OFFRAMP_UNIVERSITY = 6,
	OFFRAMP_MACARTHUR = 7,
	OFFRAMP_BISON = 8,
	OFFRAMP_BONITA_CANYON = 9,
	OFFRAMP_NEWPORT_COAST = 10,
	OFFRAMP_EL_TORO = 11,
	OFFRAMP_GLENWOOD = 12,
	OFFRAMP_ALISO_CREEK = 13,
	OFFRAMP_LA_PAZ = 14,
	OFFRAMP_GREENFIELD = 15,
	OFFRAMP_5 = 16,
	OFFRAMP_UNKNOWN_SOUTHBOUND = 17,
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
	Timestamp() { }

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

// Return the price of the toll, in units of cents, for a given trip.
//
// When is_FasTrak is true, use FasTrak prices; otherwise use One-Time-Toll prices.
//
// time indicates when the trip happened, which factors into some FasTrak trips
// that happen during rush hour.
//
// start and end indicate the endpoints of the trip.
//
// All trips should be for 2 Axle Vehicles & Motorcycles (page 1 of the price
// matrix PDF).
int calculate_toll(bool is_FasTrak, Timestamp& time, Onramp start, Offramp end) {
	int totalToll = 0;

	//FasTrak Member
	if (true == is_FasTrak) {
		totalToll = FasTrak_Toll[start][end];
	}
	//Non-FasTrak
	else if (false == is_FasTrak) {
		totalToll = oneTime_Toll[start][end];
		return totalToll;
	}

	///*DBG*/cout << "Before Changes: " << totalToll << endl;
	//Southbound Extra Charge
	if (1 == SB_ExtraCharge_Toll[start][end]){
		//Southbound Weekend Reduction
		if (time.day_of_week() == SATURDAY || time.day_of_week() == SUNDAY) {
			if (totalToll != 0) {
				totalToll -= 25;
			}
		}
		//Southbound Weekday Rate Change
		else if (time.day_of_week() == MONDAY || time.day_of_week() == TUESDAY || time.day_of_week() == WEDNESDAY || time.day_of_week() == THURSDAY || time.day_of_week() == FRIDAY) {
			//Southbound Peak
			if ((time.hour() == 17) && (time.minute() >= 0 && time.minute() <= 59))
			{
				if (totalToll != 0) {
					totalToll += 153;
				}
			}
			//Southbound Before & After Peak
			else if ((time.hour() == 15 || time.hour() == 16 || time.hour() == 18) && (time.minute() >= 0 && time.minute() <= 59))
			{
				if (totalToll != 0) {
					totalToll += 123;
				}
			};
		}
	}

	//Northbound Extra Charge
	if (1 == NB_ExtraCharge_Toll[start][end]) {
		//Northbound Weekend Reduction
		if (time.day_of_week() == SATURDAY || time.day_of_week() == SUNDAY) {
			if (totalToll != 0) {
				totalToll -= 25;
			}
		}
		//Northbound Weekday Rate Change
		else if (time.day_of_week() == MONDAY || time.day_of_week() == TUESDAY || time.day_of_week() == WEDNESDAY || time.day_of_week() == THURSDAY || time.day_of_week() == FRIDAY) {
			//Northbound Peak
			if ((time.hour() == 8) && (time.minute() >= 0 && time.minute() <= 59))
			{
				if (totalToll != 0) {
					totalToll += 153;
				}
			}
			//Northbound Before & After Peak
			else if ((time.hour() == 7 || time.hour() == 9) && (time.minute() >= 0 && time.minute() <= 59))
			{
				if (totalToll != 0) {
					totalToll += 123;
				}
			};
		}
	}
	///*DBG*/cout << "After Changes: " << totalToll << endl;
	return totalToll;
}

// A trip recored by a FasTrak transponder inside the car of someone with a
// registered FasTrak account. Each transponder has an integer transponder_id
// which is unique to the transponder device, and is linked to the owner's
// account.
class TransponderTrip {
public:
	TransponderTrip(Timestamp time, Onramp start, Offramp end, int transponder_id) {
		_time = time;
		_start = start;
		_end = end;
		_transponder_id = transponder_id;
	}

	// Accessors.
	Timestamp& time() { return _time; }
	Onramp start() { return _start; }
	Offramp end() { return _end; }
	int transponder_id() { return _transponder_id; }

private:
	Timestamp _time;
	Onramp _start;
	Offramp _end;
	int _transponder_id;
};

// A trip recored by the license plate cameras. This represents a trip taken
// by someone without any FasTrak account.
class LicenseTrip {
public:
	LicenseTrip(Timestamp time, Onramp start, Offramp end, string& plate_number) {
		assert(!plate_number.empty());
		_time = time;
		_start = start;
		_end = end;
		_plate_number = plate_number;
	}

	// Accessors.
	Timestamp& time() { return _time; }
	Onramp start() { return _start; }
	Offramp end() { return _end; }
	string& plate_number() { return _plate_number; }

	// Compute the toll, in cents, of this trip.
	int toll() {
		return calculate_toll(false, _time, _start, _end);
	}

private:
	Timestamp _time;
	Onramp _start;
	Offramp _end;
	string _plate_number;
};

// Represents a FasTrak account. Each account has
//
// - account_number, a unique integer that identifies the account.
//
// - balance, the amount of money owed, in units of cents. A negative
//   balance means that the account holder owes money.
//
// - transponder_id, the integer transponder ID linked to this account.
//
// - plate_number, a string containing the license plate number (strictly
//   speaking not a "number" since license plates may contain letters)
class FasTrakAccount {
public:
	FasTrakAccount(int account_number, int balance, int transponder_id, string& plate_number) {
		assert(!plate_number.empty());
		_account_number = account_number;
		_balance = balance;
		_transponder_id = transponder_id;
		_plate_number = plate_number;
	}

	// Accessors.

	int account_number() { return _account_number; }
	int balance() { return _balance; }
	int transponder_id() { return _transponder_id; }
	string& plate_number() { return _plate_number; }

	// Debit (subtract) a number of cents against this account. cents must be
	// a positive number of cents. balance will decrease by cents.
	void debit(int cents) {
		_balance -= cents;
	}

	// Credit (add) a number of cents toward this account. cents must be a
	// positive number of cents. balance will increase by cents.
	void credit(int cents) {
		_balance += cents;
	}

private:
	int _account_number, _balance, _transponder_id;
	string _plate_number;
};

// Represents a toll violation, meaning that a motorist drove using the
// One-Time-Toll option, but never followed through on paying the toll
// within the 5-day grace period. This becomes a traffic ticket that
// the motorist will have to pay. A Violation is defined only by a
// license plate number and a balance, in units of cents.
class Violation {
public:
	Violation(string& plate_number, int balance) {
		assert(!plate_number.empty());
		assert(balance > 0);
		_plate_number = plate_number;
		_balance = balance;
	}

	// Accessors.
	string& plate_number() { return _plate_number; }
	int balance() { return _balance; }

private:
	string _plate_number;
	int _balance;
};

// The TollAdministrator class manages the entire process of tracking
// accounts, trips, and violations. It owns:
//
// - A list of FasTrakAccount objects, representing registered FasTrak
//   accounts.
//
// - A list of unpaid LicenseTrip objects. When a motorist without a FasTrak
//   account drives through the tolls, their license plate is recorded, and
//   they have up to five days to pay the toll.
//
// - A list of Violation objects. When a LicenseTrip goes 5 days without being
//   paid off, it becomes a violation.
class TollAdministrator {
public:
	// Create a new TollAdministrator, where the account list,
	// unpaid trip list, and violation list are all initially empty.
	TollAdministrator() {
		//done
	}

	// Destructor. This should delete any objects remaining in any
	// of the three lists.
	~TollAdministrator() {
		account_list.clear();
		unpaid_list.clear();
		violations_list.clear();
	}

	// Accessors. Each list has a _count() accessor that returns the
	// number of elements in the list, and an accessor that returns
	// one of the elements of the list by index.

	// Return the number of FasTrak accounts.
	int account_count() {
		return account_list.size();
	}

	// Return the FasTrak account at index i, which must satisfy
	// (i >= 0) && (i < account_count()) .
	FasTrakAccount& account(int i) {
		return account_list.at(i);
	}

	// Return the number of unpaid LicenseTrip objects.
	int unpaid_count() {
		return unpaid_list.size();
	}

	// Return the unpaid LicenseTrip at index i, which must satisfy
	// (i >= 0) && (i < unpaid_count()) .
	LicenseTrip& unpaid(int index) {
		return unpaid_list.at(index);
	}

	// Return the number of Violation objects.
	int violation_count() {
		return violations_list.size();
	}

	// Return the Violation object at index i, which must satisfy
	// (i >= 0) && (i < violation_count()) .
	Violation& violation(int index) {
		return violations_list.at(index);
	}

	// Add a new FasTrak account object. account must not be a nullptr.
	// This increments account_count(). This TollAdministrator class is
	// responsible for deleting the account object in the TollAdministrator
	// destructor.
	void register_account(FasTrakAccount* account) {
		assert(account != nullptr);
		account_list.push_back(*account);
	}

	// Register a TransponderTrip object. trip must not be nullptr.
	// If there is a registered FasTrakAccount whose transponder_id()
	// matches trip's transponder_id(), then debit the toll for the
	// trip from the matching account. If there is no matching account,
	// that means the transponder is obsolete or invalid, so ignore
	// the trip. In either case, this function should delete the trip
	// object.
	void register_transponder_trip(TransponderTrip* trip) {
		assert(trip != nullptr);
		int count = account_count();
		for (int i = 0; i < count; i++) {
			if (account_list[i].transponder_id() == trip->transponder_id()) {
				account_list[i].debit(calculate_toll(true, trip->time(), trip->start(), trip->end()));
				//cout << "Account #" << account_list[i].account_number() << " debited " << calculate_toll(true, trip->time(), trip->start(), trip->end()) << " cents." << endl;
				return;
			}
		}
		delete trip;
	}

	// Register a LicenseTrip object. trip must not be nullptr.
	// This function adds trip to the end of the unpaid list, which
	// increments unpaid_count(). This TripAdministrator class is
	// responsible for eventually deleting the trip object. The trip
	// may get deleted when it is paid off on time; or when the
	// trip transforms into a Violation object; or if neither of those
	// ever happen, the trip must be deleted in TripAdministrator's
	// destructor.
	void register_license_trip(LicenseTrip* trip) {
		assert(trip != nullptr);
		unpaid_list.push_back(*trip);
	}

	// Register a one-time payment. If there are any unpaid LicenseTrip
	// objects whose plate_number() matches plate_number(), and whose
	// toll cost is less than or equal to cents, then the matching
	// unpaid LicenseTrip is considered paid off and should be deleted,
	// decrementing unpaid_count(). Otherwise (there are no matches),
	// this function has no effect.
	void one_time_payment(string& plate_number, int cents) {
		for (unsigned int i = 0; i < unpaid_list.size(); i++) {
			if (unpaid_list[i].plate_number() == plate_number) {
				if (unpaid_list[i].toll() <= cents) {
					unpaid_list.erase(unpaid_list.begin()+ i);
					//cout << "Your trip has been paid sucessfully" << endl;
					return;
				}
				else if (unpaid_list[i].toll() > cents) {
					//cout << "Insufficient payment. Please pay the entire cost of your toll." << endl;
					return;
				}
			}
		}
		//cout << "No unpaid trip with that plate number could be found." << endl;
	}

	// Charge the monthly maintenance fee to all FasTrak accounts.
	// This should debit ACCOUNT_MAINTENANCE_FEE_CENTS from each
	// account.
	void debit_monthly_fees() {
		unsigned int i = 0;
		for (i = 0; i < account_list.size(); i++) {
			account_list[i].debit(ACCOUNT_MAINTENANCE_FEE_CENTS);
		}
		//cout << "Account maintenance fee debited to " << i << " accounts. Should be " << account_count() << "." << endl;
	}

	// Check for unpaid LicenseTrip objects whose grace period has
	// expired, and should be converted to Violation objects. This should
	// check each LicenseTrip to see if its time() is more than GRACE_PERIOD_DAYS
	// in the past, relative to current_time. This is computed based on
	// the unix_time() data member (remember that this is recorded in units of
	// seconds, not days). LicenseTrips that haven't expired yet are left
	// unchanged. For each trip that has expired, the trip is removed from the
	// unpaid list, decrementing unpaid_count(); and a new Violation is added
	// to the violation list, incrementing violation_count().
	void update_violations(Timestamp& current_time) {
		for (unsigned int i = 0; i < unpaid_list.size(); i++) {
			//cout << ((current_time.unix_time() - unpaid_list[i].time().unix_time()) > (GRACE_PERIOD_DAYS * 86400)) << endl;
			if ((current_time.unix_time() - unpaid_list[i].time().unix_time()) >= (GRACE_PERIOD_DAYS * 86400)) {
				//cout << "Grace period expiration detected. Moving trip to Violations." << endl;
				Violation newViolation(unpaid_list[i].plate_number(), (unpaid_list[i].toll() + VIOLATION_PENALTY_CENTS));
				violations_list.push_back(newViolation);
				unpaid_list.erase(unpaid_list.begin() + i);
			}
		}
	}

private:
	vector<FasTrakAccount> account_list;

	vector<LicenseTrip> unpaid_list;

	vector<Violation> violations_list;
	int _count = 0;
};

// This main() function runs through a set of unit tests using the assert(...) macro.
// If all of the above classes are implemented correctly, this program will run
// succesfully. If the classes have a bug, the program will crash with an assertion
// error, which is hopefully helpful.
//
// YOU MUST LEAVE THE DEFINITION OF main() UNCHANGED.
int main()
{
	Timestamp monday(1, MONDAY, 2, 3);
	string plate("123456");

	// Timestamp
	{
		assert(1 == monday.unix_time());
		assert(MONDAY == monday.day_of_week());
		assert(2 == monday.hour());
		assert(3 == monday.minute());
		assert(monday == Timestamp(1, MONDAY, 2, 3));
	}

	// calculate_toll
	{
		Timestamp sat(1, SATURDAY, 9, 0),
			sun(1, SUNDAY, 9, 0),
			sb_peak(1, MONDAY, 17, 1),
			sb_before_peak(1, MONDAY, 15, 1),
			sb_after_peak(1, MONDAY, 18, 1),
			nb_peak(1, MONDAY, 8, 1),
			nb_before_peak(1, MONDAY, 7, 1),
			nb_after_peak(1, MONDAY, 9, 1);

		// diagonal entry
		assert(0 == calculate_toll(false, sat, ONRAMP_MACARTHUR, OFFRAMP_MACARTHUR));

		// other black entry
		assert(0 == calculate_toll(false, sat, ONRAMP_BRISTOL, OFFRAMP_GREENFIELD));

		// crossed-out entry
		assert(0 == calculate_toll(true, sat, ONRAMP_UNKNOWN_SOUTHBOUND, OFFRAMP_UNKNOWN_SOUTHBOUND));
		assert(0 == calculate_toll(true, sat, ONRAMP_UNKNOWN_NORTHBOUND, OFFRAMP_UNKNOWN_NORTHBOUND));

		// trip that is the same price at all times
		assert(102 == calculate_toll(true, sat, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(102 == calculate_toll(true, sun, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(102 == calculate_toll(true, sb_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(102 == calculate_toll(true, sb_before_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(102 == calculate_toll(true, sb_after_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(102 == calculate_toll(true, nb_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(102 == calculate_toll(true, nb_before_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(102 == calculate_toll(true, nb_after_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(202 == calculate_toll(false, sat, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(202 == calculate_toll(false, sun, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(202 == calculate_toll(false, sb_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(202 == calculate_toll(false, sb_before_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(202 == calculate_toll(false, sb_after_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(202 == calculate_toll(false, nb_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(202 == calculate_toll(false, nb_before_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));
		assert(202 == calculate_toll(false, nb_after_peak, ONRAMP_55, OFFRAMP_BONITA_CANYON));

		// northbound variable-price trip
		assert((525 - 25) == calculate_toll(true, sat, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert((525 - 25) == calculate_toll(true, sun, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert((525 + 153) == calculate_toll(true, sb_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert((525 + 123) == calculate_toll(true, sb_before_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert((525 + 123) == calculate_toll(true, sb_after_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(525 == calculate_toll(true, nb_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(525 == calculate_toll(true, nb_before_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(525 == calculate_toll(true, nb_after_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(748 == calculate_toll(false, sat, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(748 == calculate_toll(false, sun, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(748 == calculate_toll(false, sb_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(748 == calculate_toll(false, sb_before_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(748 == calculate_toll(false, sb_after_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(748 == calculate_toll(false, nb_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(748 == calculate_toll(false, nb_before_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));
		assert(748 == calculate_toll(false, nb_after_peak, ONRAMP_FAIRVIEW, OFFRAMP_ALISO_CREEK));

		// southbound variable-price trip
		assert((525 - 25) == calculate_toll(true, sat, ONRAMP_EL_TORO, OFFRAMP_55));
		assert((525 - 25) == calculate_toll(true, sun, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(525 == calculate_toll(true, sb_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(525 == calculate_toll(true, sb_before_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(525 == calculate_toll(true, sb_after_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert((525 + 153) == calculate_toll(true, nb_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert((525 + 123) == calculate_toll(true, nb_before_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert((525 + 123) == calculate_toll(true, nb_after_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(748 == calculate_toll(false, sat, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(748 == calculate_toll(false, sun, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(748 == calculate_toll(false, sb_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(748 == calculate_toll(false, sb_before_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(748 == calculate_toll(false, sb_after_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(748 == calculate_toll(false, nb_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(748 == calculate_toll(false, nb_before_peak, ONRAMP_EL_TORO, OFFRAMP_55));
		assert(748 == calculate_toll(false, nb_after_peak, ONRAMP_EL_TORO, OFFRAMP_55));

		// sample a few other tolls
		assert(357 == calculate_toll(false, sb_peak, ONRAMP_UNKNOWN_NORTHBOUND, OFFRAMP_EL_TORO));
		assert(0 == calculate_toll(true, sb_peak, ONRAMP_FAIRVIEW, OFFRAMP_JAMBOREE));
		assert(202 == calculate_toll(false, sb_peak, ONRAMP_JAMBOREE, OFFRAMP_BONITA_CANYON));
		assert((525 + 153) == calculate_toll(true, sb_peak, ONRAMP_BISON, OFFRAMP_GREENFIELD));
	}

	// TransponderTrip
	{
		TransponderTrip trip(monday, ONRAMP_405, OFFRAMP_5, 123);
		assert(monday == trip.time());
		assert(ONRAMP_405 == trip.start());
		assert(OFFRAMP_5 == trip.end());
		assert(123 == trip.transponder_id());
	}

	// LicenseTrip
	{
		LicenseTrip trip(monday, ONRAMP_405, OFFRAMP_5, plate);
		assert(monday == trip.time());
		assert(ONRAMP_405 == trip.start());
		assert(OFFRAMP_5 == trip.end());
		assert(plate == trip.plate_number());
		assert(748 == trip.toll());
	}

	// FasTrakAccount
	{
		FasTrakAccount account(1, 2, 3, plate);
		assert(1 == account.account_number());
		assert(2 == account.balance());
		assert(3 == account.transponder_id());
		assert(plate == account.plate_number());
		// add $5
		account.credit(500);
		assert(502 == account.balance());
		// remove $3
		account.debit(300);
		assert(202 == account.balance());
	}

	// Violation
	{
		Violation violation(plate, 123);
		assert(plate == violation.plate_number());
		assert(123 == violation.balance());
	}

	// TollAdministrator
	{
		TollAdministrator admin;
		assert(0 == admin.account_count());
		assert(0 == admin.unpaid_count());
		assert(0 == admin.violation_count());

		// register two accounts
		FasTrakAccount* a1 = new FasTrakAccount(1, 0, 123, string("PLT1"));
		FasTrakAccount* a2 = new FasTrakAccount(2, 0, 456, string("PLT2"));
		admin.register_account(a1);
		assert(1 == admin.account_count());
		assert("PLT1" == admin.account(0).plate_number());
		admin.register_account(a2);
		assert(2 == admin.account_count());
		assert("PLT1" == admin.account(0).plate_number());
		assert("PLT2" == admin.account(1).plate_number());

		// transponder trip for a2
		TransponderTrip* t1 = new TransponderTrip(Timestamp(1, TUESDAY, 8, 21), ONRAMP_BEAR, OFFRAMP_GREENFIELD, 456);
		admin.register_transponder_trip(t1);
		assert(2 == admin.account_count());
		assert(0 == admin.unpaid_count());
		assert(0 == admin.violation_count());
		assert(-525 == admin.account(1).balance());

		// transponder trip for a1
		TransponderTrip* t2 = new TransponderTrip(Timestamp(1, WEDNESDAY, 8, 21), ONRAMP_5, OFFRAMP_EL_TORO, 123);
		admin.register_transponder_trip(t2);
		assert(2 == admin.account_count());
		assert(0 == admin.unpaid_count());
		assert(0 == admin.violation_count());
		assert(-257 == admin.account(0).balance());

		// transponder trip for unrecognized transponder ID has no effect
		TransponderTrip* t3 = new TransponderTrip(Timestamp(1, THURSDAY, 8, 21), ONRAMP_5, OFFRAMP_EL_TORO, 789);
		admin.register_transponder_trip(t3);
		assert(2 == admin.account_count());
		assert(0 == admin.unpaid_count());
		assert(0 == admin.violation_count());
		assert(-257 == admin.account(0).balance());
		assert(-525 == admin.account(1).balance());

		// register two license trips
		LicenseTrip* t4 = new LicenseTrip(Timestamp(1, MONDAY, 18, 41), ONRAMP_405, OFFRAMP_5, string("MRBIG"));
		admin.register_license_trip(t4);
		assert(2 == admin.account_count());
		assert(1 == admin.unpaid_count());
		assert(0 == admin.violation_count());
		assert("MRBIG" == admin.unpaid(0).plate_number());
		assert(748 == admin.unpaid(0).toll());
		LicenseTrip* t5 = new LicenseTrip(Timestamp(1, SUNDAY, 11, 42), ONRAMP_405, OFFRAMP_5, string("EWING2"));
		admin.register_license_trip(t5);
		assert(2 == admin.account_count());
		assert(2 == admin.unpaid_count());
		assert(0 == admin.violation_count());
		assert("EWING2" == admin.unpaid(1).plate_number());
		assert(748 == admin.unpaid(1).toll());

		// payment has no effect because it's too small
		admin.one_time_payment(string("MRBIG"), 100);
		assert(2 == admin.account_count());
		assert(2 == admin.unpaid_count());
		assert(0 == admin.violation_count());

		// payment has no effect because the plate doesn't match
		admin.one_time_payment(string("OUTATIME"), 800);
		assert(2 == admin.account_count());
		assert(2 == admin.unpaid_count());
		assert(0 == admin.violation_count());

		// pay second trip
		admin.one_time_payment(string("EWING2"), 748);
		assert(2 == admin.account_count());
		assert(1 == admin.unpaid_count());
		assert(0 == admin.violation_count());
		assert("MRBIG" == admin.unpaid(0).plate_number());

		// advance time a little bit; first trip isn't a violation yet
		admin.update_violations(Timestamp(1000, MONDAY, 12, 12));
		assert(2 == admin.account_count());
		assert(1 == admin.unpaid_count());
		assert(0 == admin.violation_count());

		// advance time more than four days, so the first trip becomes a violation
		admin.update_violations(Timestamp(MILLION, MONDAY, 12, 12));
		assert(2 == admin.account_count());
		assert(0 == admin.unpaid_count());
		assert(1 == admin.violation_count());
		assert("MRBIG" == admin.violation(0).plate_number());
		assert((5750 + 748) == admin.violation(0).balance());

		// monthly fees
		admin.debit_monthly_fees();
		assert(2 == admin.account_count());
		assert(0 == admin.unpaid_count());
		assert(1 == admin.violation_count());
		assert(-457 == admin.account(0).balance());
		assert(-725 == admin.account(1).balance());
	}

	cout << "Success, all tests passed." << endl;
	return 0;
}
