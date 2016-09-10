#pragma once

#include "Msg.h"
#include "Definitions.h"

namespace Cog {

	class Node;
	class Scene;

	/*! Listener state enumerator */
	enum class ListenerState {
		ACTIVE_ALL,			/*!< active for all events */
		ACTIVE_MESSAGES,	/*!< active for message sending */
		ACTIVE_UPDATES,		/*!< active for update */
		DISABLED			/*!< disabled completely */
	};


	/**
	* Abstract class for all messages listener
	*
	*/
	class MsgListener {
	protected:
		// identifier incremental counter
		static int idCounter;
		// identifier
		const int id;
		// listener state
		ListenerState listState = ListenerState::ACTIVE_ALL;

		/**
		* Creates a new message listener
		*/
		MsgListener();

	public:

		/**
		* Gets running state
		* @return running state
		*/
		const ListenerState GetListenerState() const {
			return listState;
		}

		/**
		* Sets running state
		* @param val value
		*/
		void SetListenerState(ListenerState val) {
			listState = val;
		}

		/**
		* Handler for accepting the message
		* @param Msg received message
		*/
		virtual void OnMessage(Msg& msg) {

		}

		/**
		* Gets element identifier
		* @return incremental value
		*/
		const int GetId() const {
			return id;
		}

		/**
		* Updates components inner state
		* @param delta delta time from the last loop
		* @param absolute absolute time since the application begun
		*/
		virtual void Update(const uint64 delta, const uint64 absolute) = 0;

		bool operator==(int id) {
			return this->id == id;
		}

		bool operator==(const MsgListener& other) {
			return id == other.id;
		}

		bool operator!=(int id) {
			return this->id != id;
		}

		bool operator!=(const MsgListener& other) {
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
		* Registers itself as a action listener
		* @param action1 action to listen to
		*/
		void SubscribeForMessages(Scene* scene, StrId action1);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		*/
		void SubscribeForMessages(Scene* scene, StrId action1, StrId action2);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		*/
		void SubscribeForMessages(Scene* scene, StrId action1, StrId action2, StrId action3);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		* @param action4 action to listen to
		*/
		void SubscribeForMessages(Scene* scene, StrId action1, StrId action2, StrId action3, StrId action4);
	};

}// namespace