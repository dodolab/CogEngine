#pragma once

#include <LuaBridge.h>
#include <iostream>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

#include "LuaScripting.h"
#include "NodeLua.h"
#include "BehaviorLua.h"
#include "ComponentStorage.h"
#include "Behavior.h"

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


#define LUA_TESTING_BEHAVIOR  "TestBehavior = Behavior:Extend(\"TestBehavior\")\n\
\
function TestBehavior : Constructor()\n\
self.variable = false\n\
end\n\
\
\
function TestBehavior : OnInit()\n\
self:SubscribeForMessages(StrId(\"VARIABLE_CHANGE\"))\n\
	end\n\
\
	function TestBehavior:OnMessage(msg)\n \
	if (msg:HasAction(StrId(\"VARIABLE_CHANGE\")))\n\
		then\n\
		self.variable = not self.variable\n\
		end\n\
		end\n\
\
		function TestBehavior:Update(delta, absolute)\n\
		if (self.variable)\n\
			then\n\
			self:SendMessage(StrId(\"VARIABLE_SUCCESSFULY_CHANGED\"))\n\
			self.variable = false\n\
			end\n\
			end\n"
			

class TestBehavior2 : public Behavior {
public:


	TestBehavior2() {

	}

	void OnInit() {
		SubscribeForMessages(StrId("VARIABLE_SUCCESSFULY_CHANGED"));
	}

	void OnMessage(Msg& msg) {
		if (!acceptedMessage && msg.GetAction() == "VARIABLE_SUCCESSFULY_CHANGED") {
			acceptedMessage = true;
		}
	}

public:
	bool acceptedMessage = false;
	virtual void Update(const uint64 delta, const uint64 absolute) {
		
	}
};