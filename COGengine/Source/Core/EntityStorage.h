#pragma once

#include "Node.h"
#include "Behavior.h"
#include "Component.h"

namespace Cog {

	/**
	* Storage for registered entities such as behaviors, components and attributes
	*/
	class EntityStorage {

	protected:
		// components, mapped by their keys
		map<StringHash, Component*> components;
		// behaviors, mapped by their keys
		map<StringHash, Behavior*> behaviorPrototypes;

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
		* Gets list of all behaviors
		*/
		vector<Behavior*> GetAllBehaviorPrototypes() {

			vector<Behavior*> output = vector<Behavior*>();

			for (auto it = behaviorPrototypes.begin(); it != behaviorPrototypes.end(); ++it) {
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
		}

		/**
		* Adds a new behaviors; or replaces already existing behavior
		* @param key key of the component
		* @param component reference
		*/
		void RegisterBehaviorPrototype(StringHash name, Behavior* prototype) {
			
			if (ExistsBehaviorPrototype(name)) {
				RemoveBehaviorPrototype(name);
			}

			behaviorPrototypes[name] = prototype;
		}

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
		* Removes existing behaviors (by its key)
		* @return true, if behaviors has been removed
		*/
		bool RemoveBehaviorPrototype(StringHash key) {
			map<StringHash, Behavior*>::iterator it = behaviorPrototypes.find(key);

			if (it != behaviorPrototypes.end()) {
				Behavior* beh = it->second;
				behaviorPrototypes.erase(it);
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
		* Returns true, if the behavior is presented
		*/
		bool ExistsBehaviorPrototype(StringHash key) const {
			return behaviorPrototypes.find(key) != behaviorPrototypes.end();
		}

		/**
		* Gets component by key; call this method only if you are sure that the component exists
		* @param key component key
		*/
		template<class T> T* GetComponent(StringHash key) {
			auto it = components.find(key);

			COGASSERT(it != components.end(), "ENTITY_STORAGE", "Component %d doesn't exists", (unsigned)key);

			T* attr = static_cast<T*>(it->second);
			return attr;
		}

		Behavior* GetBehaviorPrototype(StringHash key) {
			auto it = behaviorPrototypes.find(key);

			COGASSERT(it != behaviorPrototypes.end(), "ENTITY_STORAGE", "Behavior prototype %d doesn't exists", (unsigned)key);

			return it->second;
		}

		/**
		* Gets behavior by key; call this method only if you are sure that the component exists
		* @param key behavior key
		*/
		template<class T> T* GetBehaviorPrototype(StringHash key) {
			auto it = behaviorPrototypes.find(key);

			COGASSERT(it != behaviorPrototypes.end(), "ENTITY_STORAGE", "Behavior prototype %d doesn't exists", key);

			T* attr = static_cast<T*>(it->second);
			return attr;
		}
	};

}// namespace