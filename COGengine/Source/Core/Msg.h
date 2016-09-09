#pragma once

#include "ofxCogMain.h"
#include "Flags.h"

namespace Cog {

	class Msg;
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
		int action;
		// id of subaction that has been invoked
		int subAction;
		// bubbling settings
		BubblingType bubblingType;
		// data payload
		void* data;
		// id of behavior that sent this message
		int behaviorId;
		// source node that is connected with this message
		Node* sourceObj;

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
		Msg(BubblingType bubblingType, int action, int subAction, int behaviorId, Node* sourceObj, void* data);

		~Msg() {

		}

		/**
		* Gets id of action; see Actions for common action ids
		*/
		const int GetAction() const {
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
		const void* GetData() const {
			return data;
		}

		/**
		* Gets data payload
		*/
		void* GetData() {
			return data;
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

	};

}