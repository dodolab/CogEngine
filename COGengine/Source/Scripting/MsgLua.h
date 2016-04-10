#pragma once

#include "StrId.h"

namespace Cog {

	class Msg;
	class NodeLua;

	class MsgLua {
		Msg* msg;
	public:

		MsgLua(Msg* msg):msg(msg) {
		}

		StrId GetAction() const;
		int GetSubAction() const;
		int GetBehaviorId() const;
		NodeLua* GetSourceObj() const;
	};

} // namespace