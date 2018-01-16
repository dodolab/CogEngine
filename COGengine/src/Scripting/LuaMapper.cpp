#include "LuaMapper.h"
#include "ofUtils.h"
#include "Mesh.h"
#include <LuaBridge.h>
#include "Vec2i.h"
#include "Transform.h"
#include "StrId.h"
#include "Flags.h"
#include "ResourcesMgr.h"
#include "BehaviorLua.h"
#include "FacadeLua.h"
#include "Settings.h"
#include "TransformMath.h"
#include "Scene.h"
#include "Stage.h"
#include "TransformBuilder.h"
#include "Dynamics.h"
#include "AttribAnimator.h"

using namespace luabridge;


namespace Cog {

	void LuaMapper::InitLuaMapping(lua_State* L) {

		// static functions
		luabridge::getGlobalNamespace(L)
			.addFunction("ofRandom", static_cast<float(*)(float, float)>(&ofRandom))
			.addFunction("floor", static_cast<double(*)(double)>(&floor))
			.addFunction("CheckTime", &CheckTimeUnsigned)
			.addFunction("Modulo", &Modulo);

		// ofVec2f
		luabridge::getGlobalNamespace(L)
			.beginClass<ofVec2f>("ofVec2f")
			.addConstructor<void(*)(float, float)>()
			.addFunction(LUA_OPERATOR_PLUS, static_cast<ofVec2f(ofVec2f::*)(const ofVec2f &)const>(&ofVec2f::operator+))
			.addFunction(LUA_OPERATOR_MULT, static_cast<ofVec2f(ofVec2f::*)(const ofVec2f &)const>(&ofVec2f::operator*))
			.addFunction(LUA_OPERATOR_EQ, static_cast<bool(ofVec2f::*)(const ofVec2f &)const>(&ofVec2f::operator==))
			.addData("x", &ofVec2f::x)
			.addData("y", &ofVec2f::y)
			.addFunction("length", &ofVec2f::length)
			.addFunction("lengthSquared", &ofVec2f::lengthSquared)
			.endClass();

		// ofVec3f
		luabridge::getGlobalNamespace(L)
			.beginClass<ofVec3f>("ofVec3f")
			.addConstructor<void(*)(float, float, float)>()
			.addFunction(LUA_OPERATOR_PLUS, static_cast<ofVec3f(ofVec3f::*)(const ofVec3f &)const>(&ofVec3f::operator+))
			.addFunction(LUA_OPERATOR_MULT, static_cast<ofVec3f(ofVec3f::*)(const ofVec3f &)const>(&ofVec3f::operator*))
			.addFunction(LUA_OPERATOR_EQ, static_cast<bool(ofVec3f::*)(const ofVec3f &)const>(&ofVec3f::operator==))
			.addData("x", &ofVec3f::x)
			.addData("y", &ofVec3f::y)
			.addData("z", &ofVec3f::z)
			.addFunction("length", &ofVec3f::length)
			.addFunction("lengthSquared", &ofVec3f::lengthSquared)
			.endClass();

		// Bounding Box
		luabridge::getGlobalNamespace(L)
			.beginClass<BoundingBox>("BoundingBox")
			.addConstructor<void(*)()>()
			.addFunction("GetCenter", &BoundingBox::GetCenter)
			.addFunction("GetSize", &BoundingBox::GetSize)
			.addFunction("HorizontalIntersection", &BoundingBox::HorizontalIntersection)
			.addFunction("Intersects", &BoundingBox::Intersects)
			.addFunction("VerticalIntersection", &BoundingBox::VerticalIntersection)
			.addData("topLeft", &BoundingBox::topLeft)
			.addData("bottomLeft", &BoundingBox::bottomLeft)
			.addData("topRight", &BoundingBox::topRight)
			.addData("bottomRight", &BoundingBox::bottomRight)
			.endClass();

		// Mesh 
		luabridge::getGlobalNamespace(L)
			.beginClass<Mesh>("Mesh")
			.addFunction("IsVisible", &Mesh::IsVisible)
			.addFunction("SetIsVisible", &Mesh::SetIsVisible)
			.addFunction("GetBoundingBox", &Mesh::GetBoundingBox)
			.addProperty("isVisible", &Mesh::IsVisible, &Mesh::SetIsVisible)
			.endClass();

		// Text mesh
		luabridge::getGlobalNamespace(L)
			.deriveClass<Text, Mesh>("Text")
			.addProperty("text", &Text::GetText, &Text::SetText)
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
		luabridge::getGlobalNamespace(L)
			.deriveClass<Image, Mesh>("Image")
			.addFunction("GetHeight", &Image::GetHeight)
			.addProperty("height", &Image::GetHeight)
			.addFunction("GetWidth", &Image::GetWidth)
			.addProperty("width", &Image::GetWidth)
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<ofImage>("ofImage")
			.addFunction("getWidth", &ofImage::getWidth)
			.addFunction("getHeigt", &ofImage::getHeight)
			.endClass();


		// Sprite
		luabridge::getGlobalNamespace(L)
			.beginClass<Sprite>("Sprite")
			.addProperty("frame", &Sprite::GetFrame, &Sprite::SetFrame)
			.addFunction("GetFrame", &Sprite::GetFrame)
			.addFunction("SetFrame", &Sprite::SetFrame)
			.addProperty("offsetX", &Sprite::GetOffsetX)
			.addFunction("GetOffsetX", &Sprite::GetOffsetX)
			.addProperty("offsetY", &Sprite::GetOffsetY)
			.addFunction("GetOffsetY", &Sprite::GetOffsetY)
			.addProperty("width", &Sprite::GetWidth)
			.addFunction("GetWidth", &Sprite::GetWidth)
			.addProperty("height", &Sprite::GetHeight)
			.addFunction("GetHeight", &Sprite::GetHeight)
			.addFunction("GetTransform", &Sprite::GetTransform)
			.addFunction("CalcBoundingBox", &Sprite::CalcBoundingBoxPtr)
			.endClass();

		// Sprite mesh 
		luabridge::getGlobalNamespace(L)
			.deriveClass<SpriteMesh, Mesh>("SpriteMesh")
			.addFunction("GetLayerName", &SpriteMesh::GetLayerName)
			.addFunction("GetSprite", &SpriteMesh::GetSprite)
			.endClass();

		// Multi Sprite mesh 
		luabridge::getGlobalNamespace(L)
			.deriveClass<MultiSpriteMesh, Mesh>("MultiSpriteMesh")
			.addFunction("GetSprite", &MultiSpriteMesh::GetSprite)
			.addFunction("RemoveSprite", &MultiSpriteMesh::RemoveSprite)
			.addFunction("GetSpritesNum", &MultiSpriteMesh::GetSpritesNum)
			.endClass();


		// Vec2i
		luabridge::getGlobalNamespace(L)
			.beginClass<Vec2i>("Vec2i")
			.addConstructor<void(*)(int, int)>()
			.addData("x", &Vec2i::x)
			.addData("y", &Vec2i::y)
			.addFunction(LUA_OPERATOR_PLUS, static_cast<Vec2i(Vec2i::*)(const Vec2i &)const>(&Vec2i::operator+))
			.addFunction(LUA_OPERATOR_EQ, static_cast<bool(Vec2i::*)(const Vec2i &)const>(&Vec2i::operator==))
			.endClass();

		// TransformBuilder
		luabridge::getGlobalNamespace(L)
			.beginClass<TransformBuilder>("TransformBuilder")
			.addConstructor<void(*)()>()
			.addFunction<TransformBuilder&(TransformBuilder::*)(ofVec2f)>("AbsolutePosition", &TransformBuilder::AbsolutePosition)
			.addFunction<TransformBuilder&(TransformBuilder::*)(ofVec2f)>("AbsoluteScale", &TransformBuilder::AbsoluteScale)
			.addFunction<TransformBuilder&(TransformBuilder::*)(ofVec2f)>("Anchor", &TransformBuilder::Anchor)
			.addFunction<TransformBuilder&(TransformBuilder::*)(ofVec2f)>("LocalPosition", &TransformBuilder::LocalPosition)
			.addFunction<TransformBuilder&(TransformBuilder::*)(ofVec2f)>("LocalScale", &TransformBuilder::LocalScale)
			.addFunction<TransformBuilder&(TransformBuilder::*)(ofVec2f)>("RelativePosition", &TransformBuilder::RelativePosition)
			.addFunction<TransformBuilder&(TransformBuilder::*)(ofVec2f)>("RelativeScale", &TransformBuilder::RelativeScale)
			.addFunction("Rotation", &TransformBuilder::Rotation)
			.addFunction<TransformBuilder&(TransformBuilder::*)(ofVec2f)>("RotationCenter", &TransformBuilder::RotationCenter)
			.addFunction("ZIndex", &TransformBuilder::ZIndex)
			.addFunction("Calculate", &TransformBuilder::Calculate)
			.addFunction<void(TransformBuilder::*)(Node*)>("Build", &TransformBuilder::Build)
			.addFunction("BuildAndReset", &TransformBuilder::BuildAndReset)
			.endClass();

		// Dynamics
		luabridge::getGlobalNamespace(L)
			.beginClass<Dynamics>("Dynamics")
			.addFunction("GetAcceleration", &Dynamics::GetAcceleration)
			.addFunction("SetAcceleration", &Dynamics::SetAcceleration)
			.addFunction("GetVelocity", &Dynamics::GetVelocity)
			.addFunction("SetVelocity", &Dynamics::SetVelocity)
			.addProperty("angularSpeed", &Dynamics::GetAngularSpeed, &Dynamics::SetAngularSpeed)
			.addFunction("GetAngularSpeed", &Dynamics::GetAngularSpeed)
			.addFunction("SetAngularSpeed", &Dynamics::SetAngularSpeed)
			.endClass();

		// Trans
		luabridge::getGlobalNamespace(L)
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

		// TransformEnt
		luabridge::getGlobalNamespace(L)
			.beginClass<TransformEnt>("TransformEnt")
			.addConstructor <void(*) (ofVec2f, stenum, float)>()
			.LUA_REGDATA(TransformEnt, anchor)
			.LUA_REGDATA(TransformEnt, name)
			.LUA_REGDATA(TransformEnt, pos)
			.LUA_REGDATA(TransformEnt, pType)
			.LUA_REGDATA(TransformEnt, rotation)
			.LUA_REGDATA(TransformEnt, rotationCentroid)
			.LUA_REGDATA(TransformEnt, size)
			.LUA_REGDATA(TransformEnt, sType)
			.LUA_REGDATA(TransformEnt, zIndex)
			.endClass();

		// StrId
		luabridge::getGlobalNamespace(L)
			.beginClass<StrId>("StrId")
			.addConstructor<void(*)(string)>()
			.addFunction("GetValue", &StrId::GetValue)
			.addFunction("__eq", static_cast<bool(StrId::*)(const StrId &)const>(&StrId::operator==)) // override ==
			.endClass();

		// Flags proxy
		luabridge::getGlobalNamespace(L)
			.beginClass<Flags>("Flags")
			.addConstructor<void(*)(StrId)>()
			.addFunction(LUA_OPERATOR_EQ, static_cast<bool(Vec2i::*)(const Vec2i &)const>(&Vec2i::operator==))
			.addFunction("HasState", &Flags::HasState)
			.addFunction("SetState", &Flags::SetState)
			.addFunction("SwitchState", &Flags::SwitchState)
			.addFunction("ResetState", &Flags::ResetState)
			.endClass();

		// Settings proxy
		luabridge::getGlobalNamespace(L)
			.beginClass<Settings>("Settings")
			.addConstructor<void(*)()>()
			.addFunction("GetSettingVal", static_cast<string(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("GetSettingValInt", static_cast<int(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("GetSettingValFloat", static_cast<float(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("GetSettingValBool", static_cast<bool(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("GetSetting", &Settings::GetSetting)
			.addFunction("SetSetting", &Settings::SetSetting)
			.endClass();

		// Setting
		luabridge::getGlobalNamespace(L)
			.beginClass<Setting>("Setting")
			.addConstructor<void(*)(string)>()
			.addFunction("GetItem", &Setting::GetItem)
			.addFunction("GetItemVal", &Setting::GetItemVal)
			.addFunction("GetItemValDouble", &Setting::GetItemValDouble)
			.addFunction("GetItemValInt", &Setting::GetItemValInt)
			.addFunction("GetItemValFloat", &Setting::GetItemValFloat)
			.addFunction("GetItemValBool", &Setting::GetItemValBool)
			.addFunction("GetItemVals", &Setting::GetItemVals)
			.addFunction("AddItem", &Setting::AddItem)
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<SettingItem>("SettingItem")
			.addConstructor<void(*)(string)>()
			.addFunction("AddValues", static_cast<string(Settings::*)(string, string)> (&Settings::GetSettingVal))
			.addFunction("HasMoreValues", &SettingItem::HasMoreValues)
			.addFunction("GetValues", &SettingItem::GetValues)
			.addFunction("GetValStr", &SettingItem::GetValStr)
			.addFunction("GetValDouble", &SettingItem::GetValDouble)
			.addFunction("GetValInt", &SettingItem::GetValInt)
			.addFunction("GetValFloat", &SettingItem::GetValFloat)
			.addFunction("GetValBool", &SettingItem::GetValBool)
			.endClass();


		luabridge::getGlobalNamespace(L)
			.beginClass<Scene>("Scene")
			.addFunction("FindBehaviorById", &Scene::FindBehaviorById)
			.addFunction("FindNodeById", &Scene::FindNodeById)
			.addFunction("FindNodeByNetworkId", &Scene::FindNodeByNetworkId)
			.addFunction("FindNodeByTag", &Scene::FindNodeByTag)
			.addProperty("name", &Scene::GetName, &Scene::SetName)
			.addFunction("GetName", &Scene::GetName)
			.addFunction("GetSceneNode", &Scene::GetSceneNode)
			.addFunction("Initialized", &Scene::Initialized)
			.addFunction("Loaded", &Scene::Loaded)
			.addFunction("SendMessage", &Scene::SendMessage)
			.addFunction("Reload", &Scene::Reload)
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<Component>("Component")
			.endClass();

		luabridge::getGlobalNamespace(L)
			.beginClass<BehaviorLua>("BehaviorProxy")
			.addConstructor<void(*)(void)>()
			.addFunction("SubscribeForMessages", &BehaviorLua::SubscribeForMessagesLua)
			.addFunction("SendMessage", &BehaviorLua::SendMessage)
			.addCFunction("RegisterDelegate", &BehaviorLua::RegisterDelegateCt)
			.addData("owner", &BehaviorLua::ownerLua)
			.endClass();

		
		luabridge::getGlobalNamespace(L)
			.beginClass<AttribAnimator>("AttribAnimator")
			.addConstructor<void(*)()>()
			.addFunction("Load", &AttribAnimator::Load)
			.endClass();
			

		luabridge::getGlobalNamespace(L)
			.deriveClass<Stage, Component>("Stage")
			.addProperty("rootObject", &Stage::GetRootObject)
			.addProperty("actualScene", &Stage::GetActualScene)
			.addFunction("FindSceneByName", &Stage::FindSceneByName)
			.addFunction("SwitchToScene", &Stage::SwitchToScene)
			.addFunction("SwitchBackToScene", &Stage::SwitchBackToScene)
			.endClass();

		luabridge::getGlobalNamespace(L)
			.deriveClass<Resources, Component>("Resources")
			.addFunction("Get2DImage", &Resources::Get2DImage)
			.addFunction<Settings&(Resources::*)()>("GetDefaultSettings", &Resources::GetDefaultSettings)
			.addFunction<Settings&(Resources::*)()>("GetGlobalSettings", &Resources::GetGlobalSettings)
			.addFunction<Settings&(Resources::*)()>("GetProjectSettings", &Resources::GetProjectSettings)
			.addFunction("GetResourceStr", &Resources::GetResourceStr)
			.endClass();

		// Node
		luabridge::getGlobalNamespace(L)
			.beginClass<Node>("Node")
			.addProperty("tag", &Node::GetTag, &Node::SetTag)
			.addFunction("GetTag", &Node::GetTag)
			.addFunction("SetTag", &Node::SetTag)
			.addFunction("GetTransform", &Node::GetTransform)
			.addFunction("SetTransform", &Node::SetTransform)
			.addFunction<Node* (Node::*)() const>("GetParent", &Node::GetParent)
			.addProperty("root", &Node::GetRoot)
			.addFunction("GetRoot", &Node::GetRoot)
			.addFunction("RemoveFromParent", &Node::RemoveFromParent)
			.addProperty("scene", &Node::GetScene)
			.addFunction("GetScene", &Node::GetScene)
			.addFunction("HasAttr", &Node::HasAttr)
			.addFunction("RemoveAttr", &Node::RemoveAttr)
			.addFunction("AddAttrString", &Node::AddAttr<string>)
			.addFunction("AddAttrInt", &Node::AddAttr<int>)
			.addFunction("AddAttrFloat", &Node::AddAttr<float>)
			.addFunction("AddAttrVector2f", &Node::AddAttr<ofVec2f>)
			.addFunction("AddAttrVector3f", &Node::AddAttr<ofVec3f>)
			.addFunction("AddAttrVec2i", &Node::AddAttr<Vec2i>)
			.addFunction("AddAttrPtr", &Node::AddAttr<void*>)
			.addFunction("GetAttrString", &Node::GetAttr<string>)
			.addFunction("GetAttrInt", &Node::GetAttr<int>)
			.addFunction("GetAttrFloat", &Node::GetAttr<float>)
			.addFunction("GetAttrVector2f", &Node::GetAttr<ofVec2f>)
			.addFunction("GetAttrVector3f", &Node::GetAttr<ofVec3f>)
			.addFunction("GetAttrVec2i", &Node::GetAttr<Vec2i>)
			.addFunction("GetMesh", reinterpret_cast<Mesh*(Node::*)() const> (&Node::GetMeshPtr))
			.addFunction("GetImage", reinterpret_cast<Image*(Node::*)() const> (&Node::GetMeshPtr))
			.addFunction("GetSprite", reinterpret_cast<SpriteMesh*(Node::*)() const> (&Node::GetMeshPtr))
			.addFunction("GetMultiSprite", reinterpret_cast<MultiSpriteMesh*(Node::*)() const> (&Node::GetMeshPtr))
			.addFunction("GetText", reinterpret_cast<Text*(Node::*)() const> (&Node::GetMeshPtr))
			.endClass();



		luabridge::getGlobalNamespace(L)
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
			.addFunction("CogStopAllSounds", &FacadeLua::CogStopAllSounds)
			.addFunction("CogPreloadXMLFile", &FacadeLua::CogPreloadXMLFile)
			.addFunction("CogGetComponent_Stage", &FacadeLua::CogGetComponent<Stage>)
			.addFunction("CogGetComponent_Resources", &FacadeLua::CogGetComponent<Resources>)
			.addCFunction("CogRegisterBehaviorPrototype", &FacadeLua::CogRegisterBehaviorPrototypeCt);
	}

} // namespace