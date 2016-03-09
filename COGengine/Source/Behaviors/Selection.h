#pragma once

#include "Behavior.h"
#include "Bounds.h"
#include "Collision.h"
#include "Shape.h"
#include "Scene.h"

namespace Cog {


	class Selection : public Behavior {
		OBJECT_PROTOTYPE(Selection)
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> selectedImg = spt<ofImage>();
		StringHash selectedState = StringHash(STATES_SELECTED);
		bool hasSelectedState = false;
	public:

		/**
		* Creates a new selection behavior
		* @param defaultImg default image
		* @param selectedImg selected image
		*/
		Selection(spt<ofImage> defaultImg, spt<ofImage> selectedImg) :
			defaultImg(defaultImg), selectedImg(selectedImg) {

		}

		void Init() {
			RegisterListening(owner->GetScene(), ACT_STATE_CHANGED);
			
			if (owner->HasState(selectedState)) {
				owner->GetShape<spt<Image>>()->SetImage(selectedImg);
			}
			else {
				owner->GetShape<spt<Image>>()->SetImage(defaultImg);
			}
		}


		void OnMessage(Msg& msg) {
/*			if (msg.GetAction() == ACT_STATE_CHANGED && msg.GetSourceObject()->GetId() == owner->GetId()) {
				if (owner->HasState(selectedState)) {
					owner->GetShape<spt<Image>>()->SetImage(selectedImg);
				}
				else {
					owner->GetShape<spt<Image>>()->SetImage(defaultImg);
				}
			} moved to the update loop */
		}


		void Update(const uint64 delta, const uint64 absolute) {
			if (!hasSelectedState && owner->HasState(selectedState)) {
				owner->GetShape<spt<Image>>()->SetImage(selectedImg);
				hasSelectedState = true;
			}
			else if(hasSelectedState && !owner->HasState(selectedState)){
				owner->GetShape<spt<Image>>()->SetImage(defaultImg);
				hasSelectedState = false;
			}
		}

	};


}// namespace