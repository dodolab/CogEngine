#pragma once

#include "ofMain.h"
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <string>

void COGLogError(const char* format, ...);
void COGLogInfo(const char* format, ...);
void COGLogDebug(const char* format, ...);
void COGLoggerFlush();


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



std::string string_format(const std::string fmt_str, ...);

void spaces(int howMany, std::ostringstream& ss);

typedef unsigned int uint64;

