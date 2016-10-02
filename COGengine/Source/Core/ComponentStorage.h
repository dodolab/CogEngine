#pragma once

#include "Node.h"
#include "Behavior.h"
#include "BehaviorCreator.h"
#include "Component.h"
#include <typeindex>
#include <typeinfo>

namespace Cog {

	/**
	* Storage for registered behavior prototypes and component instances
	*/
	class ComponentStorage {

	protected:
		// components, mapped by their types
		map<type_index, Component*> components;
		// behaviors, mapped by their hashed names
		map<StrId, BehaviorCreator*> behaviorBuilders;

	public:

		ComponentStorage() {

		}

		~ComponentStorage() {
			// delete content
			for (auto& key : components) {
				delete key.second;
			}
		}

		/**
		* Gets list of all components
		*/
		void GetAllComponents(vector<Component*>& output) {
			for (auto it = components.begin(); it != components.end(); ++it) {
				output.push_back((*it).second);
			}
		}

		/** 
		* Gets link to map of all components
		*/
		map<type_index, Component*>& GetComponents() {
			return components;
		}

		/**
		* Eitehr adds a new component or replaces already existing component
		* @param value instance of the component
		* @tparam type of the component
		*/
		template<class T>
		void RegisterComponent(T* value) {
			
			if (ExistsComponent<T>()) {
				COGLOGDEBUG("ComponentStorage", "Warning, attempt to insert already inserted component %s ",typeid(T).name());
				RemoveComponent<T>();
			}

			components[typeid(T)] = value;
		}


		/**
		* Removes existing components by its type
		* @tparam type of the component
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
		* Returns true, if the component exists in the collection
		* @tparam type of the component
		*/
		template<class T>
		bool ExistsComponent() const {
			return components.count(typeid(T)) != 0;
		}

		/**
		* Gets component by its type
		* @tparam type of the component
		*/
		template<class T> T* GetComponent() {
			auto it = components.find(typeid(T));

			COGASSERT(it != components.end(), "ComponentStorage", "Component %s doesn't exists", typeid(T).name());

			T* attr = static_cast<T*>(it->second);
			return attr;
		}

		Component* GetComponentById(int id) {
			for (auto& cmp : components) {
				if (cmp.second->GetId() == id) return cmp.second;
			}
			return nullptr;
		}

		/**
		* Either adds a new behavior prototype builder or replaces already existing
		* @param name name of the behavior
		* @tparam type of the behavior
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
		* Removes existing behavior builder by its name
		* @param key hashed name of the type of the behavior
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
		* Returns true, if the behavior builder exists in the collection
		* @param key hashed name of the behavior
		*/
		bool ExistsBehaviorBuilder(StrId key) const {
			return behaviorBuilders.count(key) != 0;
		}

		/**
		* Creates a new behavior using the prototype builder
		*/
		Behavior* CreateBehaviorPrototype(StrId key);
	};

}// namespace