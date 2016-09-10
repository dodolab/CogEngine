#include "NodeLua.h"
#include "Node.h"

namespace Cog {

	NodeLua::NodeLua(string tag) {
		node = new Node(tag);
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
		return node->AddBehavior(beh);
	}

	bool NodeLua::RemoveBehavior(BehaviorLua* beh, bool erase) {
		return node->RemoveBehavior(beh, erase);
	}

	bool NodeLua::RemoveAttr(StringHash key, bool erase) {
		return node->RemoveAttr(key, erase);
	}

	bool NodeLua::HasAttr(StringHash key) const {
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
		// todo: use prototype instead
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


} // namespace