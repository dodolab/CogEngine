#pragma once

#include "ofxCogMain.h"
#include "Flags.h"
#include "Msg.h"
#include "Facade.h"


namespace Cog {

	class Node;

	/**
	* Abstract class for all behaviors
	*
	*/
	class Behavior {
		OBJECT_VIRTUAL()
		PROTOTYPE_VIRTUAL(Behavior)

	protected:
		// identifier incremental counter
		static int idCounter;
		// identifier
		const int id;
		// behavior running state
		BehState behState;
		// owner node
		Node* owner;
		// indicator if this behavior has ended
		bool ended;

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
		const Node* GetOwner() const {
			return owner;
		}

		/**
		* Gets element identifier
		* @return incremental value
		*/
		const int GetId() const {
			return id;
		}

		/**
		* Gets behavior running state
		* @return running state
		*/
		const BehState GetBehState() const {
			return behState;
		}

		/**
		* Sets behavior running state
		* @param val value
		*/
		void SetBehState(BehState val) {
			behState = val;
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
		* Handler for accepting the message
		* @param Msg received message
		*/
		virtual void OnMessage(Msg& msg) {

		}

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
		* Registers itself as a action listener
		* @param action1 action to listen to
		*/
		void RegisterListening(StringHash action1);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		*/
		void RegisterListening(StringHash action1, StringHash action2);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		*/
		void RegisterListening(StringHash action1, StringHash action2, StringHash action3);

		/**
		* Registers itself as a action listener
		* @param action1 action to listen to
		* @param action2 action to listen to
		* @param action3 action to listen to
		* @param action4 action to listen to
		*/
		void RegisterListening(StringHash action1, StringHash action2, StringHash action3, StringHash action4);


		/**
		* Sends a message to any set of behaviors
		* @param bubblingType setting that indicates who should process this message and how it will bubble through scene tree
		* @param action id of action; see Actions namespace for common action ids
		* @param subaction id of subaction; see Actions namespace for common action ids
		* @param data payload
		* @param source source node that is a part of message
		*/
		void SendMessage(BubblingType bubblingType, StringHash action, int subaction, void* data, Node* source) const;

		/**
		* Sends a message to any set of behaviors without tree-bubbling
		* @param action id of action; see Actions namespace for common action ids
		* @param subaction id of subaction; see Actions namespace for common action ids
		* @param data payload
		* @param source source node that is a part of message
		*/
		void SendMessageNoBubbling(StringHash action, int subaction, void* data, Node* source) const;

		/**
		* Sends a message to one behavior with specific id
		* @param action id of action; see Actions namespace for common action ids
		* @param subaction id of subaction; see Actions namespace for common action ids
		* @param data payload
		* @param source source node that is a part of message
		* @param behaviorId id of behavior that should get this message
		*/
		void SendDirectMessage(StringHash action, int subaction, void* data, Node* source, int behaviorId) const;

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