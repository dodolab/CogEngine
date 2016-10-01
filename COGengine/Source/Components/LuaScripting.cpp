#include "LuaScripting.h"

#include "Definitions.h"
#include "MsgLua.h"
#include "BehaviorLua.h"
#include "NodeLua.h"
#include "SceneLua.h"
#include "FacadeLua.h"
#include "ComponentStorage.h"
#include "Resources.h"

namespace Cog {

	void LuaScripting::OnInit() {

		COGLOGDEBUG("Lua", "Initializing lua binding");

		L = luaL_newstate();
		luaL_openlibs(L);

		// ofVec2f
		getGlobalNamespace(L)
			.beginClass<ofVec2f>("ofVec2f")
			.addConstructor<void(*)(float, float)>()
			.addData("x", &ofVec2f::x)
			.addData("y", &ofVec2f::y)
			.endClass();

		// ofVec3f
		getGlobalNamespace(L)
			.beginClass<ofVec3f>("ofVec3f")
			.addConstructor<void(*)(float, float, float)>()
			.addData("x", &ofVec3f::x)
			.addData("y", &ofVec3f::y)
			.addData("z", &ofVec3f::z)
			.endClass();

		// Vec2i
		getGlobalNamespace(L)
			.beginClass<Vec2i>("Vec2i")
			.addConstructor<void(*)(int, int)>()
			.addData("x", &Vec2i::x)
			.addData("y", &Vec2i::y)
			.endClass();

		// Trans
		getGlobalNamespace(L)
			.beginClass<Trans>("Trans")
			.addConstructor <void(*) (void)>()
			.LUA_REGDATA(Trans, absPos)
			.LUA_REGDATA(Trans, absRotation)
			.LUA_REGDATA(Trans, absRotationCentroid)
			.LUA_REGDATA(Trans, absScale)
			.LUA_REGDATA(Trans, localPos)
			.LUA_REGDATA(Trans, rotation)
			.LUA_REGDATA(Trans, rotationCentroid)
			.LUA_REGDATA(Trans, scale)
			.LUA_REGDATA(Trans, absPos)
			.LUA_REGFUNC(Trans, SetAbsAsLocal)
			.LUA_REGFUNC(Trans, CalcAbsTransform)
			.LUA_REGFUNC(Trans, CalcAngle)
			.LUA_REGFUNC(Trans, SetRotationAsAngleToPosition)
			.endClass();

		// StrId
		getGlobalNamespace(L)
			.beginClass<StrId>("StrId")
			.addConstructor<void(*)(string)>()
			.LUA_REGFUNC(StrId, GetValue)
			.endClass();

		// Msg proxy
		getGlobalNamespace(L)
			.beginClass<MsgLua>("Msg")
			.addProperty("action", &MsgLua::GetAction)
			.addProperty("senderId", &MsgLua::GetSenderId)
			.addProperty("contextNode", &MsgLua::GetContextNode)
			.addProperty("parameter", &MsgLua::GetParameter)
			.addFunction("HasAction", &MsgLua::HasAction)
			.endClass();

		// Behavior proxy
		getGlobalNamespace(L)
			.beginClass<BehaviorLua>("Behavior")
			.addConstructor<void(*)(void)>()
			.addFunction("SubscribeForMessages", &BehaviorLua::SubscribeForMessagesLua)
			.addCFunction("Register", &BehaviorLua::RegisterCt)
			.addFunction("SendMessage", &BehaviorLua::SendMessage)
			.addFunction("GetScene", &BehaviorLua::GetScene)
			.addData("owner", &BehaviorLua::ownerLua)
			.endClass();

		// Flags proxy
		getGlobalNamespace(L)
			.beginClass<Flags>("Flags")
			.addConstructor<void(*)(StrId)>()
			.addFunction("HasState", &Flags::HasState)
			.addFunction("SetState", &Flags::SetState)
			.addFunction("SwitchState", &Flags::SwitchState)
			.addFunction("ResetState", &Flags::ResetState)
			.endClass();

		// Node proxy
		getGlobalNamespace(L)
			.beginClass<NodeLua>("Node")
			.addConstructor<void(*)(string)>()
			.addProperty("id", &NodeLua::GetId)
			.addProperty("parent", &NodeLua::GetParent, &NodeLua::SetParent)
			.addProperty("tag", &NodeLua::GetTag)
			.addProperty("transform", &NodeLua::GetTransform, &NodeLua::SetTransform)
			.addFunction("AddBehavior", &NodeLua::AddBehavior)
			.addFunction("AddChild", &NodeLua::AddChild)
			.addFunction("Draw", &NodeLua::Draw)
			.addFunction("HasAttr", &NodeLua::HasAttr)
			.addFunction("RemoveAttr", &NodeLua::RemoveAttr)
			.addFunction("RemoveBehavior", &NodeLua::RemoveBehavior)
			.addFunction("RemoveChild", &NodeLua::RemoveChild)
			.addFunction("SubmitChanges", &NodeLua::SubmitChanges)
			.addFunction("Update", &NodeLua::Update)
			.addFunction("UpdateTransform", &NodeLua::UpdateTransform)
			.addFunction("HasGroups", &NodeLua::HasGroups)
			.addProperty("groups", &NodeLua::GetGroups, &NodeLua::SetGroups)
			.addFunction("IsInGroup", &NodeLua::IsInGroup)
			.addFunction("SetGroup", &NodeLua::SetGroup)
			.addFunction("UnsetGroup", &NodeLua::UnsetGroup)
			.addFunction("HasStates", &NodeLua::HasStates)
			.addProperty("states", &NodeLua::GetStates, &NodeLua::SetStates)
			.addFunction("HasState", &NodeLua::HasState)
			.addFunction("SetState", &NodeLua::SetState)
			.addFunction("ResetState", &NodeLua::ResetState)
			.addFunction("SwitchState", &NodeLua::SwitchState)
			.addFunction("AddAttrString", &NodeLua::AddAttrString)
			.addFunction("AddAttrInt", &NodeLua::AddAttrInt)
			.addFunction("AddAttrFloat", &NodeLua::AddAttrFloat)
			.addFunction("AddAttrVector2f", &NodeLua::AddAttrVector2f)
			.addFunction("AddAttrVector3f", &NodeLua::AddAttrVector3f)
			.addFunction("AddAttrVec2i", &NodeLua::AddAttrVec2i)
			.addFunction("GetAttrString", &NodeLua::GetAttrString)
			.addFunction("GetAttrInt", &NodeLua::GetAttrInt)
			.addFunction("GetAttrFloat", &NodeLua::GetAttrFloat)
			.addFunction("GetAttrVector2f", &NodeLua::GetAttrVector2f)
			.addFunction("GetAttrVector3f", &NodeLua::GetAttrVector3f)
			.addFunction("GetAttrVec2i", &NodeLua::GetAttrVec2i)
			.addFunction("AddToActualScene", &NodeLua::AddToActualScene)
			.addFunction("GetTextMesh", &NodeLua::GetTextMesh)
			.addFunction("GetImageMesh", &NodeLua::GetImageMesh)
			.addFunction("SetImageMesh", &NodeLua::SetImageMesh)
			.endClass();

		// Text mesh
		getGlobalNamespace(L)
			.beginClass<Text>("Text")
			.addFunction("AppendLine", &Text::AppendLine)
			.addFunction("AppendText", &Text::AppendText)
			.addFunction("GetHeight", &Text::GetHeight)
			.addFunction("GetText", &Text::GetText)
			.addFunction("GetTextHeight", &Text::GetTextHeight)
			.addFunction("GetTextWidth", &Text::GetTextWidth)
			.addFunction("GetWidth", &Text::GetWidth)
			.addFunction("SetText", &Text::SetText)
			.endClass();

		// Image mesh
		getGlobalNamespace(L)
			.beginClass<Image>("Image")
			.addFunction("GetHeight", &Image::GetHeight)
			.addFunction("GetWidth", &Image::GetWidth)
			.endClass();

		// Scene proxy
		getGlobalNamespace(L)
			.beginClass<SceneLua>("Scene")
			.addFunction("FindBehaviorById", &SceneLua::FindBehaviorById)
			.addFunction("FindNodeById", &SceneLua::FindNodeById)
			.addFunction("FindNodeBySecondaryId", &SceneLua::FindNodeBySecondaryId)
			.addFunction("FindNodeByTag", &SceneLua::FindNodeByTag)
			.addProperty("name", &SceneLua::GetName, &SceneLua::SetName)
			.addFunction("GetSceneNode", &SceneLua::GetSceneNode)
			.addFunction("Initialized", &SceneLua::Initialized)
			.addFunction("Loaded", &SceneLua::Loaded)
			.addFunction("SendMessage", &SceneLua::SendMessage)
			.endClass();

		// Facade
		getGlobalNamespace(L)
			.addFunction("CogGetAbsoluteTime", &FacadeLua::CogGetAbsoluteTime)
			.addFunction("CogGetFrameCounter", &FacadeLua::CogGetFrameCounter)
			.addFunction("CogGetMousePosition", &FacadeLua::CogGetMousePosition)
			.addFunction("CogGetMouseScroll", &FacadeLua::CogGetMouseScroll)
			.addFunction("CogGetScreenAspectRatio", &FacadeLua::CogGetScreenAspectRatio)
			.addFunction("CogGetScreenHeight", &FacadeLua::CogGetScreenHeight)
			.addFunction("CogGetScreenSize", &FacadeLua::CogGetScreenSize)
			.addFunction("CogGetScreenWidth", &FacadeLua::CogGetScreenWidth)
			.addFunction("CogGetVirtualAspectRatio", &FacadeLua::CogGetVirtualAspectRatio)
			.addFunction("CogGetVirtualHeight", &FacadeLua::CogGetVirtualHeight)
			.addFunction("CogGetVirtualScreenSize", &FacadeLua::CogGetVirtualScreenSize)
			.addFunction("CogGetVirtualWidth", &FacadeLua::CogGetVirtualWidth)
			.addFunction("CogIsKeyPressed", &FacadeLua::CogIsKeyPressed)
			.addFunction("CogLogDebug", &FacadeLua::CogLogDebug)
			.addFunction("CogLogError", &FacadeLua::CogLogError)
			.addFunction("CogLogInfo", &FacadeLua::CogLogInfo)
			.addFunction("CogSwitchBackToScene", &FacadeLua::CogSwitchBackToScene)
			.addFunction("CogSwitchToScene", &FacadeLua::CogSwitchToScene)
			.addFunction("CogStopAllSounds", &FacadeLua::CogStopAllSounds);


		LoadAllScripts();
	}

	void LuaScripting::LoadAllScripts() {
		// preload scripts
		string scriptsPath = ofToDataPath(PATH_SCRIPTS);

		ofDirectory dir(scriptsPath);

		if (dir.exists()) {
			auto files = dir.getFiles();

			for (auto file : files) {

				string name = file.getFileName();
				
				COGLOGDEBUG("Lua", "Loading script %s", name.c_str());

				int status = luaL_loadfile(L, file.getAbsolutePath().c_str());

				if (status != 0) {
					CogLogError("Lua", lua_tostring(L, -1));
				}
				else {
					// run script
					status = lua_pcall(L, 0, LUA_MULTRET, 0);
					if (status != 0) {
						CogLogError("Lua", lua_tostring(L, -1));
					}
				}
			}
		}
	}

} // namespace