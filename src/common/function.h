#pragma once

#include <string>
#include <sstream>

template <typename T>
std::string toString(const T & v)
{
	std::ostringstream ss;
	ss << v;
	return ss.str();
}