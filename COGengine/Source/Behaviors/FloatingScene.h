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
		OBJECT_PROTOTYPE(FloatingScene)
	protected:
		// if true, user will be able to scroll the scene
		bool touchEnabled = false;
		ofVec3f lastMousePos = ofVec3f(0);
		TransformMath math = TransformMath();

	public:

		FloatingScene(bool touchEnabled) : touchEnabled(touchEnabled) {

		}

		virtual void LoadFromXml(spt<ofxXml> xml) {
			if (xml->tagExists("touch_enabled")) {
				touchEnabled = xml->getBoolValue("touch_enabled", false);
			}
		}

		void Init() {
			if (touchEnabled) {
				RegisterListening(owner->GetScene(), ACT_OBJECT_HIT_OVER, ACT_OBJECT_HIT_ENDED, ACT_OBJECT_HIT_LOST);
			}
		}

		void OnMessage(Msg& msg) {
			if (msg.GetSourceObject()->GetId() == owner->GetId()) {

				if (msg.GetAction() == ACT_OBJECT_HIT_ENDED || msg.GetAction() == ACT_OBJECT_HIT_LOST) {
					lastMousePos = ofVec3f(0); // restart mouse position
				} else if (msg.GetAction() == ACT_OBJECT_HIT_OVER) {
					
					InputEvent* evt = static_cast<InputEvent*>(msg.GetData());

					// handle only the first touch
					if(evt->input->touchId == 0){
						if (lastMousePos != ofVec3f(0)) {
							ofVec3f diff = evt->input->position - lastMousePos;

							Trans& transform = owner->GetTransform();
							ofVec2f oldPos = transform.absPos;
							ofVec2f newPos = oldPos+diff;

							float shapeWidth = owner->GetShape()->GetWidth();
							float shapeHeight = owner->GetShape()->GetHeight();

							// calculate absolute width and height
							float width = shapeWidth*transform.absScale.x;
							float height = shapeHeight*transform.absScale.y;

							if (newPos.x > 0) newPos.x = 0;
							if (newPos.y > 0) newPos.y = 0;
							if (-newPos.x + CogGetScreenWidth() > (width)) newPos.x = CogGetScreenWidth() - width;
							if (-newPos.y + CogGetScreenHeight() > (height)) newPos.y = CogGetScreenHeight() - height;

							auto newLocalPos = math.CalcPosition(owner, owner->GetParent(), newPos, CalcType::ABS, 0, 0);

							transform.localPos.x = newLocalPos.x;
							transform.localPos.y = newLocalPos.y;
						}

						lastMousePos = evt->input->position;
					}
				}
			}
		}

		void Update(const uint64 delta, const uint64 absolute) {

		}
	};


}// namespace