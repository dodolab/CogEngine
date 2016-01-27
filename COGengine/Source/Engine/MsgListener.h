#pragma once

#include "ofxCogCommon.h"
#include "Msg.h"

namespace Cog {

	class Node;
	class Scene;

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

	protected:
		
		/**
		* Registers itself as a global action listener
		* @param action1 action to listen to
		*/
		void RegisterGlobalListening(StringHash action1);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		*/
		void RegisterListening(Scene* scene, StringHash action1);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		*/
		void RegisterListening(Scene* scene, StringHash action1, StringHash action2);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		*/
		void RegisterListening(Scene* scene, StringHash action1, StringHash action2, StringHash action3);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		* @param action4 action to listen to
		*/
		void RegisterListening(Scene* scene, StringHash action1, StringHash action2, StringHash action3, StringHash action4);


		/**
		* Sends a message to any set of behaviors
		* @param bubblingType setting that indicates who should process this message and how it will bubble through scene tree
		* @param action id of action; see Actions namespace for common action ids
		* @param subaction id of subaction; see Actions namespace for common action ids
		* @param data payload
		* @param source source node that is a part of message
		*/
		void SendMessage(BubblingType bubblingType, StringHash action, int subaction, MsgEvent* data, Node* source) const;

		/**
		* Sends a message to any set of behaviors without tree-bubbling
		* @param action id of action; see Actions namespace for common action ids
		* @param subaction id of subaction; see Actions namespace for common action ids
		* @param data payload
		* @param source source node that is a part of message
		*/
		void SendMessageNoBubbling(StringHash action, int subaction, MsgEvent* data, Node* source) const;

		/**
		* Sends a message to one behavior with specific id
		* @param action id of action; see Actions namespace for common action ids
		* @param subaction id of subaction; see Actions namespace for common action ids
		* @param data payload
		* @param source source node that is a part of message
		* @param listenerId id of listener that should get this message
		*/
		void SendDirectMessage(StringHash action, int subaction, MsgEvent* data, Node* source, int listenerId) const;
	};

}// namespace