#pragma once

#include "StrId.h"

namespace Cog {

	class Msg;
	class NodeLua;

	/**
	* Msg wrapper for Lua script
	*/
	class MsgLua {
		Msg* msg;
	public:

		MsgLua(Msg* msg):msg(msg) {
		}

		StrId GetAction() const;
		bool HasAction(StrId action) const;
		int GetParameter() const;
		int GetSenderId() const;
		NodeLua* GetContextNode() const;
	};

} // namespace