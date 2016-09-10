#pragma once

#include "ofxCogMain.h"

namespace Cog {

	// assertion with formatted message
#ifdef DEBUG
#   define MASSERT(condition, module, message, ...) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion " #condition " failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << printf(message, ##__VA_ARGS__) << std::endl; \
             CogLogError(module,message, ##__VA_ARGS__); \
			 CogLoggerFlush(); \
				        } \
		    } while (false)
#else
#   define MASSERT(condition, message, ...) do { } while (false)
#endif

// macro for debug-only logging
#ifdef DEBUG
#   define MLOGDEBUG(module, message, ...) \
    do { \
             CogLogDebug(module, message, ##__VA_ARGS__); \
				    } while (false)
#else
#   define MLOGDEBUG(message, ...) do { } while (false)
#endif



/**
* Creates formatted string
*/
	std::string string_format(const char* fmt_str, ...);

	/**
	* Adds spaces into string stream
	*/
	void spaces(int howMany, std::ostream& ss);


} // namespace