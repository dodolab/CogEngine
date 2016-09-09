
#include "CogBehavior.h"

int CogBehavior::idCounter = 0;
 
CogBehavior::CogBehavior() : behState(CogBehState::ACTIVE_ALL),
ended(false), id(idCounter++) {

}

void CogBehavior::RegisterListening(int action1){
	RegisterListening(action1, -1, -1, -1);
}

void CogBehavior::RegisterListening(int action1, int action2){
	RegisterListening(action1, action2, -1, -1);
}

void CogBehavior::RegisterListening(int action1, int action2, int action3){
	RegisterListening(action1, action2, action3, -1);
}

void CogBehavior::RegisterListening(int action1, int action2, int action3, int action4){
	if (action1 != -1) CogRegisterListener(action1, this);
	if (action2 != -1) CogRegisterListener(action2, this);
	if (action3 != -1) CogRegisterListener(action3, this);
	if (action4 != -1) CogRegisterListener(action4, this);
}


void CogBehavior::SendMessage(CogBubblingType bubblingType, int action, int subaction, void* data, CogNode* source) const{
	CogMsg msg(bubblingType, action, subaction, id, source, data);
	CogSendMessage(msg, source);
}

void CogBehavior::SendMessageNoBubbling(int action, int subaction, void* data, CogNode* source) const{
	CogMsg msg(CogBubblingType(CogScope::DIRECT_NO_TRAVERSE, true, true), action, subaction, id, source, data);
	CogSendMessage(msg, source);
}

void CogBehavior::SendDirectMessage(int action, int subaction, void* data, CogNode* source, int targetId) const{
	CogMsg msg(CogBubblingType(CogScope::DIRECT_NO_TRAVERSE,true,true), action, subaction, id, source, data);
	CogSendDirectMessageToBehavior(msg, targetId);
}

