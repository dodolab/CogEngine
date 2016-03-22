#pragma once

#include "ofMain.h"
#include "CogEngine.h"


#include "catch.hpp"
using namespace Cog;


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


TEST_CASE("Lua tests", "[class]")
{

	SECTION("Simple lua Lua-C++ function call")
	{
		// create lua state
		lua_State* pl = luaL_newstate();
		// enable access to standard libs
		luaL_openlibs(pl);


		lua_register(pl, "LuaGlobalFunction", LuaGlobalFunction);

#define lua_sample "\
param = 1234 \
param = LuaGlobalFunction(param)"

		// reset stack index
		lua_settop(pl, 0);

		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			lua_pcall(pl, 0, LUA_MULTRET, 0);
		}
		lua_getglobal(pl, "param");
		// stack index starts with 1
		int param = lua_tointeger(pl, 1);
		// remove the value from the stack
		
		REQUIRE(param == 123);
		REQUIRE(lastGlobalFuncParam == 1234);

		lua_pop(pl, 1);
		lua_close(pl);
	}

	SECTION("Simple lua C++-Lua function call")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);

#define lua_sample " \
		function add(a, b) \
			return (a + b); \
		end; "
		
		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			lua_pcall(pl, 0, LUA_MULTRET, 0);
		}

		lua_getglobal(pl, "add");
		lua_pushnumber(pl, 10);
		lua_pushnumber(pl, 20);

		// 2 input parameters and 1 return parameter
		lua_call(pl, 2, 1);

		int result = (int)lua_tonumber(pl, -1);

		REQUIRE(result == 30);

		lua_pop(pl, 1);
		lua_close(pl);
	}

	SECTION("Lua classes")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);

#define lua_sample " \
function Person(salary) \
 local person = { \
  salary = salary; \
  GetSalary = function() \
  return salary; \
  end; \
  SetSalary = function(howMuch) \
  salary = howMuch; \
  end; \
  } \
  return person; \
end \
thomas = Person(12000)  "

		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			lua_pcall(pl, 0, LUA_MULTRET, 0);
		}

		LuaRef s = getGlobal(pl, "thomas");
		LuaRef getSalary = s["GetSalary"];
		LuaRef setSalary = s["SetSalary"];

		int result1 = getSalary();
		setSalary(13000);
		int result2 = getSalary();
		
		REQUIRE(result1 == 12000);
		REQUIRE(result2 == 13000);

		//don't call lua_close; LuaBridge closes it automatically
	}

	SECTION("LuaBridge arrays")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);

#define lua_sample " \
ghost = { \
  name = \"LittleGhost\", \
  sprite = \"ghost_sprite.png\", \
  size = 12 \
}"

		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			lua_pcall(pl, 0, LUA_MULTRET, 0);
		}

		LuaRef s = getGlobal(pl, "ghost");
		LuaRef nameRef = s["name"];
		LuaRef spriteRef = s["sprite"];
		LuaRef sizeRef = s["size"];

		string name = nameRef.cast<string>();
		string sprite = spriteRef.cast<string>();
		int size = sizeRef.cast<int>();

		REQUIRE(name.compare("LittleGhost") == 0);
		REQUIRE(sprite.compare("ghost_sprite.png") == 0);
		REQUIRE(size == 12);

		//don't call lua_close; LuaBridge closes it automatically
	}

	SECTION("LuaBridge C++-to-Lua")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);

		getGlobalNamespace(pl)
			.beginClass<BindObject>("BindObject")
			.addConstructor <void(*) (void)>() // necessary
			.addStaticFunction("Add", &BindObject::Add)
			.addStaticData("staticInt", &BindObject::staticInt)
			.addStaticData("staticFloat", &BindObject::staticFloat)
			.addData("floatProp", &BindObject::floatProp)
			.addProperty("intProp", &BindObject::GetIntProp, &BindObject::SetIntProp)
			.endClass();

#define lua_sample " \
 bind = BindObject() \
 bind.floatProp = 85 \
 bind.intProp = BindObject.Add(1,4) \
 BindObject.staticInt = 120 \
 BindObject.staticFloat = 7 "

		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			status = lua_pcall(pl, 0, LUA_MULTRET, 0);
			if (status != 0) {
				std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
			}
		}

		LuaRef s = getGlobal(pl, "bind");
		auto bindObj = s.cast<BindObject>();
		REQUIRE(BindObject::staticFloat == 7);
		REQUIRE(BindObject::staticInt == 120);
		REQUIRE(bindObj.floatProp == 85);
		REQUIRE(bindObj.intProp == 5);
	}

	SECTION("LuaBridge C++-to-Lua Trans")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);


		getGlobalNamespace(pl)
			.beginClass<ofVec3f>("ofVec3f")
			.addConstructor<void(*)(float, float, float)>()
			.endClass()
			.beginClass<ofVec2f>("ofVec2f")
			.addConstructor<void(*)(float, float)>()
			.endClass()
			.beginClass<Trans>("Trans")
			.addConstructor <void(*) (void)>()
			.addData("absPos", &Trans::absPos)
			.addData("absRotation", &Trans::absRotation)
			.addData("absRotationCentroid", &Trans::absRotationCentroid)
			.addData("absScale", &Trans::absScale)
			.addData("localPos", &Trans::localPos)
			.addData("rotation", &Trans::rotation)
			.addData("rotationCentroid", &Trans::rotationCentroid)
			.addData("scale", &Trans::scale)
			.addData("absPos", &Trans::absPos)
			.addFunction("SetAbsAsLocal", &Trans::SetAbsAsLocal)
			.addFunction("CalcAbsTransform", &Trans::CalcAbsTransform)
			.addFunction("CalcRotationToPosition", &Trans::CalcRotationToPosition)
			.addFunction("SetRotationToPosition", &Trans::SetRotationToPosition)
			.endClass();

#define lua_sample " \
 trans = Trans() \
 trans.absPos = ofVec3f(1,1,1) \
 trans.absRotation = 30 \
 trans.absScale = ofVec3f(20,20,20) \
 trans.rotation = 50 \
 rotPos = trans:CalcRotationToPosition(ofVec2f(10,10))"

		
		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			// execute program
			status = lua_pcall(pl, 0, LUA_MULTRET, 0);
			if (status != 0) {
				std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
			}
		}

		LuaRef s = getGlobal(pl, "trans");
		LuaRef t = getGlobal(pl, "rotPos");
		auto bindObj = s.cast<Trans>();
		float rotPos = t.cast<float>();

		REQUIRE(((int)rotPos) == 135);
		REQUIRE(bindObj.absPos == ofVec3f(1));
		REQUIRE(bindObj.absRotation == 30);
		REQUIRE(bindObj.absScale == ofVec3f(20));
		REQUIRE(bindObj.rotation == 50);
	}

	SECTION("LuaBridge C++-to-Lua object passing")
	{
		lua_State* pl = luaL_newstate();
		luaL_openlibs(pl);

		getGlobalNamespace(pl)
			.beginClass<ofVec3f>("ofVec3f")
			.addConstructor<void(*)(float, float, float)>()
			.addData("x", &ofVec3f::x)
			.addData("y", &ofVec3f::y)
			.addData("z", &ofVec3f::z)
			.endClass();

#define lua_sample " \
 testVec.x = 60 "

		ofVec3f testVec(1, 2, 3);

		int status = luaL_loadstring(pl, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
		}
		else {
			
			push(pl, &testVec);
			lua_setglobal(pl, "testVec");

			// execute program
			status = lua_pcall(pl, 0, LUA_MULTRET, 0);
			if (status != 0) {
				std::cerr << "-- " << lua_tostring(pl, -1) << std::endl;
			}
		}

		
		REQUIRE(testVec.x == 60);
	}
	
	SECTION("Luabind to CogEngine") {
		CogEngine::GetInstance().Init();
		auto scr = new LuaScripting();
		REGISTER_COMPONENT(scr);
		scr->Init();

#define lua_sample " \
mojo = { \
 attr1 = 12, \
 Update = function(delta, absolute) \
   globalFunc(12345) \
 end, \
 OnInit = function() \
 end \
 } \
mojo2 = Behavior() \
mojo2:Register(mojo) \
mojo3 = { \
 attr1 = 12, \
 Update = function(delta, absolute) \
   globalFunc(555) \
 end, \
 OnInit = function() \
 end \
 } \
mojo4 = Behavior() \
mojo4:Register(mojo3) \
\
next = { \
 Update = function(delta, absolute) \
  globalFunc(6) \
 end \
} \
mojo5 = Behavior() \
mojo5:Register(next) "

		auto L = scr->GetLua();

		getGlobalNamespace(L)
			.addFunction("globalFunc", &LuaGlobalFunction2);

		int status = luaL_loadstring(L, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		}
		else {
			// execute program
			status = lua_pcall(L, 0, LUA_MULTRET, 0);
			if (status != 0) {
				std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
			}
		}

		auto behavior = scr->behs[0];
		behavior->Update(10, 20);
		REQUIRE(lastGlobalFuncClsParam == 12345);
		scr->behs[1]->Update(30, 40);
		REQUIRE(lastGlobalFuncClsParam == 555);
		scr->behs[2]->Update(30, 40);
		REQUIRE(lastGlobalFuncClsParam == 6);
	}
	
	SECTION("Luabind to CogEngine 2") {
		CogEngine::GetInstance().Init();
		auto scr = new LuaScripting();
		REGISTER_COMPONENT(scr);
		scr->Init();

#define lua_sample " \
 test = { \
   OnInit = function() \
     test.owner:SetState(StringHash(\"TEST\")) \
     test.owner:AddAttrInt(StringHash(\"TEST_ATTR\"), 10) \
   end, \
   OnMessage = function(msg) \
     if(msg.action:Value() == StringHash(\"TEST_ACTION\"):Value()) \
     then \
       test.owner:SetState(StringHash(\"MSG_ACCEPTED\")) \
     end \
   end, \
   Update = function(delta, absolute) \
      test.owner:AddAttrInt(StringHash(\"TEST_ATTR\"), 20) \
   end \
  } \
 testBeh = Behavior() \
 testBeh:Register(test) \
 mNode = Node(\"testNode\") \
 mNode:AddBehavior(testBeh) \
 mNode:AddToActualScene() "

		// create scene
		Scene* sc = new Scene("testScene", false);
		auto stage = GETCOMPONENT(Stage);
		stage->AddScene(sc, true);
		auto L = scr->GetLua();
		int status = luaL_loadstring(L, lua_sample);
		if (status != 0) {
			std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		}
		else {
			// execute program
			status = lua_pcall(L, 0, LUA_MULTRET, 0);
			if (status != 0) {
				std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
			}
		}

		// submit all changes (Init function will be called)
		sc->GetSceneNode()->SubmitChanges(true);
		auto node = scr->nodes[0];
		REQUIRE(node->HasAttr(StringHash("TEST_ATTR")));
		REQUIRE(node->GetAttrInt(StringHash("TEST_ATTR")) == 10);
		// send message (owner will have MSG_ACCEPTED state set)
		scr->behs[0]->OnMessage(Msg(HandlingType(Scope::ROOT, true, true), StringHash("TEST_ACTION"), 10, 0, nullptr, nullptr));
		REQUIRE(node->HasState(StringHash("MSG_ACCEPTED")));
		// update node (owner will have TEST_ATTR set to 20)
		scr->nodes[0]->Update(10, 10);
		REQUIRE(node->GetAttrInt(StringHash("TEST_ATTR")) == 20);
	}
}