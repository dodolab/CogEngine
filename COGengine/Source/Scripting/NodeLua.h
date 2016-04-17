#pragma once

#include "ofVec2f.h"
#include "Definitions.h"
#include "StrId.h"
#include "Flags.h"
#include "Transform.h"
#include "Vec2i.h"

namespace Cog {

	class Node;
	class BehaviorLua;


	/**
	* Node wrapper for Lua script
	*/
	class NodeLua {
	public:
		Node* node;

		NodeLua(Node* node):node(node) {

		}

		NodeLua(string tag);

		~NodeLua() {
		}

		void AddToActualScene();

		void UpdateTransform(bool deep);

		void Update(const uint64 delta, const uint64 absolute);

		void Draw(const uint64 delta, const uint64 absolute);

		bool AddBehavior(BehaviorLua* beh);

		bool RemoveBehavior(BehaviorLua* beh, bool erase);

		bool RemoveAttr(StrId key, bool erase);

		bool HasAttr(StrId key) const;

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

		bool IsInGroup(StrId group);

		void SetGroup(StrId group);

		void UnsetGroup(StrId group);

		bool HasStates() const;

		Flags& GetStates() const;

		void SetStates(Flags val);

		bool HasState(StrId state) const;

		void SetState(StrId state);

		void ResetState(StrId state);

		void SwitchState(StrId state1, StrId state2);

		void AddAttrString(StrId key, string val);

		void AddAttrInt(StrId key, int val);

		void AddAttrFloat(StrId key, float val);

		void AddAttrVector2f(StrId key, ofVec2f val);

		void AddAttrVector3f(StrId key, ofVec3f val);

		void AddAttrVec2i(StrId key, Vec2i val);

		string GetAttrString(StrId key);
		
		int GetAttrInt(StrId key);

		float GetAttrFloat(StrId key);

		ofVec2f GetAttrVector2f(StrId key);

		ofVec3f GetAttrVector3f(StrId key);
		
		Vec2i GetAttrVec2i(StrId key);
	};

} // namespace