#pragma once

#include "Behavior.h"

namespace Cog {

	/**
	* Behavior for buttons, handles hit messages, changes images and sends messages
	* The owner node must have HitEvent behavior attached, otherwise it won't work
	*/
	class Button : public Behavior {
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> pressedImg = spt<ofImage>();
		spt<ofImage> disabledImg = spt<ofImage>();
		StrId stateDisabled = StrId(STATE_DISABLED);

	public:
		Button() {

		}

		/**
		* Creates a new button behavior
		* @param defaultImg image of default button
		* @param pressedImg image of pressed button
		* @param disabledImg image of disabled button
		*/
		Button(spt<ofImage> defaultImg, spt<ofImage> pressedImg, spt<ofImage> disabledImg) :
			defaultImg(defaultImg), pressedImg(pressedImg), disabledImg(disabledImg) {

		}

		/**
		* Creates a new button behavior
		* @param defaultImg image of default button
		* @param pressedImg image of pressed button
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