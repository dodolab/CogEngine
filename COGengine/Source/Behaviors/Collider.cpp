
#include "Collider.h"
#include "CogEngine.h"
#include "Collision.h"
#include "Bounds.h"

namespace Cog {


	void Collider::OnStart() {
		vector<spt<Collision>> collisions = vector<spt<Collision>>();
		if (!owner->HasAttr(ATTR_COLLISIONS)) {
			owner->AddAttr(ATTR_COLLISIONS, collisions);
		}
	}


	void Collider::Update(const uint64 delta, const uint64 absolute) {

		const list<Node*>& childrens = owner->GetChildren();
		vector<spt<Collision>> collisions = owner->GetAttr<vector<spt<Collision>>>(ATTR_COLLISIONS);
		collisions.clear();

		for (list<Node*>::const_iterator it = childrens.begin(); it != childrens.end(); ++it) {

			Node* first = *it;

			bool firstInGroupA = first->GetGroups().HasState(groupA);
			bool firstInGroupB = first->GetGroups().HasState(groupB);

			if (firstInGroupA || firstInGroupB) {
				// check collisions with first 

				for (list<Node*>::const_iterator jt = it; jt != childrens.end(); ++jt) {

					Node* second = *jt;

					bool secondInGroupA = second->GetGroups().HasState(groupA);
					bool secondInGroupB = second->GetGroups().HasState(groupB);

					if ((firstInGroupA && secondInGroupB) || (firstInGroupB && secondInGroupA)) {

						// found collidable pair, check collision
						if (first->HasAttr(ATTR_BOUNDS) && first->GetAttr<Bounds*>(ATTR_BOUNDS)->Collides(*first, *second)) {
							// create collision
							spt<Collision> col = spt<Collision>(new Collision(first, second));
							collisions.push_back(col);
						}
					}
				}
			}
		}

		if (collisions.size() != 0) {
			// send info about collision
			owner->ChangeAttr(ATTR_COLLISIONS, collisions);
			SendMessage(HandlingType(Scope::SCENE, true, true), ACT_COLLISION_OCURRED, 0, nullptr, owner);
		}
	}
}// namespace