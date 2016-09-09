#pragma once
#include <string>

namespace Cog {

	using namespace std;

	/**
	* Exception that occurs when configuration is wrong
	*/
	class ConfigErrorException : public runtime_error {
	public:
		ConfigErrorException(string msg) : runtime_error(msg.c_str()) {}
	};

	/**
	* Exception that occurs when arguments or parameters are wrong
	*/
	class IllegalArgumentException : public runtime_error {
	public:
		IllegalArgumentException(string msg) : runtime_error(msg.c_str()) {}
	};

}// namespace