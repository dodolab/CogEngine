#pragma once

#include "Behavior.h"

namespace Cog {

	/**x
	* Behavior for selection (similar to button but only one node can be selected)
	* This behavior should be attached to selection's parents, because it manipulates with more than one nodes
	*/
	class MultiSelection : public Behavior {
		OBJECT_PROTOTYPE_INIT(MultiSelection)
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> selectedImg = spt<ofImage>();
		ofColor selectedColor;
		ofColor defaultColor;
		StringHash selectionGroup;
		StringHash selectedState = StringHash(STATES_SELECTED);
	public:

		/**
		* Creates a new selection behavior
		* @param defaultImg default image
		* @param selectedImg selected image
		* @param selectionGroup group in where selection is controlled
		*/
		MultiSelection(spt<ofImage> defaultImg, spt<ofImage> selectedImg, StringHash selectionGroup) :
			defaultImg(defaultImg), selectedImg(selectedImg), selectionGroup(selectionGroup) {
		}

		/**
		* Creates a new selection behavior without images
		*/
		MultiSelection(StringHash selectionGroup) : selectionGroup(selectionGroup) {
		}


		/**
		* Creates a new selection behavior without images
		*/
		MultiSelection(ofColor defaultColor, ofColor selectedColor, StringHash selectionGroup) 
			: defaultColor(defaultColor), selectedColor(selectedColor), selectionGroup(selectionGroup) {
		}

		MultiSelection(Setting& setting);

		void OnInit();

		void OnStart();

		void OnMessage(Msg& msg);

		void ProcessCheckMessage(Msg& msg, bool setDirectly);

		void Update(const uint64 delta, const uint64 absolute) {

		}

		void CheckState();

	};


}// namespace