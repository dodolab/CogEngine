#pragma once

#include "Behavior.h"
#include "Msg.h"
#include "Node.h"

namespace Cog {

	/**x
	* Behavior for simple movement
	*/
	class Move : public Behavior {
		OBJECT_PROTOTYPE(Move)
	protected:
		float speed = 0;
	public:

		/**
		* Creates a new behavior for simple movement
		* @param speed movement speed
		*/
		Move(float speed) : speed(speed) {

		}

		void Init() {
			if (!owner->HasAttr(ATTR_VELOCITY)) {
				owner->AddAttr(ATTR_VELOCITY, ofVec3f());
			}
		}

		void Update(const uint64 delta, const uint64 absolute) {
			Trans& transform = owner->GetTransform();

			ofVec3f velocity = owner->GetAttr<ofVec3f>(ATTR_VELOCITY);
			transform.localPos.x += speed*0.001f*CogGetScreenWidth()*velocity.x * delta;
			transform.localPos.y += speed*0.001f*CogGetScreenWidth()*velocity.y * delta;
		}

	};

}// namespace