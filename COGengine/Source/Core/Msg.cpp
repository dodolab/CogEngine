#include "Msg.h"

namespace Cog {

	int Msg::idCounter = 0;

	Msg::Msg(BubblingType bubblingType, StringHash action, int subaction, int behaviorId, Node* sourceObj, MsgEvent* data)
		: bubblingType(bubblingType), action(action), subAction(subaction), behaviorId(behaviorId),
		sourceObj(sourceObj), data(data), id(idCounter++) {
	}

}// namespace