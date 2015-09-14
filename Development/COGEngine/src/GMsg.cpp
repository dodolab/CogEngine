#include "GMsg.h"

int GMsg::idCounter = 0;

GMsg::GMsg(ElemType elemType, BubblingType bubblingType, int action, int subaction, int behaviorId, GNode* sourceObj, void* data)
	: elemType(elemType), bubblingType(bubblingType), action(action), subAction(subaction), behaviorId(behaviorId),
	sourceObj(sourceObj), data(data), id(idCounter++) {
}
