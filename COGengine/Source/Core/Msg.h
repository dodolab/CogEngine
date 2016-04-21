#pragma once

#include "Events.h"
#include "StrId.h"
#include "Definitions.h"
#include "Constants.h"


namespace Cog {

	class Node;
	
	/**
	* Type of the object participated in message (sender or recipient)
	*/
	enum class MsgObjectType {
		NODE_ROOT = 0,				 /** root node */
		NODE_SCENE = 1,				 /** scene node */
		NODE_COMMON = 2,			 /** actual node */
		NODE_CHILDREN = 3,			 /** children of actual node */
		COMPONENT = 4,				 /** component */
		BEHAVIOR = 5,				 /** behavior */	
		SUBSCRIBERS = 6,			 /** all objects that subscribed the action */
		OTHER = 7					 /** other objects */
	};

	/**
	* Tunneling mode (only for scope that is not of type SUBSCRIBERS)
	*/
	enum class TunnelingMode {
		TUNNELING = 0,		/** message will be sent from top to bottom of the tree */
		BUBBLING = 1		/** message will be sent from bottom to top of the tree */
	};

	/**
	* Message for communicating between behaviors, nodes and components
	*/
	class Msg {
	private:
		// identifier counter
		static int idCounter;
		// id of this message
		int id;
		// type of invoked action 
		StrId action;
		// custom parameter
		int parameter = 0;
		// type of the sender
		MsgObjectType senderType;
		// sender id (-1 for entities without id)
		int senderId;
		// type of the recipient
		MsgObjectType recipientType = MsgObjectType::SUBSCRIBERS;
		// recipient id (-1 for subscribers and other objects)
		int recipientId = 1;
		// tunneling mode
		TunnelingMode tunnelingMode = TunnelingMode::TUNNELING;
		// indicator whether the message should be sent to the whole subtree of actual node 
		// (only for recipient that is not of type SUBSCRIBERS)
		bool sendToWholeTree = true;


		// node that is connected with this message
		Node* contextNode = nullptr;
		// data payload
		spt<MsgEvent> data = spt<MsgEvent>();
		
	public:

		Msg() {

		}

		/**
		* Creates a new message
		* @param action type of invoked action
		* @param senderType type of the sender
		* @param senderId id of the sender
		* @param recipientType type of the recipient
		* @param recipientId id of the recipient
		* @param tunnelingMode tunneling mode
		* @param contextNode node that is connected with this message
		* @param data data payload
		*/
		Msg(StrId action, MsgObjectType senderType, int senderId, MsgObjectType recipientType, 
			int recipientId, TunnelingMode tunnelingMode, Node* contextNode, spt<MsgEvent> data);

		/**
		* Creates a new message
		* @param action type of invoked action
		* @param senderType type of the sender
		* @param senderId id of the sender
		* @param recipientType type of the recipient
		* @param tunnelingMode tunneling mode
		* @param contextNode node that is connected with this message
		* @param data data payload
		*/
		Msg(StrId action, MsgObjectType senderType, int senderId, MsgObjectType recipientType, 
			TunnelingMode tunnelingMode, Node* contextNode, spt<MsgEvent> data);

		/**
		* Creates a new message
		* @param action type of invoked action
		* @param senderType type of the sender
		* @param senderId id of the sender
		* @param recipientType type of the recipient
		* @param tunnelingMode tunneling mode
		* @param sendToWholeTree if true, the message whill be sent to the whole subtree of actual node (recipient mustn't be of type SUBSCRIBER)
		* @param contextNode node that is connected with this message
		* @param data data payload
		*/
		Msg(StrId action, MsgObjectType senderType, int senderId, MsgObjectType recipientType, 
			TunnelingMode tunnelingMode, bool sendToWholeTree, Node* contextNode, spt<MsgEvent> data);

		/**
		* Creates a new message
		* @param action type of invoked action
		* @param senderType type of the sender
		* @param senderId id of the sender
		* @param recipientType type of the recipient
		* @param contextNode node that is connected with this message
		* @param data data payload
		*/
		Msg(StrId action, MsgObjectType senderType, int senderId, MsgObjectType recipientType, Node* contextNode, spt<MsgEvent> data);


		~Msg() {

		}

		/**
		* Gets type of action
		*/
		StrId GetAction() const {
			return action;
		}

		/**
		* Checkes whether the action is equal to given value
		*/
		bool HasAction(StrId actionCmp) const {
			return action == actionCmp;
		}

		/**
		* Sets the type of action
		*/
		void SetAction(StrId action) {
			this->action = action;
		}

		/**
		* Gets custom parameter
		*/
		int GetParameter() const {
			return parameter;
		}

		/**
		* Sets custom parameter
		*/
		void SetParameter(int parameter) {
			this->parameter = parameter;
		}

		/**
		* Gets type of the sender
		*/
		MsgObjectType GetSenderType() const {
			return senderType;
		}

		/**
		* Sets type of the sender
		*/
		void SetSenderType(MsgObjectType type) {
			this->senderType = type;
		}

		/**
		* Gets id of the sender
		*/
		int GetSenderId() const {
			return senderId;
		}

		/**
		* Sets id of the sender (-1 for nodes which are not part of the scene)
		*/
		void SetSenderId(int senderId) {
			this->senderId = senderId;
		}

		/**
		* Gets type of the recipient
		*/
		MsgObjectType GetRecipientType() const {
			return recipientType;
		}

		/**
		* Sets type of the recipient
		*/
		void SetRecipientType(MsgObjectType type) {
			this->recipientType = type;
		}

		/**
		* Gets id of the recipient (-1 for subscribers and other objects)
		*/
		int GetRecipientId() const {
			return recipientId;
		}

		/**
		* Sets id of the recipient
		*/
		void SetRecipientId(int recipientId) {
			this->recipientId = recipientId;
		}

		/**
		* Gets the tunneling mode (BUBBLE or TUNNELING)
		*/
		TunnelingMode GetTunnelingMode() const {
			return tunnelingMode;
		}

		/**
		* Sets the tunneling mode (BUBBLE or TUNNELING)
		*/
		void SetTunnelingMode(TunnelingMode mode) {
			this->tunnelingMode = mode;
		}

		/**
		* Gets indicator whether the message should be sent to the whole subtree of actual node
		* (only for recipient that is not of type SUBSCRIBERS)
		*/
		bool SendToWholeTree() const {
			return sendToWholeTree;
		}

		/**
		* Sets indicator whether the message should be sent to the whole subtree of actual node 
		* (only for recipient that is not of type SUBSCRIBERS)
		*/
		void SetSendToWholeTree(bool sendToWholeTree) {
			this->sendToWholeTree = sendToWholeTree;
		}

		/**
		* Returns true, if this message has a context node
		*/
		bool HasContextNode() {
			return contextNode != nullptr;
		}

		/**
		* Gets the node that is connected with this message
		*/
		Node* GetContextNode() {
			return contextNode;
		}

		/**
		* Sets the node that is connected with this message
		*/
		void SetContextNode(Node* contextNode) {
			this->contextNode = contextNode;
		}


		/**
		* Gets data payload
		*/
		template <class T>
		spt<T> GetData() {
			return static_pointer_cast<T>(data);
		}
	};

}// namespace