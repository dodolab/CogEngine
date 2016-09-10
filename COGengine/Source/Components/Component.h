#pragma once

#include "ofxCogMain.h"

namespace Cog {

#define COMPONENT(compName) \
  public: \
  string GetClassName() { \
      return GetClassNameStatic(); \
  } \
  \
  static string GetClassNameStatic() { \
	  return string(#compName); \
  }

	/**
	* Common class for all components
	*/
	class Component {
	private:
		
	public:

		virtual string GetClassName() {
			return string();
		}

		/**
		* Initializes factory
		*/
		virtual void Init() {

		}

		/**
		* Initializes factory, using xml configuration
		*/
		virtual void Init(spt<ofxXml> config) {
			// call the common init method 
			Init();
		}

	};

}// namespace