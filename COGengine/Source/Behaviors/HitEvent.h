#pragma once

#include "Behavior.h"
#include "EntityStorage.h"
#include "InputHandler.h"

namespace Cog {

	/**
	* Behavior for hit testing
	* Every object that has a handler for hit events such as buttons, sliders or selections, 
	* must contain this behavior 
	*/
	class HitEvent : public Behavior {

	protected:
		// if true, device will vibrate when object is hit (only for android)
		bool vibrate = false;
		// indicator whether hit started 
		bool hitStarted = false;
		// indicator whether the hit has been lost
		bool hitLost = false;
		// id of button/touch that started the hit
		int hitStartedTouchId = -1;
		// id of behavior that should receive message about hit (-1 for all)
		int handlerBehId = -1;
		// if true, hit will be tested precisely per pixel (suitable for images with alpha channel)
		bool preciseTest = false;

	public:

		HitEvent() {

		}

		/**
		* Creates a new behavior for hit testing
		* @param handlerBehId id of behavior that should receive message about hit (-1 for all)
		* @param preciseTest indicator whether the hit should be tested precisely per-pixel 
		* (suitable for images with alpha channel)
		* @param vibrate if true, device will vibrate when object is hit (only for Android)
		*/
		HitEvent(int handlerBehId, bool preciseTest, bool vibrate) :
			handlerBehId(handlerBehId), preciseTest(preciseTest), vibrate(vibrate) {
		}

		HitEvent(bool preciseTest) : preciseTest(preciseTest){

		}

		virtual void Load(Setting& setting) {
			preciseTest = setting.GetItemValBool("precise_test", false);
		}

		void OnStart() {
			owner->SetState(StrId(STATES_HITTABLE));
		}

	protected:
		/**
		* General hit test for all shapes
		*/
		template<class CShape>
		bool CShapeHitTest(spt<CShape> shape, ofVec3f testPos) {
			if (testPos.x < 0
				|| testPos.y < 0
				|| testPos.x >(float)shape->GetWidth()
				|| testPos.y >(float)shape->GetHeight())
				return false;

			return true;
		}


		/**
		* Makes a hit test
		* @param touchTrans inverted position of the touch/mouse to the local space of object
		* @param touchVEctor absolute position of the touch/mouse
		*/
		bool DoHitTest(ofVec3f testPos, ofVec3f touchVector);

		/**
		* Hit test for bounding box
		* @param bbox bounding box
		* @param touchVector position of the touch/mouse
		*/
		bool BoundingBoxHitTest(spt<BoundingBox> bbox, ofVec2f touchVector);

		/**
		* Tests precisely if the image has been hit
		* @param image image to test
		* @param touchTrans inverted position of the touch/mouse to the local space of object
		*/
		bool ImagePreciseHitTest(spt<ofImage> image, ofVec3f touchTrans);

		virtual void Update(const uint64 delta, const uint64 absolute);

	};
}// namespace