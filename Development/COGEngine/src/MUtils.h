#pragma once

#include "ofMain.h"
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <string>
#include "MFacade.h"


// assertion with formatted message
#ifndef DEBUG
#   define MASSERT(condition, message, ...) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion " #condition " failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << printf(message, ##__VA_ARGS__) << std::endl; \
             COGLogError(message, ##__VA_ARGS__); \
			 COGLoggerFlush(); \
				        } \
		    } while (false)
#else
#   define MASSERT(condition, message) do { } while (false)
#endif

/**
* Creates formatted string
*/
std::string string_format(const std::string fmt_str, ...);

/**
* Adds spaces into string stream
*/
void spaces(int howMany, std::ostream& ss);

typedef unsigned int uint64;

