#pragma once

#include "ofxCogCommon.h"

namespace Cog {

	/*! Type of device that invoked the input */
	enum InputType {
		KEYBOARD,   /*!< keyboard */
		TOUCH		/*!< pointing device (finger or mouse) */
	};

	/**
	* Entity for input activity
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
		ofVec3f position;

		// handlerId, -1 if there is none
		int handlerNodeId = -1;
		// indicates, if input has just started
		bool started = true;
		// indicates, if input has ended (e.g. key release)
		bool ended = false;

		/**
		* Creates a new input action
		* @param keyboardKey pressed key
		*/
		InputAct(int keyboardKey) : inputType(InputType::KEYBOARD), key(keyboardKey) {
		}

		/**
		* Creates a new input action
		* @param touchId pressed button
		* @param pos button position
		*/
		InputAct(int touchId, ofVec3f pos) : inputType(InputType::TOUCH), touchId(touchId), position(pos) {
		}

		InputAct() {
		}

		/**
		* Returns true, if the action has been already handled (by some node)
		*/
		bool IsHandled() {
			return handlerNodeId != -1;
		}

	};

}// namespace