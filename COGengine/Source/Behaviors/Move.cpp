
#include "Move.h"
#include "ofxCogEngine.h"
#include "Movement.h"
#include "Node.h"

namespace Cog {

	void Move::OnStart() {
		if (!owner->HasAttr(ATTR_MOVEMENT)) {
			owner->AddAttr(ATTR_MOVEMENT, Movement());
		}
	}

	void Move::Update(const uint64 delta, const uint64 absolute) {

		Trans& transform = owner->GetTransform();
		Movement& movement = owner->GetAttr<Movement>(ATTR_MOVEMENT);
		
		// update velocity according to all forces
		movement.SetVelocity(movement.GetVelocity() + movement.CalcForce()*0.001f*delta);
		// update transformations (velocity is measured in units per second)
		transform.localPos.x += movement.GetVelocity().x*0.001f*delta;
		transform.localPos.y += movement.GetVelocity().y*0.001f*delta;
		transform.rotation += movement.GetAngularSpeed()*0.001f*delta;

		if (infiniteBoard) {
			// Once the object arrives a boundary of the parent object, it will be teleported to the opposite boundary with the velocity negated

			if (transform.absPos.x < -owner->GetMesh()->GetWidth()) transform.localPos.x = CogGetScreenWidth() / transform.absScale.x*transform.scale.x;
			else if (transform.absPos.x >(CogGetScreenWidth() + 10)) transform.localPos.x = 0;

			if (transform.absPos.y < -owner->GetMesh()->GetHeight()) transform.localPos.y = CogGetScreenHeight() / transform.absScale.y*transform.scale.y;
			else if (transform.absPos.y >(CogGetScreenHeight() + 10)) transform.localPos.y = 0;

		}
	}

}// namespace