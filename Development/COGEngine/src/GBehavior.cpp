
#include "GBehavior.h"
#include "GNode.h"
#include "MGameEngine.h"
#include "Utils.h"

int GBehavior::idCounter = 0;


GBehavior::GBehavior(ElemType elemType, EnFlags msgFlags) : _behState(BehState::ACTIVE_ALL),
_elemType(elemType), _msgFlags(msgFlags), _id(idCounter++) {

}

void GBehavior::SendMessage(Traversation traverse, int action, void* data, GNode* target) const{
	COGLogDebug("Message %d from %d beh, target %s : %d",action, GetId(), target->GetTag().c_str(), target->GetSubType());

	GMsg msg(_elemType,traverse, action,_id, target, data);
	COGSendTraversationMessage(msg, target);
}

