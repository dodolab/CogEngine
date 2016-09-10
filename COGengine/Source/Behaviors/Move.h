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
		float speed;
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

			transform.localPos.x += CogTranslateSpeed(speed)*velocity.x * delta;
			transform.localPos.y += CogTranslateSpeed(speed)*velocity.y * delta;
		}

	};

}// namespace