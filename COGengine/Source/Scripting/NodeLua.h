#pragma once

#include "BehaviorLua.h"

namespace Cog {

	class Node;

	class NodeLua {
	public:
		Node* node;

		NodeLua(Node* node):node(node) {

		}

		NodeLua(string tag);

		void UpdateTransform(bool deep);

		void Update(const uint64 delta, const uint64 absolute);

		void Draw(const uint64 delta, const uint64 absolute);

		bool AddBehavior(BehaviorLua* beh);

		bool RemoveBehavior(BehaviorLua* beh, bool erase);

		bool RemoveAttr(StringHash key, bool erase);

		bool HasAttr(StringHash key) const;

		void SubmitChanges(bool applyToChildren);

		bool AddChild(NodeLua* child);

		bool RemoveChild(NodeLua* child, bool erase);

		NodeLua* GetParent() const;

		void SetParent(NodeLua* val);

		int GetId() const;

		string GetTag() const;

		Trans& GetTransform() const;

		void SetTransform(Trans val);

	};

} // namespace