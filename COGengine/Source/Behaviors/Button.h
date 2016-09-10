#pragma once

#include "Behavior.h"
#include "Flags.h"
#include "Bounds.h"
#include "Collision.h"
#include "Shape.h"

namespace Cog {

	/**x
	* Behavior that switches button on-click images
	*/
	class Button : public Behavior {
		OBJECT_PROTOTYPE(Button)
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> pressedImg = spt<ofImage>();

	public:

		/**
		* Creates a new switch-button behavior
		* @param defaultImg default image
		* @param pressedImg pressed image
		*/
		Button(spt<ofImage> defaultImg, spt<ofImage> pressedImg) :
			defaultImg(defaultImg), pressedImg(pressedImg) {

		}

		void Init() {
			RegisterListening(ACT_OBJECT_HIT_STARTED, ACT_OBJECT_HIT_LOST, ACT_OBJECT_HIT_ENDED);
		}

		void OnMessage(Msg& msg) {
			if (msg.GetAction() == ACT_OBJECT_HIT_STARTED) {
				if (msg.GetSourceObject()->GetId() == owner->GetId()) {
					msg.GetSourceObject()->GetShape<spt<Image>>()->SetImage(pressedImg);
				}
			}

			if (msg.GetAction() == ACT_OBJECT_HIT_ENDED || msg.GetAction() == ACT_OBJECT_HIT_LOST) {
				if (msg.GetSourceObject()->GetId() == owner->GetId()) {
					msg.GetSourceObject()->GetShape<spt<Image>>()->SetImage(defaultImg);
				}
			}
		}


		void Update(const uint64 delta, const uint64 absolute) {

		}

	};


}// namespace