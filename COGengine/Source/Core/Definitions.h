#pragma once

#include <iostream>


#define GETCOMPONENT(className) CogGetEntityStorage()->GetComponent<className>(#className)
#define GETBEHAVIOR(className) CogEngine::GetInstance().entityStorage->GetBehaviorPrototype<className>(#className)
#define CREATE_BEHAVIOR(className) CogEngine::GetInstance().entityStorage->GetBehaviorPrototype<className>(#className)->CreatePrototype()
#define REGISTER_COMPONENT(object) CogEngine::GetInstance().entityStorage->RegisterComponent(object)
#define REGISTER_BEHAVIOR(className) CogEngine::GetInstance().entityStorage->RegisterBehaviorPrototype(#className, new className())

#define LUA_REGFUNC(className, funcName) addFunction(#funcName, &className::funcName)
#define LUA_REGDATA(className, dataName) addData(#dataName, &className::dataName)


#define OBJECT(compName) \
public: \
virtual string GetClassName() { \
  return GetClassNameStatic(); \
} \
  \
static string GetClassNameStatic() { \
 return string(#compName); \
}

#define DEFAULT_CONST(compName) \
public: \
  compName(){ \
} 


#define OBJECT_PROTOTYPE(compName) \
OBJECT(compName) \
DEFAULT_CONST(compName) \
compName* CreatePrototype(){ \
  return new compName(); \
}

#define OBJECT_PROTOTYPE_INIT(compName) \
OBJECT_PROTOTYPE(compName) \
compName* CreatePrototype(Setting& setting) { \
  return new compName(setting); \
}

#define BEHAVIOR_MAXCOUNT(count) \
virtual int GetMaxCount() { \
	return count; \
}

#define BEHAVIOR_UNIQUE() BEHAVIOR_MAXCOUNT(1)

// assertion with formatted message
// this macro can be used widely for severe conditions, no matter how difficult
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


// time measurement
#ifdef _DEBUG
#define COGMEASURE_BEGIN(key) \
  do { \
  TimeMeasure::GetInstance().MeasureBlockStart(key); \
  } while(false)
#else
#define COGMEASURE_BEGIN(key) do {} while(false)
#endif

#ifdef _DEBUG
#define COGMEASURE_END(key) \
  do { \
  TimeMeasure::GetInstance().MeasureBlockEnd(key); \
  } while(false)
#else
#define COGMEASURE_END(key) do {} while(false)
#endif


// macro for debug-only logging
#ifdef _DEBUG
#   define COGLOGDEBUG(module, message, ...) \
    do { \
             CogLogDebug(module, message, ##__VA_ARGS__); \
				    } while (false)
#else
#   define COGLOGDEBUG(message, ...) do { } while (false)
#endif

#define PIF 3.141592653f

#ifdef ANDROID
#define SCREEN_TOLERANCE 25
#else
#define SCREEN_TOLERANCE 50
#endif

typedef float(*FadeFunction)(float);

typedef unsigned long long uint64;

namespace Cog {
	typedef unsigned char tBYTE;
	typedef unsigned int tDWORD;
	typedef unsigned short tWORD;
}

#define SIZE_STR(str) sizeof(tBYTE)*str.size()+sizeof(tDWORD)