#pragma once

#include "ofxCogMain.h"

namespace Cog {



	/**
	* Common class for all components
	*/
	class Component {
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