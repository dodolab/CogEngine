#pragma once

#include "Behavior.h"
#include "Bounds.h"
#include "Collision.h"

namespace Cog {

	/**x
	* Behavior thats checks collisions Tween two groups of objects
	*/
	class Collider : public Behavior {
		OBJECT_PROTOTYPE(Collider)
	protected:
		// group of collidable objects, groupA is for collidable (bullet) and groupB for hittable (soldier)
		int groupA = 0;
		int groupB = 0;

	public:

		/**
		* Creates a new collision behavior
		* @param groupA id of group of collidable objects
		* @param groupB id of group of hittable objects
		*/
		Collider(int groupA, int groupB) :
			groupA(groupA), groupB(groupB) {

		}

		/**
		* Creates a new collision behavior
		* @param group id of group of collidable and hittable objects
		*/
		Collider(int group) : groupA(group), groupB(group) {

		}

		void Init() {
			vector<spt<Collision>> collisions = vector<spt<Collision>>();
			if (!owner->HasAttr(ATTR_COLLISIONS)) {
				owner->AddAttr(ATTR_COLLISIONS, collisions);
			}
		}


		void Update(const uint64 delta, const uint64 absolute) {

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

	};


}// namespace