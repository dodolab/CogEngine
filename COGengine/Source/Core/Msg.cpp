#include "Msg.h"

namespace Cog {

	int Msg::idCounter = 0;

	Msg::Msg(BubblingType bubblingType, int action, int subaction, int behaviorId, Node* sourceObj, void* data)
		: bubblingType(bubblingType), action(action), subAction(subaction), behaviorId(behaviorId),
		sourceObj(sourceObj), data(data), id(idCounter++) {
	}

}