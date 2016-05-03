#include "SceneLua.h"
#include "NodeLua.h"
#include "BehaviorLua.h"
#include "Scene.h"

namespace Cog {

	NodeLua* SceneLua::GetSceneNode() {
		return new NodeLua(scene->GetSceneNode());
	}

	string SceneLua::GetName() const {
		return scene->GetName();
	}

	void SceneLua::SetName(string name) {
		scene->SetName(name);
	}

	bool SceneLua::Loaded() {
		return scene->Loaded();
	}

	bool SceneLua::Initialized() const {
		return scene->Initialized();
	}

	void SceneLua::SendMessage(MsgLua& msg) {
		auto rawMsg = Msg(msg.GetAction(), MsgObjectType::BEHAVIOR, msg.GetSenderId(), MsgObjectType::SUBSCRIBERS, msg.GetContextNode()->GetRawNode(), spt<MsgPayload>());
		scene->SendMessage(rawMsg);
	}

	NodeLua* SceneLua::FindNodeById(int id) const {
		auto node = scene->FindNodeById(id);
		if (node != nullptr) return new NodeLua(node);
		else return nullptr;
	}

	BehaviorLua* SceneLua::FindBehaviorById(int id) const {
		auto beh = scene->FindBehaviorById(id);
		return static_cast<BehaviorLua*>(beh);
	}

	NodeLua* SceneLua::FindNodeByTag(string tag) const {
		auto node = scene->FindNodeByTag(tag);
		if (node != nullptr) return new NodeLua(node);
		else return nullptr;
	}

	NodeLua* SceneLua::FindNodeBySecondaryId(int secondaryId) const {
		auto node = scene->FindNodeBySecondaryId(secondaryId);
		if (node != nullptr) return new NodeLua(node);
		else return nullptr;
	}

} // namespace