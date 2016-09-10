
#include "HitEvent.h"
#include "CogEngine.h"


namespace Cog {

	bool HitEvent::BoundingBoxHitTest(spt<BoundingBox> bbox, ofVec2f touchVector) {
		bbox->Recalc(owner);
		ofRectangle bboxRect = bbox->GetBoundingBox();

		if (bboxRect.x <= touchVector.x && (bboxRect.x + bboxRect.width) >= touchVector.x
			&& bboxRect.y <= touchVector.y && (bboxRect.y + bboxRect.height) >= touchVector.y) return true;
		return false;
	}

	bool HitEvent::ImageHitTest(spt<ofImage> image, ofVec3f testPos, bool preciseTest) {
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

	void HitEvent::Update(const uint64 delta, const uint64 absolute) {

		if (owner->HasState(StrId(STATES_HITTABLE))) {

			// get inverse matrix
			ofMatrix4x4 inverse = owner->GetTransform().CalcAbsMatrix().getInverse();

			bool atLeastOneTouch = false;

			for (InputAct* touch : CogGetPressedPoints()) {

				// calculate vector in image space
				ofVec2f touchVector = touch->position;
				ofVec3f touchTrans = ofVec3f(touchVector)*inverse;

				// do hit test
				bool hasHitTest = false;


				if (owner->HasMeshType(MeshType::IMAGE)) hasHitTest = ImageHitTest(owner->GetMesh<Image>()->GetImage(), touchTrans, preciseTest);
				else if (owner->HasMeshType(MeshType::MULTISPRITE)) {
					hasHitTest = CShapeHitTest(owner->GetMesh<MultiSpriteShape>(), touchTrans);
				}
				else if (owner->HasMeshType(MeshType::SPRITE)) {
					hasHitTest = CShapeHitTest(owner->GetMesh<MultiSpriteShape>(), touchTrans);
				}
				else if (owner->HasMeshType(MeshType::RECTANGLE)) {
					hasHitTest = CShapeHitTest(owner->GetMesh<Rectangle>(), touchTrans);
				}
				else if (owner->HasMeshType(MeshType::PLANE)) {
					hasHitTest = CShapeHitTest(owner->GetMesh<Plane>(), touchTrans);
				}
				else if (owner->HasMeshType(MeshType::BOUNDING_BOX)) {
					hasHitTest = BoundingBoxHitTest(owner->GetMesh<BoundingBox>(), touchVector);
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
						owner->SetState(StrId(STATES_HIT));
						if (handlerBehId == -1) SendMessageToListeners(ACT_OBJECT_HIT_STARTED, 0, new InputEvent(touch), owner);
						else SendDirectMessage(ACT_OBJECT_HIT_STARTED, 0, new InputEvent(touch), owner, handlerBehId);*/
					}
					else if (touch->ended) {


						owner->ResetState(StrId(STATES_HIT));
						if (hitStarted) {
							if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_ENDED, spt<InputEvent>(new InputEvent(touch)));
							else SendMessageToBehavior(ACT_OBJECT_HIT_ENDED, spt<InputEvent>(new InputEvent(touch)), handlerBehId);
						}
						else {
							// hit has been lost
							hitLost = true;
							if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_LOST, spt<InputEvent>(new InputEvent(touch)));
							else SendMessageToBehavior(ACT_OBJECT_HIT_LOST, spt<InputEvent>(new InputEvent(touch)), handlerBehId);
						}

						if (hitStartedTouchId == touch->touchId && hitStarted) {
							hitStarted = false;
						}
					}
					else {
						atLeastOneTouch = true;
						if (!owner->HasState(StrId(STATES_HIT))) {
#ifdef ANDROID
							if (vibrate) ofxAndroidVibrator::vibrate(50);
#endif
							// touch hasn't started but this object hasn't been hit
							owner->SetState(StrId(STATES_HIT));
						}
						// hit started and continues
						if (hitLost) {
							hitLost = false;
							// hit started, lost and started again
							if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_STARTED, spt<InputEvent>(new InputEvent(touch)));
							else SendMessageToBehavior(ACT_OBJECT_HIT_STARTED, spt<InputEvent>(new InputEvent(touch)), handlerBehId);
						}
						else {
							// hit started but not on first touch
							if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_OVER, spt<InputEvent>(new InputEvent(touch)));
							else SendMessageToBehavior(ACT_OBJECT_HIT_OVER, spt<InputEvent>(new InputEvent(touch)), handlerBehId);
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
				if (owner->HasState(StrId(STATES_HIT))) {
					owner->ResetState(StrId(STATES_HIT));
					hitLost = true;
					if (handlerBehId == -1) SendMessage(ACT_OBJECT_HIT_LOST);
					else SendMessageToBehavior(ACT_OBJECT_HIT_LOST, handlerBehId);
				}
			}
		}
	}
}// namespace