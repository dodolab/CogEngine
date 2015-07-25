#include "GMsg.h"

int GMsg::idCounter = 0;

GMsg::GMsg(ElemType elemType, BubblingType bubblingType, int action, int behaviorId, GNode* sourceObj, void* data)
	: elemType(elemType), bubblingType(bubblingType), action(action), behaviorId(behaviorId),
	sourceObj(sourceObj), data(data), id(idCounter++) {
}
