#include "Msg.h"

namespace Cog {

	int Msg::idCounter = 0;

	Msg::Msg(HandlingType handlingType, StrId action, int subaction, int behaviorId, Node* sourceObj, MsgEvent* data)
		: handlingType(handlingType), action(action), subAction(subaction), behaviorId(behaviorId),
		sourceObj(sourceObj), data(data), id(idCounter++) {
	}

}// namespace