#pragma once

#include "CogBehavior.h"
#include "CogFlags.h"
#include "CogBounds.h"
#include "CogCollision.h"
#include "CogShape.h"

namespace Cog {

	/**x
	* Behavior that switches button on-click images
	*/
	class CogButton : public CogBehavior {
	protected:
		spt<ofImage> defaultImg;
		spt<ofImage> pressedImg;

	public:

		/**
		* Creates a new switch-button behavior
		* @param defaultImg default image
		* @param pressedImg pressed image
		*/
		CogButton(spt<ofImage> defaultImg, spt<ofImage> pressedImg) :
			defaultImg(defaultImg), pressedImg(pressedImg) {

		}

		void Init() {
			RegisterListening(Actions::OBJECT_HIT_STARTED, Actions::OBJECT_HIT_LOST, Actions::OBJECT_HIT_ENDED);
		}

		void OnMessage(CogMsg& msg) {
			if (msg.GetAction() == Actions::OBJECT_HIT_STARTED) {
				if (msg.GetSourceObject()->GetId() == owner->GetId()) {
					msg.GetSourceObject()->GetShape<spt<CogImage>>()->SetImage(pressedImg);
				}
			}

			if (msg.GetAction() == Actions::OBJECT_HIT_ENDED || msg.GetAction() == Actions::OBJECT_HIT_LOST) {
				if (msg.GetSourceObject()->GetId() == owner->GetId()) {
					msg.GetSourceObject()->GetShape<spt<CogImage>>()->SetImage(defaultImg);
				}
			}
		}


		void Update(const uint64 delta, const uint64 absolute) {

		}

	};


}