#pragma once

#include "Vec2i.h"
#include "Behavior.h"
#include "Msg.h"
#include "Transform.h"
#include "TransformMath.h"

namespace Cog {

	/**
	* Behavior that controls floating scene (e.g. game map) with a size much bigger than the screen size.
	* The position of the scene can be dragged or zoomed via mouse or touch event
	*/
	class FloatingScene : public Behavior {
	protected:
		// if true, user will be able to drag the scene
		bool draggingEnabled = false;
		Vec2i lastMousePos = Vec2i(0, 0);
		int lastDistance = 0;
		Vec2i originalMousePos = Vec2i(0, 0);
		TransformMath math = TransformMath();

		bool isDragging = false;
	public:

		FloatingScene() {

		}

		FloatingScene(bool draggingEnabled) : draggingEnabled(draggingEnabled) {

		}

		virtual void Load(Setting& setting) {
			draggingEnabled = setting.GetItem("scroll_enabled").GetValBool();
		}

		void OnInit();

		void OnMessage(Msg& msg);

		/**
		* Calculates minimal scale which will make the scene fit the whole screen
		*/
		float CalcMinScale(Trans& parentTransform);

		/**
		* Checks the topleft position of the scene whether it exceeds the screen boundaries,
		* and if it does, the position is corrected
		*/
		void CheckNewPosition(Trans& transform, ofVec3f& newPos);

		/**
		* Sets new topleft position of the floating scene
		*/
		void SetNewPosition(Trans& transform, ofVec3f& newAbsPos);

		/**
		* Sets new scale of the floating scene
		*/
		void SetNewScale(float scale, Trans& transform, Vec2i touchPosition);

		void Update(const uint64 delta, const uint64 absolute) {

		}
	};


}// namespace