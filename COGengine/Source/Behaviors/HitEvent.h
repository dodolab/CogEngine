#pragma once

#include "Behavior.h"

#ifdef ANDROID
#include "ofxAndroidVibrator.h"
#endif

#include "EntityStorage.h"
#include "InputHandler.h"
#include "HitBox.h"

namespace Cog {

	/**
	* Behavior for hit testing
	*/
	class HitEvent : public Behavior {
		OBJECT_PROTOTYPE_INIT(HitEvent)
		BEHAVIOR_UNIQUE()

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

		/**
		* Creates a new behavior for hit testing
		* @param handlerBehId id of handler behavior (set -1 for all handlers that listens HIT events)
		* @param vibrate if true, device will vibrate when object is hit
		*/
		HitEvent(int handlerBehId, bool preciseTest, bool vibrate) :
			handlerBehId(handlerBehId), preciseTest(preciseTest), vibrate(vibrate) {

		}

		HitEvent(Setting setting) {
			preciseTest = setting.GetItemValBool("precise_test", false);
		}

		void OnStart() {
			owner->SetState(StringHash(STATES_HITTABLE));
		}


		bool RectangleHitTest(spt<Rectangle> rectangle, ofVec3f testPos) {
			if (testPos.x < 0
				|| testPos.y < 0
				|| testPos.x >(float)rectangle->GetWidth()
				|| testPos.y >(float)rectangle->GetHeight())
				return false;

			return true;
		}

		bool PlaneHitTest(spt<Plane> plane, ofVec3f testPos) {
			if (testPos.x < 0
				|| testPos.y < 0
				|| testPos.x >(float)plane->GetWidth()
				|| testPos.y >(float)plane->GetHeight())
				return false;

			return true;
		}

		bool BoundingBoxHitTest(spt<BoundingBox> bbox, ofVec2f touchVector) {
			bbox->Recalc(owner);
			ofRectangle bboxRect = bbox->GetBoundingBox();

			if (bboxRect.x <= touchVector.x && (bboxRect.x + bboxRect.width) >= touchVector.x
				&& bboxRect.y <= touchVector.y && (bboxRect.y + bboxRect.height) >= touchVector.y) return true;
			return false;
		}

		/**
		* Tests if the image has been hit
		* @param image image to test
		* @param testPos test position
		* @param preciseTest if true, hit will be tested precisely (suitable for alpha-images)
		*/
		bool ImageHitTest(spt<ofImage> image, ofVec3f testPos, bool preciseTest) {
			// move the test position into "local" coordinate space
			ofVec3f localPos = testPos;
			//cout << "X: " << localPos.x << " Y:" << localPos.y << endl;
			// test for location outside the image rectangle
			if (localPos.x < 0
				|| localPos.y < 0
				|| localPos.x >(float)image->getWidth()
				|| localPos.y >(float)image->getHeight())
				return false;

			if (preciseTest) {

				ofColor col = image->getColor((int)localPos.x, (int)localPos.y);
				// return a hit if the specified local alpha value is greater than half
				return ((int)col.a) > 0x80;
			}
			else return true;
		}

		/**
		* Tests if the sprite has been hit
		*/
		bool MultiSpriteHitTest(spt<SpritesShape> shape, ofVec3f testPos) {
			if (testPos.x < 0
				|| testPos.y < 0
				|| testPos.x >(float)shape->GetWidth()
				|| testPos.y >(float)shape->GetHeight())
				return false;

			return true;
		}

		bool SpriteHitTest(spt<SpriteShape> shape, ofVec3f testPos) {
			if (testPos.x < 0
				|| testPos.y < 0
				|| testPos.x >(float)shape->GetSprite()->GetWidth()
				|| testPos.y >(float)shape->GetSprite()->GetHeight())
				return false;

			return true;
		}

		virtual void Update(const uint64 delta, const uint64 absolute) {

			if (owner->HasState(StringHash(STATES_HITTABLE))) {

				// get inverse matrix
				ofMatrix4x4 inverse = owner->GetTransform().GetAbsMatrix().getInverse();

				bool atLeastOneTouch = false;

				for (InputAct* touch : CogGetPressedPoints()) {

					// calculate vector in image space
					ofVec2f touchVector = touch->position;
					ofVec3f touchTrans = ofVec3f(touchVector)*inverse;

					// do hit test
					bool hasHitTest = false;


					if (owner->HasRenderType(RenderType::IMAGE)) hasHitTest = ImageHitTest(owner->GetShape<spt<Image>>()->GetImage(), touchTrans, preciseTest);
					else if (owner->HasRenderType(RenderType::MULTISPRITE)) {
						hasHitTest = MultiSpriteHitTest(owner->GetShape<spt<SpritesShape>>(), touchTrans);
					}
					else if (owner->HasRenderType(RenderType::SPRITE)) {
						hasHitTest = SpriteHitTest(owner->GetShape<spt<SpriteShape>>(), touchTrans);
					}
					else if (owner->HasRenderType(RenderType::RECTANGLE)) {
						hasHitTest = RectangleHitTest(owner->GetShape<spt<Rectangle>>(), touchTrans);
					}
					else if (owner->HasRenderType(RenderType::PLANE)) {
						hasHitTest = PlaneHitTest(owner->GetShape<spt<Plane>>(), touchTrans);
					}
					else if (owner->HasRenderType(RenderType::BOUNDING_BOX)) {
						hasHitTest = BoundingBoxHitTest(owner->GetShape<spt<BoundingBox>>(), touchVector);
					}


					if ((touch->handlerNodeId == -1 || touch->handlerNodeId == owner->GetId()) && hasHitTest) {
						// image has been hit
						if (touch->started) {
#ifdef ANDROID
							if (vibrate) ofxAndroidVibrator::vibrate(50);
#endif
							atLeastOneTouch = true;
							hitStarted = true;
							hitStartedTouchId = touch->touchId;

							auto inputHandler = GETCOMPONENT(InputHandler);
							inputHandler->RegisterRequest(owner, touch, -1);

							/*
							touch->handlerNodeId = owner->GetId();
							owner->SetState(StringHash(STATES_HIT));
							if (handlerBehId == -1) SendMessageToListeners(ACT_OBJECT_HIT_STARTED, 0, new InputEvent(touch), owner);
							else SendDirectMessage(ACT_OBJECT_HIT_STARTED, 0, new InputEvent(touch), owner, handlerBehId);*/
						}
						else if (touch->ended) {


							owner->ResetState(StringHash(STATES_HIT));
							if (hitStarted) {
								if (handlerBehId == -1) SendMessageToListeners(ACT_OBJECT_HIT_ENDED, 0, new InputEvent(touch), owner);
								else SendDirectMessage(ACT_OBJECT_HIT_ENDED, 0, new InputEvent(touch), owner, handlerBehId);
							}
							else {
								// hit has been lost
								hitLost = true;
								if (handlerBehId == -1) SendMessageToListeners(ACT_OBJECT_HIT_LOST, 0, new InputEvent(touch), owner);
								else SendDirectMessage(ACT_OBJECT_HIT_LOST, 0, new InputEvent(touch), owner, handlerBehId);
							}

							if (hitStartedTouchId == touch->touchId && hitStarted) {
								hitStarted = false;
							}
						}
						else {
							atLeastOneTouch = true;
							if (!owner->HasState(StringHash(STATES_HIT))) {
#ifdef ANDROID
								if (vibrate) ofxAndroidVibrator::vibrate(50);
#endif
								// touch hasn't started but this object hasn't been hit
								owner->SetState(StringHash(STATES_HIT));
							}
							// hit started and continues
							if (hitLost) {
								hitLost = false;
								// hit started, lost and started again
								if (handlerBehId == -1) SendMessageToListeners(ACT_OBJECT_HIT_STARTED, 0, new InputEvent(touch), owner);
								else SendDirectMessage(ACT_OBJECT_HIT_STARTED, 0, new InputEvent(touch), owner, handlerBehId);
							}
							else {
								// hit started but not on first touch
								if (handlerBehId == -1) SendMessageToListeners(ACT_OBJECT_HIT_OVER, 0, new InputEvent(touch), owner);
								else SendDirectMessage(ACT_OBJECT_HIT_OVER, 0, new InputEvent(touch), owner, handlerBehId);
							}
						}
					}
					else {
						if (touch->ended && touch->touchId == hitStartedTouchId) {
							// object isn't hit and this hit has already ended
							hitStarted = false;
						}
					}
				}

				if (!atLeastOneTouch) {
					// object could lost its hit
					if (owner->HasState(StringHash(STATES_HIT))) {
						owner->ResetState(StringHash(STATES_HIT));
						hitLost = true;
						if (handlerBehId == -1) SendMessageToListeners(ACT_OBJECT_HIT_LOST, 0, nullptr, owner);
						else SendDirectMessage(ACT_OBJECT_HIT_LOST, 0, nullptr, owner, handlerBehId);
					}
				}
			}
		}

	};
}// namespace