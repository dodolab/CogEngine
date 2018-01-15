#pragma once

#include <string>
#include <stdexcept>

namespace Cog {

	using namespace std;

	// =========================== Section for all custom exceptions =============================

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

	/**
	* Exception that occurs when the engine is forced to do an illegal operation
	*/
	class IllegalOperationException : public runtime_error {
	public:
		IllegalOperationException(string msg) : runtime_error(msg.c_str()) {}
	};

}// namespace