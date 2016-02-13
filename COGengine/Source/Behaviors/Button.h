#pragma once

#include "Behavior.h"
#include "Bounds.h"
#include "Collision.h"
#include "Shape.h"
#include "Node.h"

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

		void Init() {
			RegisterListening(owner->GetScene(), ACT_OBJECT_HIT_STARTED, ACT_OBJECT_HIT_LOST, ACT_OBJECT_HIT_ENDED);

			if (disabledImg && owner->HasState(stateDisabled)) {
				owner->GetShape<spt<Image>>()->SetImage(disabledImg);
			}
			else {
				owner->GetShape<spt<Image>>()->SetImage(defaultImg);
			}
		}

		void OnMessage(Msg& msg) {
			if (msg.GetSourceObject()->GetId() == owner->GetId()) {
				if (!owner->HasState(stateDisabled) && msg.GetAction() == ACT_OBJECT_HIT_STARTED) {
						msg.GetSourceObject()->GetShape<spt<Image>>()->SetImage(pressedImg);
				}
				else if (!owner->HasState(stateDisabled) && (msg.GetAction() == ACT_OBJECT_HIT_ENDED || msg.GetAction() == ACT_OBJECT_HIT_LOST)) {
						msg.GetSourceObject()->GetShape<spt<Image>>()->SetImage(defaultImg);
				}
				else if (disabledImg && msg.GetSourceObject()->HasState(stateDisabled) && msg.GetAction() == ACT_STATE_CHANGED) {
					msg.GetSourceObject()->GetShape<spt<Image>>()->SetImage(disabledImg);
				}
			}
		}


		void Update(const uint64 delta, const uint64 absolute) {

		}

	};


}// namespace