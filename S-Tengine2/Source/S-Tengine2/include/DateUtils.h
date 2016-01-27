#pragma once

#include <sstream>
#include <string>

namespace sweet{
class DateUtils{
public:
	// returns the date as a string in the format YYYY-MM-DD_UNIXTIME
	static std::string getDatetime();
};
};