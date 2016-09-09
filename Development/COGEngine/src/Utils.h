#pragma once

#include "ofMain.h"
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include "MFacade.h"

#ifndef DEBUG
#   define MASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion " #condition " failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
             MFLogError(message); \
			 MFLoggerSave(); \
				        } \
		    } while (false)
#else
#   define MASSERT(condition, message) do { } while (false)
#endif

// todo: toto smazat a dat do fasady MFacade... podivat se do examplu log a pouzit!!

#ifndef DEBUG
#   define LOGINFO(message) \
    do { \
	        MFLogInfo(message); \
            std::cout << message << std::endl; \
     } while (false)
#else
#   define LOGINFO(message) do { } while (false)
#endif

#ifndef DEBUG
#   define LOGDEBUG(message) \
    do { \
	        MFLogDebug(message); \
            std::cout << message << std::endl; \
		     } while (false)
#else
#   define LOGDEBUG(message) do { } while (false)
#endif


std::string string_format(const std::string fmt_str, ...);

void spaces(int howMany, std::ostringstream& ss);

typedef unsigned int uint64;

