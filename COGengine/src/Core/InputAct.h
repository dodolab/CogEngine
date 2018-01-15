#pragma once

#include "Definitions.h"
#include "Vec2i.h"

namespace Cog {

	/** Type of device that invoked the input */
	enum InputType {
		KEYBOARD,   /** keyboard */
		TOUCH		/** pointing device (touch or mouse) */
	};

	/**
	* Entity describing input action
	*/
	class InputAct {

	public:
		// input type 
		InputType inputType;
		// pressed key (keyboard only)
		int key = -1;
		// pressed finger (touch input only)
		int touchId = -1;
		// position (touch input only)
		Vec2i position;

		// id of node that handled the event, -1 if there is none
		int handlerNodeId = -1;
		// first behavior that will serve the action will set this flag
		bool isProcessed = false;
		// indicates, if input has just started
		bool started = true;
		// indicates, if input has ended (e.g. key release)
		bool ended = false;

		/**
		* Creates a new input action
		* @param keyboardKey pressed key
		*/
		InputAct(int keyboardKey) 
			: inputType(InputType::KEYBOARD), key(keyboardKey) {
		}

		/**
		* Creates a new input action
		* @param touchId pressed button
		* @param pos button position
		*/
		InputAct(int touchId, Vec2i pos) : inputType(InputType::TOUCH), touchId(touchId), position(pos) {
		}

		InputAct() {
		}

		/**
		* Returns true, if the action has been already handled (by some object)
		*/
		bool IsHandled() {
			return handlerNodeId != -1;
		}

		/**
		* Returns true, if there was at least one object that checked the action
		*/
		bool IsProcessed() {
			return isProcessed;
		}

		/**
		* Sets the indicator whether an object already checked out this action
		*/
		void SetIsProcessed(bool processed) {
			this->isProcessed = processed;
		}

	};

}// namespace