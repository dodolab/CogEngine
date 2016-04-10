#include "NodeLua.h"
#include "Node.h"
#include "CogEngine.h"
#include "Stage.h"
#include "LuaScripting.h"
#include "Scene.h"

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

	bool NodeLua::AddBehavior(BehaviorLua* beh) {
		beh->SetOwnerLua(this);
		return node->AddBehavior(beh);
	}

	bool NodeLua::RemoveBehavior(BehaviorLua* beh, bool erase) {
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

} // namespace