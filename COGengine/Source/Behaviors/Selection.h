#pragma once

#include "Behavior.h"


namespace Cog {

	/*
	* Behavior for selection component (similar to radio buttons)
	* Only one node can be selected at a time
	*/
	class Selection : public Behavior {
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> selectedImg = spt<ofImage>();
		StrId selectedState = StrId(STATES_SELECTED);
		// indicator whether the object has the selected state set
		bool hasSelectedState = false;
	public:

		Selection() {

		}

		/**
		* Creates a new selection behavior
		* @param defaultImg image of default node
		* @param selectedImg image of selected node
		*/
		Selection(spt<ofImage> defaultImg, spt<ofImage> selectedImg) :
			defaultImg(defaultImg), selectedImg(selectedImg) {

		}

		void OnInit();

		void OnStart();

		void OnMessage(Msg& msg);

		virtual void Update(const uint64 delta, const uint64 absolute);
	};


}// namespace