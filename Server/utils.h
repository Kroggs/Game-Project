#pragma once

#include <iostream>
#include <string>
#include <sstream>

std::string int2str(const int x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}