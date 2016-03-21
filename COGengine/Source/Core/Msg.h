#pragma once

#include "Events.h"
#include "StringHash.h"
#include "Constants.h"

namespace Cog {

	class Node;
	
	/*! Scope object for messaging */
	enum class Scope {
		ROOT = 0,				 /*!< root object scope */
		SCENE = 1,				 /*!< scene object scope */
		OBJECT = 2,				 /*!< source object scope */
		CHILDREN = 3,			 /*!< children objects scope */
		DIRECT_NO_TRAVERSE = 4   /*!< no HandlingType made, objects are obtained from map */
	};

	/**
	* Configuration class that holds information how the message should bubble through the scene tre
	*/
	class HandlingType {

	public:

		/**
		* Creates a new handling configuration
		* @param scopeType type of object handling should start from
		* @param deep if true, message will be sent to children as well
		* @param tunneling if true, handling will go from top to bottom; otherwise it will go from bottom (bubbling)
		*/
		HandlingType(Scope scopeType, bool deep, bool tunneling) : scope(scopeType), deep(deep), tunneling(tunneling) {

		}
		// scope object
		Scope scope;
		// if true, message will be sent to near nodes as well
		bool deep;
		// if true, handling will go from top to bottom; otherwise it will go from bottom (bubbling)
		bool tunneling;
	};

	/**
	* Message that is used to send information Tween nodes
	* and their behaviors
	*/
	class Msg {
	private:
		// identifier counter
		static int idCounter;
		// id of this message
		int id;
		// id of action that has been invoked
		StringHash action;
		// id of subaction that has been invoked
		int subAction;
		// handling settings
		HandlingType handlingType;
		// data payload
		MsgEvent* data = nullptr;
		// id of behavior that sent this message
		int behaviorId;
		// source node that is connected with this message
		Node* sourceObj;
		// indicator, if data has been kept by some object (if true, data won't be deleted)
		bool dataKept = false;
	public:

		/**
		* Creates a new message
		* @param handlingType handling settings
		* @param action id of action that has been invoked
		* @param subAction id of subaction that has been invoked
		* @param behaviorId id of behavior that sent this message
		* @param sourceObj source object
		* @param data payload
		*/
		Msg(HandlingType handlingType, StringHash action, int subAction, int behaviorId, Node* sourceObj, MsgEvent* data);

		~Msg() {

		}

		/**
		* Gets id of action; see Actions for common action ids
		*/
		const StringHash GetAction() const {
			return action;
		}

		/**
		* Gets indicator, if the action is equal to selected value
		*/
		const bool HasAction(StringHash actionCmp) const {
			return action == actionCmp;
		}

		/**
		* Gets id of subaction; see Actions for common action ids
		*/
		const int GetSubaction() const {
			return subAction;
		}

		/**
		* Gets bubbling configuration
		*/
		HandlingType& GetHandlingType() {
			return handlingType;
		}

		/**
		* Gets data payload
		*/
		const MsgEvent* GetData() const {
			return data;
		}

		/**
		* Gets data payload
		*/
		MsgEvent* GetData() {
			return data;
		}

		template <class T>
		T* GetDataS() {
			return static_cast<T*>(data);
		}

		/**
		* Gets identifier of the behavior that sent this message
		*/
		const int GetBehaviorId() const {
			return behaviorId;
		}

		/**
		* Gets source object that is connected with this message
		*/
		Node* GetSourceObject() {
			return sourceObj;
		}

		/**
		* Sets indicator that data has been kept so they won't be deleted
		*/
		void SetDataKept(bool val) {
			this->dataKept = val;
		}

		/**
		* Gets indicator, if data has been kept by some object
		*/
		bool DataKept() {
			return this->dataKept;
		}

		/**
		* Deletes data
		*/
		void DeleteData() {
			delete data;
		}
	};

}// namespace