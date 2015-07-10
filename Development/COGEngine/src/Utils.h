#pragma once

#include "ofMain.h"
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <string>

void COGLogError(string message);
void COGLogInfo(string message);
void COGLogDebug(string message);
void COGLoggerSave();


#ifndef DEBUG
#   define MASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion " #condition " failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
             COGLogError(message); \
			 COGLoggerSave(); \
				        } \
		    } while (false)
#else
#   define MASSERT(condition, message) do { } while (false)
#endif



std::string string_format(const std::string fmt_str, ...);

void spaces(int howMany, std::ostringstream& ss);

typedef unsigned int uint64;

