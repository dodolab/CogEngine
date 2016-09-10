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

		~NodeLua() {
			bool mojo = false;
		}

		void AddToActualScene();

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

		bool HasGroups() const;

		Flags& GetGroups() const;

		void SetGroups(Flags val);

		bool IsInGroup(StringHash group);

		void SetGroup(StringHash group);

		void UnsetGroup(StringHash group);

		bool HasStates() const;

		Flags& GetStates() const;

		void SetStates(Flags val);

		bool HasState(StringHash state) const;

		void SetState(StringHash state);

		void ResetState(StringHash state);

		void SwitchState(StringHash state1, StringHash state2);

		void AddAttrString(StringHash key, string val);

		void AddAttrInt(StringHash key, int val);

		void AddAttrFloat(StringHash key, float val);

		void AddAttrVector2f(StringHash key, ofVec2f val);

		void AddAttrVector3f(StringHash key, ofVec3f val);

		void AddAttrVec2i(StringHash key, Vec2i val);

		string GetAttrString(StringHash key);
		
		int GetAttrInt(StringHash key);

		float GetAttrFloat(StringHash key);

		ofVec2f GetAttrVector2f(StringHash key);

		ofVec3f GetAttrVector3f(StringHash key);
		
		Vec2i GetAttrVec2i(StringHash key);
	};

} // namespace