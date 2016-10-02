#pragma once

#include "ofUtils.h"

namespace luabridge {
	struct lua_State;
}

namespace Cog {

#define LUA_OPERATOR_TOSTRING "__tostring"
#define LUA_OPERATOR_UNMINUS "__unm"
#define LUA_OPERATOR_PLUS "__add"
#define LUA_OPERATOR_MINUS "__sub"
#define LUA_OPERATOR_MULT "__mul"
#define LUA_OPERATOR_DIV "__div"
#define LUA_OPERATOR_MOD "__mod"
#define LUA_OPERATOR_POW "__pow"
#define LUA_OPERATOR_CONCAT "__concat"
#define LUA_OPERATOR_EQ "__eq"
#define LUA_OPERATOR_LESS "__lt"
#define LUA_OPERATOR_LESSEQ "__le"

	/**
	* Class that initializes mapping for all entities that haven't their own wrapper
	*/
	class LuaMapper {
	public:
		void InitMapping(luabridge::lua_State* L);
	private:
	};

} // namespace