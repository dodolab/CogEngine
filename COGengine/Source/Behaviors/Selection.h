#pragma once

#include "Behavior.h"


namespace Cog {


	class Selection : public Behavior {
	protected:
		spt<ofImage> defaultImg = spt<ofImage>();
		spt<ofImage> selectedImg = spt<ofImage>();
		StrId selectedState = StrId(STATES_SELECTED);
		bool hasSelectedState = false;
	public:

		Selection() {

		}

		/**
		* Creates a new selection behavior
		* @param defaultImg default image
		* @param selectedImg selected image
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