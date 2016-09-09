
#include "GBehavior.h"

int GBehavior::idCounter = 0;
 
GBehavior::GBehavior() : behState(BehState::ACTIVE_ALL),
ended(false), id(idCounter++) {

}

void GBehavior::RegisterListening(int action1){
	RegisterListening(action1, -1, -1, -1);
}

void GBehavior::RegisterListening(int action1, int action2){
	RegisterListening(action1, action2, -1, -1);
}

void GBehavior::RegisterListening(int action1, int action2, int action3){
	RegisterListening(action1, action2, action3, -1);
}

void GBehavior::RegisterListening(int action1, int action2, int action3, int action4){
	if (action1 != -1) COGRegisterListener(action1, this);
	if (action2 != -1) COGRegisterListener(action2, this);
	if (action3 != -1) COGRegisterListener(action3, this);
	if (action4 != -1) COGRegisterListener(action4, this);
}


void GBehavior::SendMessage(BubblingType bubblingType, int action, int subaction, void* data, GNode* source) const{
	GMsg msg(bubblingType, action, subaction, id, source, data);
	COGSendMessage(msg, source);
}

void GBehavior::SendMessageNoBubbling(int action, int subaction, void* data, GNode* source) const{
	GMsg msg(BubblingType(ScopeType::DIRECT_NO_TRAVERSE, true, true), action, subaction, id, source, data);
	COGSendMessage(msg, source);
}

void GBehavior::SendDirectMessage(int action, int subaction, void* data, GNode* source, int targetId) const{
	GMsg msg(BubblingType(ScopeType::DIRECT_NO_TRAVERSE,true,true), action, subaction, id, source, data);
	COGSendDirectMessageToBehavior(msg, targetId);
}

