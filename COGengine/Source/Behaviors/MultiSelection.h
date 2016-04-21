#pragma once

#include "Behavior.h"

namespace Cog {

	/*
	* Behavior for selection component (similar to checkboxes)
	* This behavior should be attached to the parent of nodes that can be selected, 
	* because it manipulates with more than one node
	*
	* This component can manipulate with images, colors or only with states
	*/
	class MultiSelection : public Behavior {
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> selectedImg = spt<ofImage>();
		// color of selected node
		ofColor selectedColor;
		// color of unselected node
		ofColor defaultColor;
		// id of group all selected nodes belong to (similar to radiogroup in HTML)
		StrId selectionGroup;
		StrId selectedState = StrId(STATES_SELECTED);
	public:

		MultiSelection() {

		}

		/**
		* Creates a new selection behavior
		* @param defaultImg image of default checkbox
		* @param selectedImg image of selected checkbox
		* @param selectionGroup name of group all selected nodes belong to
		*/
		MultiSelection(spt<ofImage> defaultImg, spt<ofImage> selectedImg, StrId selectionGroup) :
			defaultImg(defaultImg), selectedImg(selectedImg), selectionGroup(selectionGroup) {
		}

		/**
		* Creates a new selection behavior that manipulates only with states
		* @param selectionGroup name of group all selected nodes belong to
		*/
		MultiSelection(StrId selectionGroup) : selectionGroup(selectionGroup) {
		}


		/**
		* Creates a new selection behavior that manipulates only with colors
		* @param defaultColor color of unselected node
		* @param selectedColor color of selected node
		* @param selectionGroup name of group all selected nodes belong to
		*/
		MultiSelection(ofColor defaultColor, ofColor selectedColor, StrId selectionGroup) 
			: defaultColor(defaultColor), selectedColor(selectedColor), selectionGroup(selectionGroup) {
		}

		virtual void Load(Setting& setting);

		void OnInit();

		void OnStart();

		void OnMessage(Msg& msg);

		void Update(const uint64 delta, const uint64 absolute) {

		}

		/**
		* Processes hit message from hit event and transforms it to the
		* message that notifies other behaviors that the object has been selected
		*/
		void ProcessHit(Msg& msg, bool setDirectly);

		/**
		* Sets mesh or color according to owner's state
		*/
		void CheckState();

	};


}// namespace