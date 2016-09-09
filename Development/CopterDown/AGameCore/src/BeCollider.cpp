
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
	CIwArray<GNode*> childrens = _node->GetChildren();

	for (int i = 0; i < childrens.size(); i++){
		GNode* first = (childrens)[i];

		bool isInFirstGroup = first->GetGroups().ContainsAtLeastOne(_firstColGroups);
		bool isInSecondGroup = first->GetGroups().ContainsAtLeastOne(_secondColGroups);

		if (isInFirstGroup || isInSecondGroup){
			for (int j = i+1; j < childrens.size(); j++){
				GNode* second = (childrens)[0];
				bool isSecondInFirstGroup = second->GetGroups().ContainsAtLeastOne(_firstColGroups);
				bool isSecondInSecondGroup = second->GetGroups().ContainsAtLeastOne(_secondColGroups);

				if ((isInFirstGroup && isSecondInSecondGroup) || (isInSecondGroup && isSecondInFirstGroup)){
					Attrx<EnBounds> firstBounds = *first->FindAtt<EnBounds>(Attrs::BOUNDS);

					if (firstBounds.HasValue() && firstBounds.GetValue().Collides(*first, *second)){
						EnCollision* col = new EnCollision(first->GetId(), second->GetId());

						SendMessageNoResp(Traverses::BEH_FIRST, Actions::COLLISION_OCURRED, nullptr);
					}
				}
			}
		}
	}
}

