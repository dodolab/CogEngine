#pragma once

#include "BehaviorCreator.h"
#include "ofxCogCommon.h"
#include "Msg.h"
#include "Facade.h"
#include "MsgListener.h"
#include "Settings.h"


namespace Cog {

	class Node;

	/**
	* Abstract class for all behaviors
	*
	*/
	class Behavior : public MsgListener {
	public: 


		virtual void Load(Setting& setting) {

		}

	protected:
		// owner node
		Node* owner;
		// indicator if this behavior has ended
		bool finished = false;
		// indicator, if this behavior has been initialized
		bool initialized = false;
		bool isExternal = false;
		bool started = false;
		bool removeWhenFinish = false;

		/**
		* Creates a new behavior
		*/
		Behavior();


	public:

		virtual ~Behavior()
		{

		}

		/**
		* Initialization procedure;
		* Any attribute that is owned only by specific behavior should
		* be created here
		*/
		void Init() {
			if (!initialized) {
				this->finished = false;
				this->OnInit();
				this->initialized = true;
			}
		}

		void Start() {
			this->finished = false;
			this->OnStart();
			this->started = true;
		}


		/**
		* Finishes the behavior
		*/
		void Finish();
		/**
		* Gets the owner node
		*/
		Node* GetOwner() const {
			return owner;
		}


		/**
		* Gets indicator, if this behavior has been initialized
		*/
		bool IsInitialized() {
			return initialized;
		}

		bool IsExternal() {
			return isExternal;
		}

		/**
		* Returns true, if the behavior has ended
		*/
		bool IsFinished() {
			return finished;
		}

		bool IsStarted() {
			return started;
		}
	
		void SetIsExternal(bool ext) {
			this->isExternal = ext;
		}

		bool RemoveWhenFinish() {
			return this->removeWhenFinish;
		}

		void SetRemoveWhenFinish(bool remove) {
			this->removeWhenFinish = remove;
		}

		// allow to access Node private members
		friend class Node;

	protected:
	
		virtual void OnInit() {

		}

		virtual void OnStart() {

		}

		virtual void OnFinish() {

		}

		void SubscribeForMessages(StrId action1);
		void SubscribeForMessages(StrId action1, StrId action2);
		void SubscribeForMessages(StrId action1, StrId action2, StrId action3);
		void SubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4);
		void SubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4, StrId action5);

		/**
		* Sets owner to behavior
		* @param beh behavior
		* @param owner node to set as an owner
		*/
		static void SetOwner(Behavior* beh, Node* owner) {
			beh->owner = owner;
		}

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