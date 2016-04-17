#include "FloatingScene.h"
#include "CogEngine.h"
#include "Node.h"

namespace Cog {

	void FloatingScene::OnInit() {
		if (scrollEnabled) {
			SubscribeForMessages(ACT_OBJECT_HIT_OVER, ACT_OBJECT_HIT_ENDED, ACT_OBJECT_HIT_LOST, ACT_KEY_PRESSED);
		}
	}

	void FloatingScene::OnMessage(Msg& msg) {

		if (msg.HasAction(ACT_KEY_PRESSED)) {
			if (CogIsKeyPressed('m') || CogIsKeyPressed('n')) {

				// zoom for desktop version
				bool isZoomIn = CogIsKeyPressed('m');
				Vec2i mousePos = CogGetMousePosition();
				Trans& transform = owner->GetTransform();
				float scale = isZoomIn ? 1.1f : 1 / 1.1f;
				SetNewScale(scale, transform, mousePos);
			}
		}
		else if (msg.GetContextNode()->GetId() == owner->GetId()) {
			if (msg.HasAction(ACT_OBJECT_HIT_ENDED) || msg.HasAction(ACT_OBJECT_HIT_LOST)) {
				lastMousePos = Vec2i(0); // restart mouse position
				originalMousePos = Vec2i(0);
				scrollStarted = false;
				lastDistance = 0;
			}
			else if (msg.HasAction(ACT_OBJECT_HIT_OVER)) {

				spt<InputEvent> evt = msg.GetData<InputEvent>();

				// handle only the first touch
				if (evt->input->touchId == 0) {

					auto points = CogGetPressedPoints();
					if (points.size() > 1) {

						// 2 touch -> zooming
						Vec2i pos1 = points[0]->position;
						Vec2i pos2 = points[1]->position;

						int distance = Vec2i::Distance(pos1, pos2);
						int posX = min(pos1.x, pos2.x) + distance / 2;
						int posY = min(pos1.y, pos2.y) + distance / 2;

						if (lastDistance != 0 && lastDistance != distance) {
							bool isZoomIn = lastDistance > distance;

							Trans& transform = owner->GetTransform();
							float scale = ((float)distance) / lastDistance;
							SetNewScale(scale, transform, Vec2i(posX, posY));
						}

						lastDistance = distance;
					}
					else {
						// reset zooming distance
						lastDistance = 0;

						// 1 touch -> scrolling
						// set original mouse position
						if (originalMousePos == 0) {
							originalMousePos = lastMousePos;
						}

						bool isPointerOver = Vec2i::Distance(lastMousePos, originalMousePos) >= CogGetScreenWidth() / TOUCHMOVE_TOLERANCE;

						if (lastMousePos != Vec2i(0) && (scrollStarted || isPointerOver)) { // scroll prevention

							scrollStarted = true;
							Vec2i diff = evt->input->position - lastMousePos;

							if (diff.x != 0 || diff.y != 0) {
								evt->input->SetIsProcessed(true);
							}

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

	// calculates minimal possible scale so that the scene fills entire screen
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

		// check bounds
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

		if (transform.absScale.x > 1 || transform.absScale.y > 1) {
			// max scale exceeded
		//	transform.scale = 1.0f / (transform.absScale / transform.scale);
		//	transform.CalcAbsTransform(owner->GetParent()->GetTransform());
		}

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