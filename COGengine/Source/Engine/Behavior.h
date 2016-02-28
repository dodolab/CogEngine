#pragma once

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
		OBJECT_VIRTUAL()
		PROTOTYPE_VIRTUAL_INIT(Behavior)

	protected:
		// owner node
		Node* owner;
		// indicator if this behavior has ended
		bool ended = false;
		// indicator, if this behavior has been initialized
		bool initialized = false;

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
		virtual void Init() {

		}

		/**
		* Gets the owner node
		*/
		Node* GetOwner() const {
			return owner;
		}



		/**
		* Returns true, if the behavior has ended
		*/
		bool Ended() {
			return ended;
		}

		/**
		* Finishes the behavior
		*/
		void Finish() {
			ended = true;
			SendMessageNoBubbling(ACT_BEHAVIOR_FINISHED, this->GetId(), nullptr, owner);
		}

		/**
		* Restarts the behavior
		*/
		void Restart() {
			ended = false;
			Init();
		}

		/**
		* Gets indicator, if this behavior has been initialized
		*/
		bool GetIsInitialized() {
			return initialized;
		}

		/**
		* Sets the indicator, if this behavior has been initialized
		*/
		void SetIsInitialized(bool isInitialized) {
			this->initialized = isInitialized;
		}
	public:
	
		/**
		* Updates behavior and owning object inner state
		* @param delta delta time from the last loop
		* @param absolute absolute time since the application begun
		*/
		virtual void Update(const uint64 delta, const uint64 absolute) = 0;

		bool operator==(int id) {
			return this->id == id;
		}

		bool operator==(const Behavior& other) {
			return id == other.id;
		}

		bool operator!=(int id) {
			return this->id != id;
		}

		bool operator!=(const Behavior& other) {
			return id != other.id;
		}

		// allow to access Node private members
		friend class Node;

	protected:
	

		/**
		* Sets owner to behavior
		* @param beh behavior
		* @param owner node to set as an owner
		*/
		static void SetOwner(Behavior* beh, Node* owner) {
			beh->owner = owner;
		}
	};

}// namespace