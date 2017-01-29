#include "NodeLua.h"
#include "Node.h"
#include "ofxCogEngine.h"
#include "Stage.h"
#include "LuaScripting.h"
#include "Scene.h"
#include "ComponentStorage.h"
#include "BehaviorLua.h"
#include "Mesh.h"
#include <LuaBridge.h>
#include "AttribAnimator.h"
#include "Scene.h"

using namespace luabridge;

namespace Cog {

	NodeLua::NodeLua(string tag) {
		node = new Node(tag);
		auto scr = GETCOMPONENT(LuaScripting);
		scr->StoreNode(this);
	}

	void NodeLua::AddToActualScene() {
		auto mgr = GETCOMPONENT(Stage);
		mgr->GetActualScene()->GetSceneNode()->AddChild(node);
	}

	void NodeLua::UpdateTransform(bool deep) {
		node->UpdateTransform(deep);
	}

	void NodeLua::Update(const uint64 delta, const uint64 absolute) {
		node->Update(delta, absolute);
	}

	void NodeLua::Draw(const uint64 delta, const uint64 absolute) {
		node->Draw(delta, absolute);
	}

	bool NodeLua::AddBehavior(Behavior* beh) {
		if (beh->IsExternal()) {
			static_cast<BehaviorLua*>(beh)->SetOwnerLua(this);
		}
		return node->AddBehavior(beh);
	}

	bool NodeLua::RemoveBehavior(Behavior* beh, bool erase) {
		return node->RemoveBehavior(beh, erase);
	}

	bool NodeLua::RemoveAttr(StrId key, bool erase) {
		return node->RemoveAttr(key, erase);
	}

	bool NodeLua::HasAttr(StrId key) const {
		return node->HasAttr(key);
	}

	void NodeLua::SubmitChanges(bool applyToChildren) {
		node->SubmitChanges(applyToChildren);
	}

	bool NodeLua::AddChild(NodeLua* child) {
		return node->AddChild(child->node);
	}

	bool NodeLua::RemoveChild(NodeLua* child, bool erase) {
		return node->RemoveChild(child->node, erase);
	}

	NodeLua* NodeLua::GetParent() const {
		return new NodeLua(node->GetParent());
	}

	void NodeLua::SetParent(NodeLua* val) {
		node->SetParent(val->node);
	}

	SceneLua* NodeLua::GetScene() const {
		return new SceneLua(node->GetScene());
	}

	int NodeLua::GetId() const {
		return node->GetId();
	}

	string NodeLua::GetTag() const {
		return node->GetTag();
	}

	Trans& NodeLua::GetTransform() const {
		return node->GetTransform();
	}

	void NodeLua::SetTransform(Trans val) {
		node->SetTransform(val);
	}

	bool NodeLua::HasGroups() const {
		return node->HasGroups();
	}

	Flags& NodeLua::GetGroups() const {
		return node->GetGroups();
	}

	void NodeLua::SetGroups(Flags val) {
		node->SetGroups(val);
	}

	bool NodeLua::IsInGroup(StrId group) {
		return node->IsInGroup(group);
	}

	void NodeLua::SetGroup(StrId group) {
		node->SetGroup(group);
	}

	void NodeLua::UnsetGroup(StrId group) {
		node->UnsetGroup(group);
	}

	bool NodeLua::HasStates() const {
		return node->HasStates();
	}

	Flags& NodeLua::GetStates() const {
		return node->GetStates();
	}

	void NodeLua::SetStates(Flags val) {
		node->SetStates(val);
	}

	bool NodeLua::HasState(StrId state) const{
		return node->HasState(state);
	}

	void NodeLua::SetState(StrId state) {
		node->SetState(state);
	}

	void NodeLua::ResetState(StrId state) {
		node->ResetState(state);
	}

	void NodeLua::SwitchState(StrId state1, StrId state2) {
		node->SwitchState(state1, state2);
	}

	void NodeLua::AddAttrString(StrId key, string val) {
		node->AddAttr(key, val);
	}

	void NodeLua::AddAttrInt(StrId key, int val) {
		node->AddAttr(key, val);
	}

	void NodeLua::AddAttrFloat(StrId key, float val) {
		node->AddAttr(key, val);
	}

	void NodeLua::AddAttrVector2f(StrId key, ofVec2f val) {
		node->AddAttr(key, val);
	}

	void NodeLua::AddAttrVector3f(StrId key, ofVec3f val) {
		node->AddAttr(key, val);
	}

	void NodeLua::AddAttrVec2i(StrId key, Vec2i val) {
		node->AddAttr(key, val);
	}

	string NodeLua::GetAttrString(StrId key) {
		return node->GetAttr<string>(key);
	}

	int NodeLua::GetAttrInt(StrId key) {
		return node->GetAttr<int>(key);
	}

	float NodeLua::GetAttrFloat(StrId key) {
		return node->GetAttr<float>(key);
	}

	ofVec2f NodeLua::GetAttrVector2f(StrId key) {
		return node->GetAttr<ofVec2f>(key);
	}

	ofVec3f NodeLua::GetAttrVector3f(StrId key) {
		return node->GetAttr<ofVec3f>(key);
	}

	Vec2i NodeLua::GetAttrVec2i(StrId key) {
		return node->GetAttr<Vec2i>(key);
	}

	Text* NodeLua::GetTextMesh() {
		auto text = node->GetMesh<Text>();
		return text.get();
	}

	Image* NodeLua::GetImageMesh() {
		auto image = node->GetMesh<Image>();
		return image.get();
	}

	void NodeLua::SetImageMesh(string image) {
		auto img = CogGet2DImage(image);
		if (node->HasMeshType(MeshType::IMAGE)) {
			node->GetMesh<Image>()->SetImage(img);
		}
		else {
			node->SetMesh(spt<Image>(new Image(img)));
		}
	}

	void NodeLua::InitLuaMapping(luabridge::lua_State* L) {

		luabridge::getGlobalNamespace(L)
			.beginClass<NodeLua>("Node")
			.addConstructor<void(*)(string)>()
			.addProperty("id", &NodeLua::GetId)
			.addProperty("parent", &NodeLua::GetParent, &NodeLua::SetParent)
			.addProperty("scene", &NodeLua::GetScene)
			.addProperty("tag", &NodeLua::GetTag)
			.addProperty("transform", &NodeLua::GetTransform, &NodeLua::SetTransform)
			.addFunction("AddBehavior", &NodeLua::AddBehavior)

			// specify all polymorphic objects here!
			.addFunction("AddBehavior", static_cast<bool(NodeLua::*)(Behavior*)> (&NodeLua::AddBehavior))
			.addFunction("AddBehavior", reinterpret_cast<bool(NodeLua::*)(BehaviorLua*)> (&NodeLua::AddBehavior))
			.addFunction("AddBehavior", reinterpret_cast<bool(NodeLua::*)(AttribAnimator*)> (&NodeLua::AddBehavior))


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
	}

} // namespace