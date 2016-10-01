#pragma once

#include "DEntity.h"
#include "Resources.h"

namespace Cog {

	/**
	* Behavior entity; references behavior of given type
	* Is used for loading from XML where behaviors are declared
	*/
	class BehaviorEnt : public DEntity{
	public:

		BehaviorEnt() {

		}

		/**
		* Creates a new behavior entity
		* @param ref reference to existing behavior entity stored in Resources
		* @param setting behavior key-value settings
		*/
		BehaviorEnt(string ref, Setting setting) : setting(setting){
			this->name = ref;
			this->ref = ref;
		}

		/**
		* Creates a new behavior entity
		* @param name name of the entity
		* @param type behavior type (class name)
		* @param setting behavior key-value settings
		*/
		BehaviorEnt(string name, string type, Setting setting) : setting(setting), type(type) {
			this->name = name;
		}

		~BehaviorEnt() {

		}

		// reference to existing behavior entity
		// this attribute can be used when a behavior with complex settings is 
		// declared once in xml with unique name. This name is used as a reference for
		// other behavior entities
		string ref;
		// behavior type (class name, e.g. MultiAnim)
		string type;
		// key-value settings (some behaviors have Load method that accepts Setting object)
		Setting setting;

		/**
		* Loads entity from xml
		*/
		void LoadFromXml(spt<ofxXml> xml, Setting& set);

	};
}// namespace


