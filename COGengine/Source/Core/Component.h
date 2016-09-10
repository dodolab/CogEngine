#pragma once

#include "MsgListener.h"
#include "Definitions.h"
#include "ofxCogCommon.h"

namespace Cog {

	/**
	* Initialization priority
	*/
	enum class InitPriority {
		LOW = 0,
		MEDIUM = 1,
		HIGH = 2
	};

	/**
	* Common class for all components
	*/
	class Component : public MsgListener {
	protected:
		InitPriority initPriority = InitPriority::LOW;
	public:

		virtual ~Component() {

		}

		/**
		* Initializes component
		*/
		virtual void OnInit() {

		}

		/**
		* Initializes component, using xml configuration
		*/
		virtual void OnInit(spt<ofxXml> config) {
			// call the common init method 
			OnInit();
		}

		InitPriority GetPriority() const {
			return initPriority;
		}

		virtual void Update(const uint64 delta, const uint64 absolute) = 0;

	protected:

		/**
		* Sends a message to any set of behaviors
		* @param handlingType setting that indicates who should process this message and how it will bubble through scene tree
		* @param action id of action; see Actions namespace for common action ids
		* @param subaction id of subaction; see Actions namespace for common action ids
		* @param data payload
		* @param source source node that is a part of message
		*/
		void SendMessage(HandlingType handlingType, StrId action, int subaction, MsgEvent* data, Node* source) const;

		/**
		* Sends a message to any set of behaviors without tree-bubbling
		* @param action id of action; see Actions namespace for common action ids
		* @param subaction id of subaction; see Actions namespace for common action ids
		* @param data payload
		* @param source source node that is a part of message
		*/
		void SendMessageToListeners(StrId action, int subaction, MsgEvent* data, Node* source) const;

		/**
		* Sends a message to one behavior with specific id
		* @param action id of action; see Actions namespace for common action ids
		* @param subaction id of subaction; see Actions namespace for common action ids
		* @param data payload
		* @param source source node that is a part of message
		* @param listenerId id of listener that should get this message
		*/
		void SendDirectMessage(StrId action, int subaction, MsgEvent* data, Node* source, int listenerId) const;
	};

}// namespace