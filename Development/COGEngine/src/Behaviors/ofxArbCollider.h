#pragma once

#include "ofxAreBehavior.h"
#include "ofxAraFlags.h"
#include "ofxAraBounds.h"
#include "ofxAraCollision.h"

/**x
* Behavior thats checks collisions ofxArbTween two groups of objects
*/
class ofxArbCollider : public ofxAreBehavior{
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
	ofxArbCollider(int groupA, int groupB) :
		groupA(groupA), groupB(groupB){

	}

	/**
	* Creates a new collision behavior
	* @param group id of group of collidable and hittable objects
	*/
	ofxArbCollider(int group) : groupA(group), groupB(group){

	}

	void Init(){
		vector<spt<ofxAraCollision>> collisions = vector<spt<ofxAraCollision>>();
		if (!owner->HasAttr(Attrs::COLLISIONS)){
			owner->AddAttr(Attrs::COLLISIONS, collisions);
		}
	}


	void Update(const uint64 delta, const uint64 absolute){

		const list<ofxAreNode*>& childrens = owner->GetChildren();
		vector<spt<ofxAraCollision>> collisions = owner->GetAttr<vector<spt<ofxAraCollision>>>(Attrs::COLLISIONS);
		collisions.clear();

		for (list<ofxAreNode*>::const_iterator it = childrens.begin(); it != childrens.end(); ++it){
			
			ofxAreNode* first = *it;

			bool firstInGroupA = first->GetGroups().HasState(groupA);
			bool firstInGroupB = first->GetGroups().HasState(groupB);

			if (firstInGroupA || firstInGroupB){
				// check collisions with first 

				for (list<ofxAreNode*>::const_iterator jt = it; jt != childrens.end(); ++jt){
					
					ofxAreNode* second = *jt;
					
					bool secondInGroupA = second->GetGroups().HasState(groupA);
					bool secondInGroupB = second->GetGroups().HasState(groupB);

					if ((firstInGroupA && secondInGroupB) || (firstInGroupB && secondInGroupA)){

						// found collidable pair, check collision
						if (first->HasAttr(Attrs::BOUNDS) && first->GetAttr<ofxAraBounds*>(Attrs::BOUNDS)->Collides(*first, *second)){
							// create collision
							spt<ofxAraCollision> col = spt<ofxAraCollision>(new ofxAraCollision(first, second));
							collisions.push_back(col);
						}
					}
				}
			}
		}

		if (collisions.size() != 0){
			// send info about collision
			owner->ChangeAttr(Attrs::COLLISIONS, collisions);
			SendMessage(BubblingType(ScopeType::SCENE, true, true), Actions::COLLISION_OCURRED, 0, nullptr, owner);
		}
	}

};


