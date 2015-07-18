
#include "GBehavior.h"
#include "GNode.h"
#include "MGameEngine.h"
#include "Utils.h"

int GBehavior::idCounter = 0;


GBehavior::GBehavior(ElemType elemType) : _behState(BehState::ACTIVE_ALL),
_elemType(elemType), _id(idCounter++) {

}

void GBehavior::SendMessage(Traversation traverse, int action, void* data, GNode* target) const{
	GMsg msg(_elemType, traverse, action, _id, target, data);
	COGSendTraversationMessage(msg, target);
}

