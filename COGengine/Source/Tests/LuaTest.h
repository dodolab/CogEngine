#pragma once

#include "ofMain.h"
#include "ofxCogEngine.h"
#include "Scene.h"
#include "Stage.h"
#include "catch.hpp"
using namespace Cog;
#include "LuaScripting.h"
#include "LuaTestAssets.h"
#include "CogConfig.h"

#ifdef LUA_SCRIPTING
TEST_CASE("Lua tests")
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
			.addFunction("CalcAngle", &Trans::CalcAngle)
			.addFunction("SetRotationAsAngleToPosition", &Trans::SetRotationAsAngleToPosition)
			.endClass();

#define lua_sample " \
 trans = Trans() \
 trans.absPos = ofVec3f(1,1,1) \
 trans.absRotation = 30 \
 trans.absScale = ofVec3f(20,20,20) \
 trans.rotation = 50 \
 rotPos = trans:CalcAngle(ofVec2f(10,10))"

		
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
	

	SECTION("Lua testing behavior")
	{
		// init engine
		ofxCogEngine::GetInstance().SetFps(20);
		ofxCogEngine::GetInstance().Init();
		// create scene
		Scene* scene = new Scene("main", false);
		ofxCogEngine::GetInstance().stage->AddScene(scene, true);
		// define node
		Node* node = new Node("node");
		// load testing behavior into lua
		auto luaScripting = ofxCogEngine::GetInstance().compStorage->GetComponent<LuaScripting>();
		luaScripting->LoadScript(LUA_TESTING_BEHAVIOR);
		// create prototype of this behavior
		auto behaviorPrototype = ofxCogEngine::GetInstance().compStorage->CreateBehaviorPrototype(StrId("TestBehavior"));
		REQUIRE(behaviorPrototype != nullptr);
		// add behavior to the node
		node->AddBehavior(behaviorPrototype);
		// create another behavior that will accept a message
		auto testBeh = new TestBehavior2();
		node->AddBehavior(testBeh);
		// add main node to the scene
		scene->GetSceneNode()->AddChild(node);
		// submit changes
		ofxCogEngine::GetInstance().stage->GetRootObject()->SubmitChanges(true);
		// simulate update
		ofxCogEngine::GetInstance().Update(16, 16);
		// send message into lua behavior
		auto msg = Msg(StrId("VARIABLE_CHANGE"), MsgObjectType::BEHAVIOR, 1, MsgObjectType::SUBSCRIBERS, nullptr, spt<MsgPayload>());
		scene->SendMessage(msg);
		// update engine
		ofxCogEngine::GetInstance().Update(16, 16);
		// check the second behavior if it has accepted message from the lua behavior
		REQUIRE(testBeh->acceptedMessage);
	}
}

#endif