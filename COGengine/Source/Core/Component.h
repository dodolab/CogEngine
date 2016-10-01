#pragma once

#include "BaseComponent.h"
#include "Definitions.h"
#include "ofxXmlSettings.h"

namespace Cog {

	/**
	* Initialization priority for components
	*/
	enum class InitPriority {
		LOW = 0,	
		MEDIUM = 1,		
		HIGH = 2
	};

	/**
	* Base class for global components
	* Global components are kept in ComponentStorage and exist throughout 
	* the whole application life
	*/
	class Component : public BaseComponent {
	protected:
		InitPriority initPriority = InitPriority::LOW;
	public:

		virtual ~Component() {

		}

		/**
		* Initialization procedure
		* Appropriate for attribute initialization and message subscribing
		* Is called only once
		*/
		virtual void OnInit() {

		}

		/**
		* Gets initialization priority
		*/
		InitPriority GetPriority() const {
			return initPriority;
		}

		/**
		* Updates inner state
		* @param delta delta time since the last loop
		* @param absolute absolute time since the engine was initialized
		*/
		virtual void Update(const uint64 delta, const uint64 absolute) = 0;

	protected:

		/** Sends message to subscribers listening to selected action */
		void SendMessage(StrId action) const;
		/** Sends message with custom context node to subscribers listening to selected action */
		void SendMessage(StrId action, Node* contextNode) const;
		/** Sends message with payload to subscribers listening to selected action */
		void SendMessage(StrId action, spt<MsgPayload> data) const;
		/** Sends message with payload and custom context node to subscribers listening to selected action */
		void SendMessage(StrId action, spt<MsgPayload> data, Node* contextNode) const;
		/** Sends message with payload and custom context node to recipient with selected id */
		void SendMessageToBehavior(StrId action, spt<MsgPayload> data, Node* contextNode, int recipientId) const;
	};

}// namespace