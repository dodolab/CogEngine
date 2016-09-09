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

	public:
		/**
		* Removes existing components (by its key)
		* @return true, if components has been removed
		*/
		bool RemoveComponent(StringHash key) {
			map<StringHash, Component*>::iterator it = components.find(key);

			if (it != components.end()) {
				Component* cmp = it->second;
				components.erase(it);
				delete cmp;
				return true;
			}
			return false;
		}

		/**
		* Returns true, if the component is presented
		*/
		bool HasComponent(StringHash key) const {
			return components.find(key) != components.end();
		}

		/**
		* Adds a new component; or replaces already existing component
		* @param key key of the component
		* @param component reference
		*/
		template<class T> void AddComponent(StringHash key, T value) {
			if (HasComponent(key)) {
				RemoveComponent(key);
			}

			components[key] = value;
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