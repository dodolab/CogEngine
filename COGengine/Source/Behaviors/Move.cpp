
#include "Move.h"
#include "CogEngine.h"
#include "Movement.h"

namespace Cog {

	void Move::OnStart() {
		if (!owner->HasAttr(ATTR_MOVEMENT)) {
			owner->AddAttr(ATTR_MOVEMENT, Movement());
		}
	}

	void Move::Update(const uint64 delta, const uint64 absolute) {
		Trans& transform = owner->GetTransform();
		Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);

		movement.SetVelocity(movement.GetVelocity() + movement.CalcAccelerationForce()*0.001f*delta);

		transform.localPos.x += movement.GetVelocity().x*0.001f*delta;
		transform.localPos.y += movement.GetVelocity().y*0.001f*delta;
		transform.rotation += movement.GetAngularSpeed()*0.001f*delta;

		if (infiniteBoard) {
			if (transform.absPos.x < -owner->GetShape()->GetWidth()) transform.localPos.x = CogGetScreenWidth() / transform.absScale.x*transform.scale.x;
			else if (transform.absPos.x >(CogGetScreenWidth() + 10)) transform.localPos.x = 0;

			if (transform.absPos.y < -owner->GetShape()->GetHeight()) transform.localPos.y = CogGetScreenHeight() / transform.absScale.y*transform.scale.y;
			else if (transform.absPos.y >(CogGetScreenHeight() + 10)) transform.localPos.y = 0;

		}
	}

}// namespace