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

		void SendMessage(StrId action, spt<MsgEvent> data, Node* contextNode) const;

		void SendMessage(StrId action, spt<MsgEvent> data) const;

		void SendMessage(StrId action, Node* contextNode) const;

		void SendMessage(StrId action) const;

		void SendMessageToBehavior(StrId action, spt<MsgEvent> data, Node* contextNode, int recipientId) const;
	};

}// namespace