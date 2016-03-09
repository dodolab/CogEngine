#pragma once

#include <iostream>


#define GETCOMPONENT(className) CogGetEntityStorage()->GetComponent<className>(#className)
#define GETBEHAVIOR(className) CogEngine::GetInstance().entityStorage->GetBehaviorPrototype<className>(#className)
#define CREATE_BEHAVIOR(className) CogEngine::GetInstance().entityStorage->GetBehaviorPrototype<className>(#className)->CreatePrototype()
#define REGISTER_COMPONENT(object) CogEngine::GetInstance().entityStorage->RegisterComponent(object)
#define REGISTER_BEHAVIOR(className) CogEngine::GetInstance().entityStorage->RegisterBehaviorPrototype(#className, new className())

#define OBJECT_VIRTUAL() \
public: \
virtual string GetClassName() { \
	return string(); \
} 

#define PROTOTYPE_VIRTUAL(compName) \
public: \
virtual compName* CreatePrototype(){ \
 return nullptr; \
}

#define PROTOTYPE_VIRTUAL_INIT(compName) \
PROTOTYPE_VIRTUAL(compName) \
public: \
virtual compName* CreatePrototype(Setting& setting){ \
  return nullptr; \
}

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


// assertion with formatted message
// this macro can be used widely for severe conditions, no matter how difficult
#ifdef DEBUG
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
#ifdef DEBUG
#define COGMEASURE_BEGIN(key) \
  do { \
  TimeMeasure::GetInstance().MeasureBlockStart(key); \
  } while(false)
#else
#define COGMEASURE_BEGIN(key) do {} while(false)
#endif

#ifdef DEBUG
#define COGMEASURE_END(key) \
  do { \
  TimeMeasure::GetInstance().MeasureBlockEnd(key); \
  } while(false)
#else
#define COGMEASURE_END(key) do {} while(false)
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

#define PIF 3.141592653f
#define SCREEN_TOLERANCE 25;

typedef float(*FadeFunction)(float);

typedef unsigned long long uint64;