#include "CogMsg.h"

int CogMsg::idCounter = 0;

CogMsg::CogMsg(CogBubblingType bubblingType, int action, int subaction, int behaviorId, CogNode* sourceObj, void* data)
	: bubblingType(bubblingType), action(action), subAction(subaction), behaviorId(behaviorId),
	sourceObj(sourceObj), data(data), id(idCounter++) {
}
