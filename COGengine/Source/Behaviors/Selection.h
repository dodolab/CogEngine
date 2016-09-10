#pragma once

#include "Behavior.h"
#include "Flags.h"
#include "Bounds.h"
#include "Collision.h"
#include "Shape.h"
#include "Scene.h"

namespace Cog {

	/**x
	* Behavior for selection (similar to button but only one node can be selected)
	* This behavior should be attached to selection's parents, because it manipulates with more than one nodes
	*/
	class Selection : public Behavior {
		OBJECT_PROTOTYPE(Selection)
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> selectedImg = spt<ofImage>();
		StringHash selectionGroup;
		StringHash selectedState = StringHash(STATES_SELECTED);

	public:

		/**
		* Creates a new selection behavior
		* @param defaultImg default image
		* @param selectedImg selected image
		* @param selectionGroup group in where selection is controlled
		*/
		Selection(spt<ofImage> defaultImg, spt<ofImage> selectedImg, StringHash selectionGroup) :
			defaultImg(defaultImg), selectedImg(selectedImg), selectionGroup(selectionGroup) {

		}

		void Init() {
			RegisterListening(owner->GetScene(), ACT_OBJECT_HIT_ENDED);
			
			if (owner->HasState(selectedState)) {
				owner->GetShape<spt<Image>>()->SetImage(selectedImg);
			}
			else {
				owner->GetShape<spt<Image>>()->SetImage(defaultImg);
			}
		}


		void OnMessage(Msg& msg) {
			if (msg.GetAction() == ACT_OBJECT_HIT_ENDED && msg.GetSourceObject()->IsInGroup(selectionGroup)) {
				// check if the object was clicked (user could click on different area and release touch event over the button)
				InputEvent* evt = static_cast<InputEvent*>(msg.GetData());
				if (evt->input->handlerNodeId == msg.GetSourceObject()->GetId()) {
					
					if (msg.GetSourceObject()->GetId() == owner->GetId()) {
						// selected actual node
						owner->SetState(selectedState);
						owner->GetShape<spt<Image>>()->SetImage(selectedImg);
					}
					else {
						owner->ResetState(selectedState);
						owner->GetShape<spt<Image>>()->SetImage(defaultImg);
					}
				}
			}
		}


		void Update(const uint64 delta, const uint64 absolute) {

		}

	};


}// namespace