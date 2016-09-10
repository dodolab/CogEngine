#pragma once

#include "Node.h"
#include "Behavior.h"
#include "Component.h"
#include <typeindex>
#include <typeinfo>

namespace Cog {

	/**
	* Storage for registered entities such as behaviors, components and attributes
	*/
	class EntityStorage {

	protected:
		// components, mapped by their keys
		map<type_index, Component*> components;
		// behaviors, mapped by their keys
		map<StrId, BehaviorCreator*> behaviorBuilders;

	public:

		~EntityStorage() {
			for (auto& key : components) {
				delete key.second;
			}

			for (auto& key : behaviorBuilders) {
				delete key.second;
			}
		}

		/**
		* Gets list of all components
		*/
		vector<Component*> GetAllComponents() {
			
			vector<Component*> output;
			
			for (auto it = components.begin(); it != components.end(); ++it) {
				output.push_back((*it).second);
			}

			return output;
		}

		map<type_index, Component*>& GetComponents() {
			return components;
		}


		/**
		* Adds a new component; or replaces already existing component
		* @param key key of the component
		* @param component reference
		*/
		template<class T>
		void RegisterComponent(T* value) {
			
			if (ExistsComponent<T>()) {
				COGLOGDEBUG("ENTITY_STORAGE", "Warning, attempt to insert already inserted component %s ",typeid(T).name());
				RemoveComponent<T>();
			}

			components[typeid(T)] = value;
		}

		
		/**
		* Adds a new behaviors; or replaces already existing behavior
		* @param key key of the component
		* @param component reference
		*/
		template<class T>
		void RegisterBehaviorBuilder(StrId name) {
			
			if (ExistsBehaviorBuilder(name)) {
				RemoveBehaviorBuilder(name);
			}
			
			static_assert(std::is_default_constructible<T>::value, "All Behavior classes must have default constructor");
			auto builder = new BehaviorCreatorImpl<T>(); //T::creator;
			behaviorBuilders[name] = builder;
		}

		/**
		* Removes existing components (by its key)
		* @return true, if components has been removed
		*/
		template<class T>
		bool RemoveComponent() {
			if (components.count(typeid(T)) != 0) {
				components.erase(typeid(T));
				return true;
			}
			return false;
		}

		/**
		* Removes existing behavior builder
		* @return true, if builder has been removed
		*/
		bool RemoveBehaviorBuilder(StrId key) {
			if (ExistsBehaviorBuilder(key)) {
				behaviorBuilders.erase(key);
				return true;
			}
			return false;
		}

		/**
		* Returns true, if the component is presented
		*/
		template<class T>
		bool ExistsComponent() const {
			return components.count(typeid(T)) != 0;
		}

		/**
		* Returns true, if the behavior is presented
		*/
		bool ExistsBehaviorBuilder(StrId key) const {
			return behaviorBuilders.count(key) != 0;
		}

		/**
		* Gets component by key; call this method only if you are sure that the component exists
		* @param key component key
		*/
		template<class T> T* GetComponent() {
			auto it = components.find(typeid(T));

			COGASSERT(it != components.end(), "ENTITY_STORAGE", "Component %s doesn't exists", typeid(T).name());

			T* attr = static_cast<T*>(it->second);
			return attr;
		}

		Behavior* CreateBehaviorPrototype(StrId key) {
			auto it = behaviorBuilders.find(key);

			COGASSERT(it != behaviorBuilders.end(), "ENTITY_STORAGE", "Behavior prototype %s doesn't exists", StrId::GetStringValue(key).c_str());
			auto builder = it->second;
			return builder->CreateDefault();
		}
	};

}// namespace