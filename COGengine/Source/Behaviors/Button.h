#pragma once

#include "Behavior.h"

namespace Cog {

	/**x
	* Behavior that switches button on-click images
	*/
	class Button : public Behavior {
		OBJECT_PROTOTYPE(Button)
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> pressedImg = spt<ofImage>();
		spt<ofImage> disabledImg = spt<ofImage>();
		StringHash stateDisabled = StringHash(STATES_DISABLED);

	public:

		/**
		* Creates a new switch-button behavior
		* @param defaultImg default image
		* @param pressedImg pressed image
		*/
		Button(spt<ofImage> defaultImg, spt<ofImage> pressedImg, spt<ofImage> disabledImg) :
			defaultImg(defaultImg), pressedImg(pressedImg), disabledImg(disabledImg) {

		}

		/**
		* Creates a new switch-button behavior
		* @param defaultImg default image
		* @param pressedImg pressed image
		*/
		Button(spt<ofImage> defaultImg, spt<ofImage> pressedImg) :
			defaultImg(defaultImg), pressedImg(pressedImg) {

		}

		void OnInit();

		void OnStart();

		void OnMessage(Msg& msg);

		void Update(const uint64 delta, const uint64 absolute) {

		}

	};


}// namespace