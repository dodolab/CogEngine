#pragma once

#include <iostream>
#include <memory>
#include "TimeMeasure.h"
#include "Error.h"

// ==========================================================================
// This section contains definitions of all macros, default constants and
// aliases, used in ofxCogEngine


/** Gets component by its type */
#define GETCOMPONENT(className) CogGetComponentStorage()->GetComponent<className>()

/** Registers an instance of a component*/
#define REGISTER_COMPONENT(object) ofxCogEngine::GetInstance().compStorage->RegisterComponent(object)

/** Registers beahvior prototype by its type */
#define REGISTER_BEHAVIOR(className) ofxCogEngine::GetInstance().compStorage->RegisterBehaviorBuilder<className>(#className)

/** Registers lua function */
#define LUA_REGFUNC(className, funcName) addFunction(#funcName, &className::funcName)

/** Registers lua member property */
#define LUA_REGDATA(className, dataName) addData(#dataName, &className::dataName)



/** Assertion with formatted message, may be used widely  
* for time-consuming assertions since it is only for debug mode
*/
#ifdef _DEBUG
#   define COGASSERT(condition, module, message, ...) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion " #condition " failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << printf(message, ##__VA_ARGS__) << std::endl; \
             CogLogError(module,message, ##__VA_ARGS__); \
			 CogLoggerFlush(); \
				        } \
		    } while (false)
#else
#   define COGASSERT(condition, message, ...) do { } while (false)
#endif

/** Macro for declaring time measure of the beginning of a block of code */
#ifdef _DEBUG
#define COGMEASURE_BEGIN(key) \
  do { \
  TimeMeasure::GetInstance().MeasureBlockStart(key); \
  } while(false)
#else
#define COGMEASURE_BEGIN(key) do {} while(false)
#endif


/** Macro for declaring time measure of the end of a block of code */
#ifdef _DEBUG
#define COGMEASURE_END(key) \
  do { \
  TimeMeasure::GetInstance().MeasureBlockEnd(key); \
  } while(false)
#else
#define COGMEASURE_END(key) do {} while(false)
#endif


/** Macro for debug-only logging */
#ifdef _DEBUG
#   define COGLOGDEBUG(module, message, ...) \
    do { \
             CogLogDebug(module, message, ##__VA_ARGS__); \
				    } while (false)
#else
#   define COGLOGDEBUG(message, ...) do { } while (false)
#endif

// tolerance to distinguish between click and move
#ifdef ANDROID
#define TOUCHMOVE_TOLERANCE 25
#else
#define TOUCHMOVE_TOLERANCE 50
#endif

// tolerance to distinguish between double-click and two clicks in different area
#ifdef ANDROID
#define TOUCHPOINT_TOLERANCE 80
#else
#define TOUCHPOINT_TOLERANCE 12
#endif

// float PI number
#define PIF 3.141592653f
// default app speed
#define DEFAULT_APP_SPEED 60

// alias for fade function
typedef float(*FadeFunction)(float);

// 64bit integer for timing
typedef unsigned long long uint64;

// alias for shared pointer
template<typename T>
using spt = std::shared_ptr<T>;

// aliases for data types for networking
namespace Cog {
	typedef unsigned char tBYTE;
	typedef unsigned int tDWORD;
	typedef unsigned short tWORD;
}

// macro for calculating size of a string used in networking
// when a string is seralized, the size of a string goes with it
#define SIZE_STR(str) sizeof(tBYTE)*str.size()+sizeof(tDWORD)