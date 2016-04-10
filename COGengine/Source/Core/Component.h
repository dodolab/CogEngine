#pragma once

#include "MsgListener.h"
#include "Definitions.h"
#include "ofxCogCommon.h"

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

		virtual ~Component() {

		}

		/**
		* Initializes component
		*/
		virtual void Init() {

		}

		InitPriority GetPriority() const {
			return initPriority;
		}

		/**
		* Initializes component, using xml configuration
		*/
		virtual void Init(spt<ofxXml> config) {
			// call the common init method 
			Init();
		}

		virtual void Update(const uint64 delta, const uint64 absolute) = 0;
	};

}// namespace