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
	EnFlags _firstColGroups;
	EnFlags _secondColGroups;

public:
	BeCollider(EnFlags firstColGroups, EnFlags secondColGroups) : GBehavior(ElemType::MODEL, EnFlags()),
		_firstColGroups(firstColGroups), _secondColGroups(secondColGroups){

	}

	BeCollider(int colGroup) : GBehavior(ElemType::MODEL, EnFlags()),
		_firstColGroups(colGroup), _secondColGroups(colGroup){

	}

	BeCollider(int firstColGroup, int secondColGroup) : GBehavior(ElemType::MODEL, EnFlags()),
		_firstColGroups(firstColGroup), _secondColGroups(secondColGroup){

	}

	virtual void OnMessage(GMsg& msg){

	}

	void Update(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner){
		list<GNode*> childrens = owner->GetChildren();

		for (list<GNode*>::iterator it = childrens.begin(); it != childrens.end(); ++it){
			GNode* first = *it;

			bool isInFirstGroup = first->GetGroups().ContainsAtLeastOne(_firstColGroups);
			bool isInSecondGroup = first->GetGroups().ContainsAtLeastOne(_secondColGroups);

			if (isInFirstGroup || isInSecondGroup){
				for (list<GNode*>::iterator jt = it; jt != childrens.end(); ++jt){
					GNode* second = *jt;
					bool isSecondInFirstGroup = second->GetGroups().ContainsAtLeastOne(_firstColGroups);
					bool isSecondInSecondGroup = second->GetGroups().ContainsAtLeastOne(_secondColGroups);

					if ((isInFirstGroup && isSecondInSecondGroup) || (isInSecondGroup && isSecondInFirstGroup)){

						if (first->HasAttr(Attrs::BOUNDS) && first->GetAttr<EnBounds>(Attrs::BOUNDS).Collides(*first, *second)){
							EnCollision* col = new EnCollision(first->GetId(), second->GetId());

							SendMessageNoResp(Traverses::BEH_FIRST, Actions::COLLISION_OCURRED, nullptr, owner);
						}
					}
				}
			}
		}
	}
};







#endif