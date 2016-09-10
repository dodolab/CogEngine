#pragma once

#include "Behavior.h"

#include "EntityStorage.h"
#include "InputHandler.h"
#include "HitBox.h"

namespace Cog {

	/**
	* Behavior for hit testing
	*/
	class HitEvent : public Behavior {

	protected:
		// if true, device will vibrate when object is hit
		bool vibrate = false;
		// indicator, if hit has started over this object
		bool hitStarted = false;
		// indicator, if hit has been lost
		bool hitLost = false;
		// id of button who started the hit
		int hitStartedTouchId = -1;
		// id of handler behavior (-1 for all behaviors)
		int handlerBehId = -1;
		// if true, hit will be tested precisely (suitable for alpha-images)
		bool preciseTest = false;

	public:

		HitEvent() {

		}

		/**
		* Creates a new behavior for hit testing
		* @param handlerBehId id of handler behavior (set -1 for all handlers that listens HIT events)
		* @param vibrate if true, device will vibrate when object is hit
		*/
		HitEvent(int handlerBehId, bool preciseTest, bool vibrate) :
			handlerBehId(handlerBehId), preciseTest(preciseTest), vibrate(vibrate) {

		}

		virtual void Load(Setting& setting) {
			preciseTest = setting.GetItemValBool("precise_test", false);
		}

		void OnStart() {
			owner->SetState(StrId(STATES_HITTABLE));
		}

		template<class CShape>
		bool CShapeHitTest(spt<CShape> shape, ofVec3f testPos) {
			if (testPos.x < 0
				|| testPos.y < 0
				|| testPos.x >(float)shape->GetWidth()
				|| testPos.y >(float)shape->GetHeight())
				return false;

			return true;
		}

		bool BoundingBoxHitTest(spt<BoundingBox> bbox, ofVec2f touchVector);

		/**
		* Tests if the image has been hit
		* @param image image to test
		* @param testPos test position
		* @param preciseTest if true, hit will be tested precisely (suitable for alpha-images)
		*/
		bool ImageHitTest(spt<ofImage> image, ofVec3f testPos, bool preciseTest);

		virtual void Update(const uint64 delta, const uint64 absolute);

	};
}// namespace