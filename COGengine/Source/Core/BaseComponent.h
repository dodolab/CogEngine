#pragma once

#include "Msg.h"
#include "Definitions.h"

namespace Cog {

	class Node;
	class Scene;

	/** Listener states */
	enum class ComponentState {
		ACTIVE_ALL,			/** active for all events */
		ACTIVE_MESSAGES,	/** active for message sending */
		ACTIVE_UPDATES,		/** active for update loop */
		DISABLED			/** disabled completely */
	};


	/**
	* Abstract class for all components that can subscribe itself as listeners
	* for messaging systems and can be continuously updated in update loop
	*/
	class BaseComponent {
	protected:
		// incremental counter
		static int idCounter;
		// identifier
		const int id;
		// component state
		ComponentState listState = ComponentState::ACTIVE_ALL;

		BaseComponent();

	public:

		/**
		* Gets component state
		* @return component state
		*/
		const ComponentState GetComponentState() const {
			return listState;
		}

		/**
		* Sets component state
		* @param val value
		*/
		void SetComponentState(ComponentState val) {
			listState = val;
		}

		/**
		* Message handler, accepts a message
		* @param Msg received message
		*/
		virtual void OnMessage(Msg& msg) {

		}

		/**
		* Gets identifier
		* @return incremental value
		*/
		const int GetId() const {
			return id;
		}

		/**
		* Updates inner state 
		* @param delta delta time since the last loop
		* @param absolute absolute time since the engine was initialized
		*/
		virtual void Update(const uint64 delta, const uint64 absolute) = 0;

		bool operator==(int id) {
			return this->id == id;
		}

		bool operator==(const BaseComponent& other) {
			return id == other.id;
		}

		bool operator!=(int id) {
			return this->id != id;
		}

		bool operator!=(const BaseComponent& other) {
			return id != other.id;
		}

	protected:
		
		/**
		* Registers itself as a global action listener
		* @param action1 action to listen to
		*/
		void GlobalSubscribeForMessages(StrId action1);

		/**
		* Registers itself as a global action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		*/
		void GlobalSubscribeForMessages(StrId action1, StrId action2);

		/**
		* Registers itself as a global action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		*/
		void GlobalSubscribeForMessages(StrId action1, StrId action2, StrId action3);

		/**
		* Registers itself as a global action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		* @param action4 action to listen to
		*/
		void GlobalSubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4);

		/**
		* Registers itself as an action listener
		* @param scene scene whose messages will be handled
		* @param action1 action to listen to
		*/
		void SubscribeForMessages(Scene* scene, StrId action1);

		/**
		* Registers itself as an action listener
		* @param scene scene whose messages will be handled
		* @param action1 action to listen to
		* @param action2 action to listen to
		*/
		void SubscribeForMessages(Scene* scene, StrId action1, StrId action2);

		/**
		* Registers itself as an action listener
		* @param scene scene whose messages will be handled
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		*/
		void SubscribeForMessages(Scene* scene, StrId action1, StrId action2, StrId action3);

		/**
		* Registers itself as an action listener
		* @param scene scene whose messages will be handled
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		* @param action4 action to listen to
		*/
		void SubscribeForMessages(Scene* scene, StrId action1, StrId action2, StrId action3, StrId action4);
	};

}// namespace