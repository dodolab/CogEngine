#ifndef BECOLLIDER_H
#define BECOLLIDER_H

#include "GBehavior.h"
#include "EnFlags.h"
#include "EnBounds.h"
#include "EnCollision.h"

/**
* Behavior for colliding two objects
*/
class BeCollider : public GBehavior{
protected:
	int _firstColGroup;
	int _secondColGroup;

public:
	BeCollider(int firstColGroup, int secondColGroup) : GBehavior(ElemType::MODEL, EnFlags()),
		_firstColGroup(firstColGroup), _secondColGroup(secondColGroup){

	}

	BeCollider(int colGroup) : GBehavior(ElemType::MODEL, EnFlags()),
		_firstColGroup(colGroup), _secondColGroup(colGroup){

	}


	virtual void OnMessage(GMsg& msg){

	}

	void Update(const uint64 delta, const uint64 absolute, GNode* owner){

		const list<GNode*>& childrens = owner->GetChildren();
		vector<spt<EnCollision>> collisions = vector<spt<EnCollision>>();

		for (list<GNode*>::const_iterator it = childrens.begin(); it != childrens.end(); ++it){
			GNode* first = *it;

			bool isInFirstGroup = first->GetGroups().HasState(_firstColGroup);
			bool isInSecondGroup = first->GetGroups().HasState(_secondColGroup);

			if (isInFirstGroup || isInSecondGroup){
				for (list<GNode*>::const_iterator jt = it; jt != childrens.end(); ++jt){
					GNode* second = *jt;
					bool isSecondInFirstGroup = second->GetGroups().HasState(_firstColGroup);
					bool isSecondInSecondGroup = second->GetGroups().HasState(_secondColGroup);

					if ((isInFirstGroup && isSecondInSecondGroup) || (isInSecondGroup && isSecondInFirstGroup)){

						if (first->HasAttr(Attrs::BOUNDS) && first->GetAttr<EnBounds>(Attrs::BOUNDS).Collides(*first, *second)){
							spt<EnCollision> col = spt<EnCollision>(new EnCollision(first, second));
							collisions.push_back(col);
						}
					}
				}
			}
		}

		if (collisions.size() != 0){
			if (!owner->HasAttr(Attrs::COLLISIONS)){
				owner->AddAttr(Attrs::COLLISIONS, collisions);
			}
			else owner->ChangeAttr(Attrs::COLLISIONS, collisions);

			SendMessage(Traversation(ScopeType::SCENE, true, true), Actions::COLLISION_OCURRED, nullptr, owner);
		}
	}
};







#endif