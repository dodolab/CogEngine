#pragma once

#include "ofxCogMain.h"

namespace Cog {

	/*! Type of device that invoked the input */
	enum CogInputType {
		KEYBOARD,   /*!< keyboard */
		TOUCH		/*!< pointing device (finger or mouse) */
	};

	/**
	* Entity for input activity
	*/
	class CogInputAct {
	private:
		void Init() {
			handlerId = -1;
			started = true;
			ended = false;
		}

	public:
		// input type 
		CogInputType inputType;
		// pressed key (keyboard only)
		int key;
		// pressed finger (touch input only)
		int touchId;
		// position (touch input only)
		ofVec3f position;

		// handlerId, -1 if there is none
		int handlerId;
		// indicates, if input has just started
		bool started;
		// indicates, if input has ended (e.g. key release)
		bool ended;

		/**
		* Creates a new input action
		* @param keyboardKey pressed key
		*/
		CogInputAct(int keyboardKey) : inputType(CogInputType::KEYBOARD), key(keyboardKey) {
			Init();
		}

		/**
		* Creates a new input action
		* @param touchId pressed button
		* @param pos button position
		*/
		CogInputAct(int touchId, ofVec3f pos) : inputType(CogInputType::TOUCH), touchId(touchId), position(pos) {
			Init();
		}

		CogInputAct() : touchId(-1), key(-1) {
			Init();
		}

		/**
		* Returns true, if the action has been already handled (by some behavior)
		*/
		bool IsHandled() {
			return handlerId != -1;
		}

	};

}