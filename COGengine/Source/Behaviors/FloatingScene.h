#pragma once

#include "Behavior.h"
#include "TransformMath.h"
#include "Node.h"
#include "Msg.h"

namespace Cog {

	/**x
	* Behavior that controls floating scene (e.g. game world) with the size much bigger than screen size
	* World has to be specified in some measureable units
	*/
	class FloatingScene : public Behavior {
		OBJECT_PROTOTYPE_INIT(FloatingScene)
	protected:
		// if true, user will be able to scroll the scene
		bool scrollEnabled = false;
		Vec2i lastMousePos = Vec2i(0,0);
		int lastDistance = 0;
		Vec2i originalMousePos = Vec2i(0, 0);
		TransformMath math = TransformMath();
		// how far should the pointer go before scrolling will start (proper for mobile phones)
		int scrollPrevention = 0; 

	public:

		FloatingScene(bool scrollEnabled, int scrollPrevention) : scrollEnabled(scrollEnabled), scrollPrevention(scrollPrevention){

		}

		FloatingScene(Setting setting) {
			scrollEnabled = setting.GetItem("scroll_enabled").GetValBool();
			scrollPrevention = setting.GetItem("scroll_prevention").GetValInt();
		}

		void Init() {
			if (scrollEnabled) {
				RegisterListening(owner->GetScene(), ACT_OBJECT_HIT_OVER, ACT_OBJECT_HIT_ENDED, ACT_OBJECT_HIT_LOST, ACT_KEY_PRESSED);
			}
		}

		void OnMessage(Msg& msg) {

			if (msg.GetAction() == ACT_KEY_PRESSED) {
				if (CogIsKeyPressed('m') || CogIsKeyPressed('n')) {

					// zoom for desktop version
					bool isZoomIn = CogIsKeyPressed('m');
					Vec2i mousePos = CogGetMousePosition();
					Trans& transform = owner->GetTransform();
					float scale = isZoomIn ? 1.1f : 1 / 1.1f;
					SetNewScale(scale, transform, mousePos);
				}
			}
			else if (msg.GetSourceObject()->GetId() == owner->GetId()) {
				if (msg.GetAction() == ACT_OBJECT_HIT_ENDED || msg.GetAction() == ACT_OBJECT_HIT_LOST) {
					lastMousePos = Vec2i(0); // restart mouse position
					originalMousePos = Vec2i(0);
				} else if (msg.GetAction() == ACT_OBJECT_HIT_OVER) {
					
					InputEvent* evt = static_cast<InputEvent*>(msg.GetData());

					// handle only the first touch
					if(evt->input->touchId == 0){

						auto points = CogGetPressedPoints();
						if (points.size() > 1) {

							// 2 touch -> zooming
							Vec2i pos1 = points[0]->position;
							Vec2i pos2 = points[1]->position;

							int distance = Vec2i::Distance(pos1, pos2);
							int posX = min(pos1.x, pos2.x) + distance/2;
							int posY = min(pos1.y, pos2.y) + distance/2;

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

							if (lastMousePos != Vec2i(0) && Vec2i::Distance(lastMousePos, originalMousePos) >= scrollPrevention) { // scroll prevention
								
								Vec2i diff = evt->input->position - lastMousePos;
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
		float CalcMinScale(Trans& parentTransform) {
			float shapeWidth = owner->GetShape()->GetWidth();
			float shapeHeight = owner->GetShape()->GetHeight();

			float minScaleX = CogGetScreenWidth() / shapeWidth / parentTransform.absScale.x;
			float minScaleY = CogGetScreenHeight() / shapeHeight / parentTransform.absScale.y;

			return max(minScaleX, minScaleY);
		}

		void CheckNewPosition(Trans& transform, ofVec3f& newPos) {
			float shapeWidth = owner->GetShape()->GetWidth();
			float shapeHeight = owner->GetShape()->GetHeight();

			// calculate absolute width and height
			float width = shapeWidth*transform.absScale.x;
			float height = shapeHeight*transform.absScale.y;

			// check bounds
			if (newPos.x > 0) newPos.x = 0;
			if (newPos.y > 0) newPos.y = 0;
			if (-newPos.x + CogGetScreenWidth() > (width)) newPos.x = CogGetScreenWidth() - width;
			if (-newPos.y + CogGetScreenHeight() > (height)) newPos.y = CogGetScreenHeight() - height;
		}

		void SetNewPosition(Trans& transform, ofVec3f& newAbsPos) {
			
			CheckNewPosition(transform, newAbsPos);

			// calc new local position from absolute position
			auto newLocalPos = math.CalcPosition(owner, owner->GetParent(), newAbsPos, CalcType::ABS, 0, 0);

			// set new local position
			transform.localPos.x = newLocalPos.x;
			transform.localPos.y = newLocalPos.y;
		}

		float CalcScaleByDistance(int distanceDiff) {

		}

		void SetNewScale(float scale, Trans& transform, Vec2i touchPosition) {

			float originScale = transform.scale.x;

			// change scale and refresh transform
			transform.scale *= scale;

			// calc minimal scale so that the scene fills the whole screen
			float minScale = CalcMinScale(owner->GetParent()->GetTransform());

			if (transform.scale.x < minScale) transform.scale = ofVec3f(minScale);
			
			transform.CalcAbsTransform(owner->GetParent()->GetTransform());

			if (transform.absScale.x > 1 || transform.absScale.y > 1) {
				// max scale exceeded
				transform.scale = 1.0f / (transform.absScale / transform.scale);
				transform.CalcAbsTransform(owner->GetParent()->GetTransform());
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

		void Update(const uint64 delta, const uint64 absolute) {
			
		}
	};


}// namespace