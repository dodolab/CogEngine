#pragma once

#include "GBehavior.h"
#include "EnFlags.h"
#include "EnBounds.h"
#include "EnCollision.h"

/**x
* Behavior thats checks collisions between two groups of objects
*/
class BeCollider : public GBehavior{
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
	BeCollider(int groupA, int groupB) : GBehavior(ElemType::MODEL),
		groupA(groupA), groupB(groupB){

	}

	/**
	* Creates a new collision behavior
	* @param group id of group of collidable and hittable objects
	*/
	BeCollider(int group) : GBehavior(ElemType::MODEL),
		groupA(group), groupB(group){

	}

	void Init(){
		vector<spt<EnCollision>> collisions = vector<spt<EnCollision>>();
		if (!owner->HasAttr(Attrs::COLLISIONS)){
			owner->AddAttr(Attrs::COLLISIONS, collisions);
		}
	}


	void Update(const uint64 delta, const uint64 absolute){

		const list<GNode*>& childrens = owner->GetChildren();
		vector<spt<EnCollision>> collisions = owner->GetAttr<vector<spt<EnCollision>>>(Attrs::COLLISIONS);
		collisions.clear();

		for (list<GNode*>::const_iterator it = childrens.begin(); it != childrens.end(); ++it){
			
			GNode* first = *it;

			bool firstInGroupA = first->GetGroups().HasState(groupA);
			bool firstInGroupB = first->GetGroups().HasState(groupB);

			if (firstInGroupA || firstInGroupB){
				// check collisions with first 

				for (list<GNode*>::const_iterator jt = it; jt != childrens.end(); ++jt){
					
					GNode* second = *jt;
					
					bool secondInGroupA = second->GetGroups().HasState(groupA);
					bool secondInGroupB = second->GetGroups().HasState(groupB);

					if ((firstInGroupA && secondInGroupB) || (firstInGroupB && secondInGroupA)){

						// found collidable pair, check collision
						if (first->HasAttr(Attrs::BOUNDS) && first->GetAttr<EnBounds*>(Attrs::BOUNDS)->Collides(*first, *second)){
							// create collision
							spt<EnCollision> col = spt<EnCollision>(new EnCollision(first, second));
							collisions.push_back(col);
						}
					}
				}
			}
		}

		if (collisions.size() != 0){
			// send info about collision
			owner->ChangeAttr(Attrs::COLLISIONS, collisions);
			SendMessage(BubblingType(ScopeType::SCENE, true, true), Actions::COLLISION_OCURRED, nullptr, owner);
		}
	}
};


