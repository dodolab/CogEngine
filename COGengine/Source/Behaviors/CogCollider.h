#pragma once

#include "CogBehavior.h"
#include "CogFlags.h"
#include "CogBounds.h"
#include "CogCollision.h"

namespace Cog {

	/**x
	* Behavior thats checks collisions Tween two groups of objects
	*/
	class CogCollider : public CogBehavior {
	protected:
		// group of collidable objects, groupA is for collidable (bullet) and groupB for hittable (soldier)
		int groupA;
		int groupB;

	public:

		/**
		* Creates a new collision behavior
		* @param groupA id of group of collidable objects
		* @param groupB id of group of hittable objects
		*/
		CogCollider(int groupA, int groupB) :
			groupA(groupA), groupB(groupB) {

		}

		/**
		* Creates a new collision behavior
		* @param group id of group of collidable and hittable objects
		*/
		CogCollider(int group) : groupA(group), groupB(group) {

		}

		void Init() {
			vector<spt<CogCollision>> collisions = vector<spt<CogCollision>>();
			if (!owner->HasAttr(Attrs::COLLISIONS)) {
				owner->AddAttr(Attrs::COLLISIONS, collisions);
			}
		}


		void Update(const uint64 delta, const uint64 absolute) {

			const list<CogNode*>& childrens = owner->GetChildren();
			vector<spt<CogCollision>> collisions = owner->GetAttr<vector<spt<CogCollision>>>(Attrs::COLLISIONS);
			collisions.clear();

			for (list<CogNode*>::const_iterator it = childrens.begin(); it != childrens.end(); ++it) {

				CogNode* first = *it;

				bool firstInGroupA = first->GetGroups().HasState(groupA);
				bool firstInGroupB = first->GetGroups().HasState(groupB);

				if (firstInGroupA || firstInGroupB) {
					// check collisions with first 

					for (list<CogNode*>::const_iterator jt = it; jt != childrens.end(); ++jt) {

						CogNode* second = *jt;

						bool secondInGroupA = second->GetGroups().HasState(groupA);
						bool secondInGroupB = second->GetGroups().HasState(groupB);

						if ((firstInGroupA && secondInGroupB) || (firstInGroupB && secondInGroupA)) {

							// found collidable pair, check collision
							if (first->HasAttr(Attrs::BOUNDS) && first->GetAttr<CogBounds*>(Attrs::BOUNDS)->Collides(*first, *second)) {
								// create collision
								spt<CogCollision> col = spt<CogCollision>(new CogCollision(first, second));
								collisions.push_back(col);
							}
						}
					}
				}
			}

			if (collisions.size() != 0) {
				// send info about collision
				owner->ChangeAttr(Attrs::COLLISIONS, collisions);
				SendMessage(CogBubblingType(CogScope::SCENE, true, true), Actions::COLLISION_OCURRED, 0, nullptr, owner);
			}
		}

	};


}