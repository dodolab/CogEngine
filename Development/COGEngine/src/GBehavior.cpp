
#include "GBehavior.h"
#include "GNode.h"
#include "MGameEngine.h"

int GBehavior::idCounter = 0;


GBehavior::GBehavior(ElemType elemType, EnFlags msgFlags) : _behState(BehState::ACTIVE_ALL),
_elemType(elemType), _msgFlags(msgFlags), _id(idCounter++) {

}

void GBehavior::SendMessage(Traversation traverse, int action, void* data, GNode* target) const{
	GMsg msg(_elemType,traverse, action,_id, target, data);
	MEngine.storage->SendTraversationMessage(msg, target);
}

