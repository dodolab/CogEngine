#pragma once

#include "Behavior.h"
#include "Msg.h"
#include "TransformMath.h"

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
		Vec2i lastMousePos = Vec2i(0, 0);
		int lastDistance = 0;
		Vec2i originalMousePos = Vec2i(0, 0);
		TransformMath math = TransformMath();

		bool scrollStarted = false;
	public:

		FloatingScene(bool scrollEnabled) : scrollEnabled(scrollEnabled) {

		}

		FloatingScene(Setting setting) {
			scrollEnabled = setting.GetItem("scroll_enabled").GetValBool();
		}

		void OnInit();

		void OnMessage(Msg& msg);

		// calculates minimal possible scale so that the scene fills entire screen
		float CalcMinScale(Trans& parentTransform);

		void CheckNewPosition(Trans& transform, ofVec3f& newPos);

		void SetNewPosition(Trans& transform, ofVec3f& newAbsPos);

		void SetNewScale(float scale, Trans& transform, Vec2i touchPosition);

		void Update(const uint64 delta, const uint64 absolute) {

		}
	};


}// namespace