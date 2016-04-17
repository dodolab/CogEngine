#pragma once

#include "Definitions.h"
#include "MsgListener.h"
#include "StrId.h"
#include "Events.h"
#include "Settings.h"

namespace Cog {

	class Node;

	/**
	* Abstract parent for all behaviors
	* Behavior is a component attached to a node. Each node represents a game object and hence it is 
	* fully defined by set of attributes and behaviors attached
	*
	*/
	class Behavior : public MsgListener {
	public: 

		/**
		* Loads behavior from Setting entity
		*/
		virtual void Load(Setting& setting) {

		}

	protected:
		// owner node
		Node* owner;
		// indicator whether the behavior has ended
		bool finished = false;
		// indicator whether the behavior was initialized
		bool initialized = false;
		// indicator whether the behavior is external (and therefore it shouldn't be deleted)
		bool isExternal = false;
		// indicator whether the behavior started
		bool started = false;
		// indicator whether the behavior should be removed when finished
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
		* Gets the owner node
		*/
		Node* GetOwner() const {
			return owner;
		}

		/**
		* Initializes the behavior
		* Must be called only once
		*/
		void Init() {
			if (!initialized) {
				this->finished = false;
				this->OnInit();
				this->initialized = true;
			}
		}

		/**
		* Starts the behavior
		* May be called repeatedly
		*/
		void Start() {
			this->finished = false;
			this->OnStart();
			this->started = true;
		}


		/**
		* Finishes the behavior
		* May be called repeatedly
		*/
		void Finish();

		

		/**
		* Gets indicator whether the behavior was initialized
		*/
		bool IsInitialized() const {
			return initialized;
		}

		/**
		* Gets indicator whether the behavior has started
		*/
		bool HasStarted() const {
			return started;
		}

		/**
		* Gets indicator whether the behavior has finished
		*/
		bool HasFinished() {
			return finished;
		}

		/**
		* Gets indicator whether the behavior is external and therefore it
		* shouldn't be deleted
		*/
		bool IsExternal() const {
			return isExternal;
		}

		/**
		* Sets the indicator whether the behavior is external
		*/
		void SetIsExternal(bool ext) {
			this->isExternal = ext;
		}

		/**
		* Gets indicator whether the behavior should be removed when finished
		*/
		bool RemoveWhenFinish() const {
			return this->removeWhenFinish;
		}

		/**
		* Sets the indicator whether the behavior should be removed when finished
		*/
		void SetRemoveWhenFinish(bool remove) {
			this->removeWhenFinish = remove;
		}


		friend class Node;

	protected:
	
		/**
		* Initialization procedure
		* Appropriate for attribute initialization and message subscribing
		* Should be called only once
		*/
		virtual void OnInit() {

		}

		/**
		* Starting procedure
		* Appropriate for reseting all variables
		* May be called more than once
		*/
		virtual void OnStart() {

		}

		/**
		* Finalization procedure
		* Appropriate for releasing all resources
		* May be called more than once
		*/
		virtual void OnFinish() {

		}

		/**
		* Sets owner to any behavior
		* @param beh behavior
		* @param owner node to set as an owner
		*/
		static void SetOwner(Behavior* beh, Node* owner) {
			beh->owner = owner;
		}

		/** Subscribes for listening given action */
		void SubscribeForMessages(StrId action1);
		/** Subscribes for listening given actions */
		void SubscribeForMessages(StrId action1, StrId action2);
		/** Subscribes for listening given actions */
		void SubscribeForMessages(StrId action1, StrId action2, StrId action3);
		/** Subscribes for listening given actions */
		void SubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4);
		/** Subscribes for listening given actions */
		void SubscribeForMessages(StrId action1, StrId action2, StrId action3, StrId action4, StrId action5);
		
		/** Sends message to subscribers listening to selected action */
		void SendMessage(StrId action) const;
		/** Sends message with custom context node to subscribers listening to selected action */
		void SendMessage(StrId action, Node* contextNode) const;
		/** Sends message with payload to subscribers listening to selected action */
		void SendMessage(StrId action, spt<MsgEvent> data) const;
		/** Sends message with payload and custom context node to subscribers listening to selected action */
		void SendMessage(StrId action, spt<MsgEvent> data, Node* contextNode) const;
		/** Sends message to recipient with selected id */
		void SendMessageToBehavior(StrId action, int recipientId) const;
		/** Sends message with custom context node to recipient with selected id */
		void SendMessageToBehavior(StrId action, Node* contextNode, int recipientId) const;
		/** Sends message with payload to recipient with selected id */
		void SendMessageToBehavior(StrId action, spt<MsgEvent> data, int recipientId) const;
		/** Sends message with payload and custom context node to recipient with selected id */
		void SendMessageToBehavior(StrId action, spt<MsgEvent> data, Node* contextNode, int recipientId) const;
	};

}// namespace