#pragma once

#include "Behavior.h"

namespace Cog {

	/**x
	* Behavior for selection (similar to button but only one node can be selected)
	* This behavior should be attached to selection's parents, because it manipulates with more than one nodes
	*/
	class MultiSelection : public Behavior {
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> selectedImg = spt<ofImage>();
		ofColor selectedColor;
		ofColor defaultColor;
		StrId selectionGroup;
		StrId selectedState = StrId(STATES_SELECTED);
	public:

		MultiSelection() {

		}

		/**
		* Creates a new selection behavior
		* @param defaultImg default image
		* @param selectedImg selected image
		* @param selectionGroup group in where selection is controlled
		*/
		MultiSelection(spt<ofImage> defaultImg, spt<ofImage> selectedImg, StrId selectionGroup) :
			defaultImg(defaultImg), selectedImg(selectedImg), selectionGroup(selectionGroup) {
		}

		/**
		* Creates a new selection behavior without images
		*/
		MultiSelection(StrId selectionGroup) : selectionGroup(selectionGroup) {
		}


		/**
		* Creates a new selection behavior without images
		*/
		MultiSelection(ofColor defaultColor, ofColor selectedColor, StrId selectionGroup) 
			: defaultColor(defaultColor), selectedColor(selectedColor), selectionGroup(selectionGroup) {
		}

		virtual void Load(Setting& setting);

		void OnInit();

		void OnStart();

		void OnMessage(Msg& msg);

		void ProcessCheckMessage(Msg& msg, bool setDirectly);

		void Update(const uint64 delta, const uint64 absolute) {

		}

		void CheckState();

	};


}// namespace