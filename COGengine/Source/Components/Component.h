#pragma once

#include "ofxCogMain.h"
#include "MsgListener.h"
#include "Facade.h"

namespace Cog {



	/**
	* Common class for all components
	*/
	class Component : public MsgListener {
	private:
		
	public:

		OBJECT_VIRTUAL()

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