
#include "BeCollider.h"

BeCollider::BeCollider(EnFlags firstColGroups, EnFlags secondColGroups) : ABehavior(ElemType::MODEL, EnFlags()),
_firstColGroups(firstColGroups), _secondColGroups(secondColGroups){
	
}

BeCollider::BeCollider(int colGroup) : BeCollider(EnFlags(colGroup), EnFlags(colGroup)){

}

BeCollider::BeCollider(int firstColGroup, int secondColGroup) : BeCollider(EnFlags(firstColGroup), EnFlags(secondColGroup)){

}

void BeCollider::OnMessage(Msg& msg){

}

void BeCollider::Update(const uint64 delta, const uint64 absolute){
	CIwList<GNode*> childrens = _owner->GetChildren();

	for (CIwList<GNode*>::iterator it = childrens.begin(); it != childrens.end(); ++it){
		GNode* first = *it;

		bool isInFirstGroup = first->GetGroups().ContainsAtLeastOne(_firstColGroups);
		bool isInSecondGroup = first->GetGroups().ContainsAtLeastOne(_secondColGroups);

		if (isInFirstGroup || isInSecondGroup){
			for (CIwList<GNode*>::iterator jt = it; jt != childrens.end(); ++jt){
				GNode* second = *jt;
				bool isSecondInFirstGroup = second->GetGroups().ContainsAtLeastOne(_firstColGroups);
				bool isSecondInSecondGroup = second->GetGroups().ContainsAtLeastOne(_secondColGroups);

				if ((isInFirstGroup && isSecondInSecondGroup) || (isInSecondGroup && isSecondInFirstGroup)){
					 
					if (first->HasAttr(Attrs::BOUNDS) && first->FindAttrR<EnBounds>(Attrs::BOUNDS).Collides(*first, *second)){
						EnCollision* col = new EnCollision(first->GetId(), second->GetId());

						SendMessageNoResp(Traverses::BEH_FIRST, Actions::COLLISION_OCURRED, nullptr);
					}
				}
			}
		}
	}
}

