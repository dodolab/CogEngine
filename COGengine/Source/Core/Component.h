#pragma once

#include "ofxCogCommon.h"
#include "MsgListener.h"


namespace Cog {

	/**
	* Initialization priority
	*/
	enum class InitPriority {
		LOW = 0,
		MEDIUM = 1,
		HIGH = 2
	};

	/**
	* Common class for all components
	*/
	class Component : public MsgListener {
	protected:
		InitPriority initPriority = InitPriority::LOW;
	public:

		OBJECT_VIRTUAL()

		virtual ~Component() {

		}

		/**
		* Initializes factory
		*/
		virtual void Init() {

		}

		InitPriority GetPriority() const {
			return initPriority;
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