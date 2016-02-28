#pragma once

#include <DateUtils.h>
#include <ctime>

std::string sweet::DateUtils::getDatetime(){
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	std::stringstream ss;
	ss << (now.tm_year + 1900)
		<< '-'
		<< (now.tm_mon + 1)
		<< '-'
		<< now.tm_mday
		<< '_'
		<< t;
	return ss.str();
}