#pragma once

#include <LuaBridge.h>
#include <iostream>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include "LuaScripting.h"

struct BindObject {
public:

	BindObject() {
		staticInt = 0;
		staticFloat = 0;
	}


	static int staticInt;
	static float staticFloat;

	int intProp;
	float floatProp;

	static int Add(int x, int y) {
		return x + y;
	}

	int GetIntProp() const {
		return intProp;
	}

	void SetIntProp(int newProp) {
		this->intProp = newProp;
	}

};

int BindObject::staticInt;
float BindObject::staticFloat;

using namespace luabridge;

int lastGlobalFuncParam = 0;
int LuaGlobalFunction(lua_State *L)
{
	int argc = lua_gettop(L);

	for (int n = 1; n <= argc; ++n) {
		lastGlobalFuncParam = lua_tointeger(L, n);
	}

	lua_pushnumber(L, 123); // return value
	return 1; // number of return values
}

int lastGlobalFuncClsParam = 0;
void LuaGlobalFunction2(int param) {
	lastGlobalFuncClsParam = param;
}
