#pragma once

#include "Node.h"
#include "Behavior.h"
#include "Component.h"

namespace Cog {

	/**
	* Component storage
	*/
	class ComponentStorage {

	protected:
		// components, mapped by their keys
		map<StringHash, Component*> components;
		// map for xml elements and their handlers
		map<string, Component*> xmlHandlers;

	public:

		/**
		* Gets list of all components
		*/
		vector<Component*> GetAllComponents() {
			
			vector<Component*> output = vector<Component*>();
			
			for (auto it = components.begin(); it != components.end(); ++it) {
				output.push_back((*it).second);
			}

			return output;
		}

		/**
		* Adds a new component; or replaces already existing component
		* @param key key of the component
		* @param component reference
		*/
		void RegisterComponent(Component* value) {

			StringHash key = value->GetClassName();

			if (ExistsComponent(key)) {
				RemoveComponent(key);
			}

			components[key] = value;

			// add xml handler
			if (!value->XmlHandlerName().empty()) {
				xmlHandlers[value->XmlHandlerName()] = value;
			}
		}

		/**
		* Removes existing components (by its key)
		* @return true, if components has been removed
		*/
		bool RemoveComponent(StringHash key) {
			map<StringHash, Component*>::iterator it = components.find(key);

			if (it != components.end()) {
				Component* cmp = it->second;
				
				// check xml handler
				if (!cmp->XmlHandlerName().empty()) {
					map<string, Component*>::iterator handIt = xmlHandlers.find(cmp->XmlHandlerName());
					if (handIt != xmlHandlers.end()) {
						xmlHandlers.erase(handIt);
					}
				}

				components.erase(it);
				delete cmp;


				return true;
			}
			return false;
		}

		/**
		* Returns true, if the component is presented
		*/
		bool ExistsComponent(StringHash key) const {
			return components.find(key) != components.end();
		}

		/**
		* Gets component that handles xml element (or null, if there is none)
		*/
		Component* GetXmlHandler(string element) {
			map<string, Component*>::iterator handIt = xmlHandlers.find(element);
			if (handIt != xmlHandlers.end()) return handIt->second;

			else return nullptr;
		}

		/**
		* Gets component by key; call this method only if you are sure that the component exists
		* @param key component key
		*/
		template<class T> T* GetComponent(StringHash key) {
			auto it = components.find(key);

			MASSERT(it != components.end(), "COMPONENT_STORAGE", "Component %d doesn't exists", key);
			MASSERT(typeid(*it->second) == typeid(Component), "COMPONENT_STORAGE", "Component %d is of the wrong type!", key);

			T* attr = static_cast<T*>(it->second);
			return attr;
		}

	};

}// namespace