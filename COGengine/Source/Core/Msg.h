#pragma once

#include "Events.h"
#include "StringHash.h"
#include "Enums.h"

namespace Cog {

	class Node;
	
	/**
	* Configuration class that holds information how the message should bubble through the scene tre
	*/
	class BubblingType {

	public:

		/**
		* Creates a new bubbling configuration
		* @param scopeType type of object bubbling should start from
		* @param deep if true, message will be sent to near nodes as well
		* @param bubbleDown if true, bubbling will go from top to bottom; otherwise it will go from bottom
		*/
		BubblingType(Scope scopeType, bool deep, bool bubbleDown) : scope(scopeType), deep(deep), bubbleDown(bubbleDown) {

		}
		// scope object
		Scope scope;
		// if true, message will be sent to near nodes as well
		bool deep;
		// if true, bubbling will go from top to bottom; otherwise it will go from bottom 
		bool bubbleDown;
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
		// bubbling settings
		BubblingType bubblingType;
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
		* @param bubblingType bubbling settings
		* @param action id of action that has been invoked
		* @param subAction id of subaction that has been invoked
		* @param behaviorId id of behavior that sent this message
		* @param sourceObj source object
		* @param data payload
		*/
		Msg(BubblingType bubblingType, StringHash action, int subAction, int behaviorId, Node* sourceObj, MsgEvent* data);

		~Msg() {

		}

		/**
		* Gets id of action; see Actions for common action ids
		*/
		const StringHash GetAction() const {
			return action;
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
		BubblingType& GetBubblingType() {
			return bubblingType;
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