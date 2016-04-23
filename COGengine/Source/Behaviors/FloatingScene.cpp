#include "FloatingScene.h"
#include "ofxCogEngine.h"
#include "Node.h"

namespace Cog {

	void FloatingScene::OnInit() {
		if (draggingEnabled) {
			SubscribeForMessages(ACT_OBJECT_HIT_OVER, ACT_OBJECT_HIT_ENDED, ACT_OBJECT_HIT_LOST, ACT_KEY_PRESSED, ACT_MOUSE_SCROLLED);
		}
	}

	void FloatingScene::OnMessage(Msg& msg) {

		
		if (msg.HasAction(ACT_MOUSE_SCROLLED)) {
			Vec2i scroll = CogGetMouseScroll();

			if (scroll.y != 0) {
				Vec2i mousePos = CogGetMousePosition();
				Trans& transform = owner->GetTransform();
				float scale = 1 + scroll.y / 10.0f;
				SetNewScale(scale, transform, mousePos);
			}
		}
		else if (msg.HasContextNode() && msg.GetContextNode()->GetId() == owner->GetId()) {
			if (msg.HasAction(ACT_OBJECT_HIT_ENDED) || msg.HasAction(ACT_OBJECT_HIT_LOST)) {
				lastMousePos = Vec2i(0); // reset mouse position
				originalMousePos = Vec2i(0);
				isDragging = false;
				lastDistance = 0;
			}
			else if (msg.HasAction(ACT_OBJECT_HIT_OVER)) {

				spt<InputEvent> evt = msg.GetData<InputEvent>();

				// handle only the first touch, because the message will be send
				// twice with each touch separately
				if (evt->input->touchId == 0) {

					auto points = CogGetPressedPoints();
					if (points.size() > 1) {

						// 2 touch -> zooming
						Vec2i pos1 = points[0]->position;
						Vec2i pos2 = points[1]->position;

						// calculate distance between fingers
						int distance = Vec2i::Distance(pos1, pos2);
						// calculate center
						int posX = min(pos1.x, pos2.x) + distance / 2;
						int posY = min(pos1.y, pos2.y) + distance / 2;

						if (lastDistance != 0 && lastDistance != distance) {
							bool isZoomIn = lastDistance > distance;
							// set new scale
							Trans& transform = owner->GetTransform();
							float scale = ((float)distance) / lastDistance;
							SetNewScale(scale, transform, Vec2i(posX, posY));
						}

						lastDistance = distance;
					}
					else {
						// reset zooming distance
						lastDistance = 0;

						// 1 touch -> dragging
						// set original mouse position
						if (originalMousePos == 0) {
							originalMousePos = lastMousePos;
						}

						// tolerance is important for mobile platforms, because the finger is not so accurate as the mouse pointer;
						bool isPointerOver = Vec2i::Distance(lastMousePos, originalMousePos) >= CogGetScreenWidth() / TOUCHMOVE_TOLERANCE;

						if (lastMousePos != Vec2i(0) && (isDragging || isPointerOver)) {

							isDragging = true;
							Vec2i diff = evt->input->position - lastMousePos;

							if (diff.x != 0 || diff.y != 0) {
								evt->input->SetIsProcessed(true);
							}

							// translate the scene
							ofVec2f diffVec = ofVec2f((float)diff.x, (float)diff.y);

							Trans& transform = owner->GetTransform();
							ofVec3f oldPos = transform.absPos;
							ofVec3f newPos = oldPos + diffVec;

							SetNewPosition(transform, newPos);
						}

						lastMousePos = evt->input->position;
					}
				}
			}
		}
	}


	float FloatingScene::CalcMinScale(Trans& parentTransform) {
		float shapeWidth = owner->GetMesh()->GetWidth();
		float shapeHeight = owner->GetMesh()->GetHeight();
		
		float minScaleX = CogGetScreenWidth() / shapeWidth / parentTransform.absScale.x;
		float minScaleY = CogGetScreenHeight() / shapeHeight / parentTransform.absScale.y;

		return max(minScaleX, minScaleY);
	}

	void FloatingScene::CheckNewPosition(Trans& transform, ofVec3f& newPos) {
		float shapeWidth = owner->GetMesh()->GetWidth();
		float shapeHeight = owner->GetMesh()->GetHeight();

		// calculate absolute width and height
		float width = shapeWidth*transform.absScale.x;
		float height = shapeHeight*transform.absScale.y;

		// check boundaries
		if (newPos.x > 0) newPos.x = 0;
		if (newPos.y > 0) newPos.y = 0;
		if (-newPos.x + CogGetScreenWidth() > (width)) newPos.x = CogGetScreenWidth() - width;
		if (-newPos.y + CogGetScreenHeight() > (height)) newPos.y = CogGetScreenHeight() - height;
	}

	void FloatingScene::SetNewPosition(Trans& transform, ofVec3f& newAbsPos) {

		CheckNewPosition(transform, newAbsPos);

		// calc new local position from absolute position
		auto newLocalPos = math.CalcPosition(owner, owner->GetParent(), newAbsPos, CalcType::ABS, 0, 0);

		// set new local position
		transform.localPos.x = newLocalPos.x;
		transform.localPos.y = newLocalPos.y;
	}


	void FloatingScene::SetNewScale(float scale, Trans& transform, Vec2i touchPosition) {

		float originScale = transform.scale.x;

		// change scale and refresh transform
		transform.scale *= scale;

		// calc minimal scale so that the scene fills the whole screen
		float minScale = CalcMinScale(owner->GetParent()->GetTransform());

		if (transform.scale.x < minScale) transform.scale = ofVec3f(minScale);

		transform.CalcAbsTransform(owner->GetParent()->GetTransform());

		ofVec3f absPos = transform.absPos;
		// calculate centroid shift
		ofVec2f centroidA = ofVec2f(touchPosition.x - absPos.x, touchPosition.y - absPos.y);
		ofVec2f centroidDiff = centroidA*(transform.scale / originScale - 1);

		// check and fix new position if it has already exceeded the bounds 
		ofVec3f newAbsPos = absPos - centroidDiff;
		CheckNewPosition(transform, newAbsPos);

		// calculate local position from absolute position
		ofVec2f newLocalPos = math.CalcPosition(owner, owner->GetParent(), ofVec2f(newAbsPos.x, newAbsPos.y), CalcType::ABS, 0, 0);
		transform.localPos.x = newLocalPos.x;
		transform.localPos.y = newLocalPos.y;
	}

}// namespace